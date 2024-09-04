#include "system.h"

//PID参数
extern float Balance_KP;
extern float Balance_KD;
extern float Deep_KP;
extern float Deep_KD;

//标志位
extern uint8_t Deep_Flag;
extern uint8_t Deep_Open;
extern int ifconnect;

extern float Deep;					//当前深度
extern float Deep_Tar;			//设定深度			
extern Cascade_Pid Rov;     //pid参数结构体

//系统初始化
void System_Init()
{
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	uart1_init(115200);		  //电机、灯光pwm输出
	uart2_init(115200);		  //舵机pwm输出
	uart3_init(115200);		  //调试信息
	
	i2cInit();	            //MPU9250对应的IIC总线的初始化
	#if LOG
		printf("MPU9250 i2cInit succeed!\r\n"); 
	#endif

	IIC_Init();	            //初始化水压IIC
	#if LOG
		printf("水压传感器 IIC_Init succeed!\r\n");
	#endif 
	delay_ms(10); 

	MS583703BA_RESET();		  //复位MS5837
	MS5837_init();	    	  //初始化MS5837（水压传感器）
	#if LOG
		printf("水压传感器 MS5837_init succeed!\r\n");	
	#endif 
	delay_ms(10);	 
	
	 while(Mpu_Init(0));	    //0：以水平为零度，1：以当前为零度 	
	 #if LOG
	 	printf("MPU9250 Mpu_Init succeed!\r\n");
	 #endif		
	 delay_ms(10);
	
	RangeSetting();
	#if LOG
		printf("电机初始化RangeSetting succeed!\r\n");	
	#endif
	delay_ms(10);
	
	TIM3_Int_Init(10-1,8400-1);	    //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10次为1ms	
	W5500Control_Init();     //开启W5500
	#if LOG
	printf("网络 W5500Control_Init succeed!\r\n");
	#endif		
	
//	EXTIX_Init();					          //中断等级：2		控制
//	Timer1_Init(200-1,8400-1);			//定时器时钟168M，分频系数8400，所以168M/8400=20Khz的计数频率，计数200次为10ms  中断等级: 3   定时10ms读取一次数据 //Timer1_Init(100-1,16800-1); 
//	#if LOG
		printf("System_Init succeed!\r\n");
//	#endif		
}

//数据初始化
void Data_Init()
{
	u8 num;
	int data=1500;
	for(num=0;num<MotorCount;num++)		
		MotorPWM_[num].data_int=data;
	for(num=0;num<MotorCount;num++)		
		Motor_v[num].data_int=data;
	for(num=0;num<LightCount;num++)		
		LightPWM_[num].data_int=0;

	data=150;
	for(num=0;num<ServoCount;num++)		
		ServoPWM_[num].data_int=150;
	ServoPWM_[0].data_int=180;
	ServoPWM_[1].data_int=174;
	ServoPWM_[2].data_int=255;
	ServoPWM_[3].data_int=222;

	Rov.Angle_P = 2;
	Rov.Angle_I = 0.4;
	Rov.Velocity_P = 6.5;
	Rov.Velocity_D = 0.89;
	Deep_KP = 6.5;
	Deep_KD = 6.5;
	Deep_Tar = 0;
}

//更新pid参数
void UpdataPid()
{
	Rov.Angle_P = PidData_[0].data_float;
	Rov.Angle_I = PidData_[1].data_float;
	Rov.Velocity_P = PidData_[2].data_float;
	Rov.Velocity_D = PidData_[3].data_float;
	Deep_KP = PidData_[4].data_float;
	Deep_KD = PidData_[5].data_float;	
}

//机器人控制
void Control()				     // 上位机指令范围   Light,Forward:0-1000   Vertical:-500-500
{
//	UpdataPid();           //更新pid参数
	Light_Control();		   //灯光输出
	Forward_Throttle();    //水平电机控制输出
	Vertical_Throttle();	 //垂直电机参数设置（并非在这里输出垂直电机PWM，而在 Balance_PWM.c Horizontal_Control）
	Servo_Throttle();      //舵机控制输出
}


void Data_Back()
{
	SendDataToComputer();    //数据发送到上位机
}

//定深设定调整
void Deep_Adjust()			  
{
	if(Deep_Flag == 1) 
	{
	Deep_Tar = Deep;
		Deep_Flag = 0;
	}
}


