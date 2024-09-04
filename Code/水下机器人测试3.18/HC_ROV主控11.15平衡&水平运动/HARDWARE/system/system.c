#include "system.h"

//PID����
extern float Balance_KP;
extern float Balance_KD;
extern float Deep_KP;
extern float Deep_KD;

//��־λ
extern uint8_t Deep_Flag;
extern uint8_t Deep_Open;
extern int ifconnect;

extern float Deep;					//��ǰ���
extern float Deep_Tar;			//�趨���			
extern Cascade_Pid Rov;     //pid�����ṹ��

//ϵͳ��ʼ��
void System_Init()
{
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	uart1_init(115200);		  //������ƹ�pwm���
	uart2_init(115200);		  //���pwm���
	uart3_init(115200);		  //������Ϣ
	
	i2cInit();	            //MPU9250��Ӧ��IIC���ߵĳ�ʼ��
	#if LOG
		printf("MPU9250 i2cInit succeed!\r\n"); 
	#endif

	IIC_Init();	            //��ʼ��ˮѹIIC
	#if LOG
		printf("ˮѹ������ IIC_Init succeed!\r\n");
	#endif 
	delay_ms(10); 

	MS583703BA_RESET();		  //��λMS5837
	MS5837_init();	    	  //��ʼ��MS5837��ˮѹ��������
	#if LOG
		printf("ˮѹ������ MS5837_init succeed!\r\n");	
	#endif 
	delay_ms(10);	 
	
	 while(Mpu_Init(0));	    //0����ˮƽΪ��ȣ�1���Ե�ǰΪ��� 	
	 #if LOG
	 	printf("MPU9250 Mpu_Init succeed!\r\n");
	 #endif		
	 delay_ms(10);
	
	RangeSetting();
	#if LOG
		printf("�����ʼ��RangeSetting succeed!\r\n");	
	#endif
	delay_ms(10);
	
	TIM3_Int_Init(10-1,8400-1);	    //��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10��Ϊ1ms	
	W5500Control_Init();     //����W5500
	#if LOG
	printf("���� W5500Control_Init succeed!\r\n");
	#endif		
	
//	EXTIX_Init();					          //�жϵȼ���2		����
//	Timer1_Init(200-1,8400-1);			//��ʱ��ʱ��168M����Ƶϵ��8400������168M/8400=20Khz�ļ���Ƶ�ʣ�����200��Ϊ10ms  �жϵȼ�: 3   ��ʱ10ms��ȡһ������ //Timer1_Init(100-1,16800-1); 
//	#if LOG
		printf("System_Init succeed!\r\n");
//	#endif		
}

//���ݳ�ʼ��
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

//����pid����
void UpdataPid()
{
	Rov.Angle_P = PidData_[0].data_float;
	Rov.Angle_I = PidData_[1].data_float;
	Rov.Velocity_P = PidData_[2].data_float;
	Rov.Velocity_D = PidData_[3].data_float;
	Deep_KP = PidData_[4].data_float;
	Deep_KD = PidData_[5].data_float;	
}

//�����˿���
void Control()				     // ��λ��ָ�Χ   Light,Forward:0-1000   Vertical:-500-500
{
//	UpdataPid();           //����pid����
	Light_Control();		   //�ƹ����
	Forward_Throttle();    //ˮƽ����������
	Vertical_Throttle();	 //��ֱ����������ã����������������ֱ���PWM������ Balance_PWM.c Horizontal_Control��
	Servo_Throttle();      //����������
}


void Data_Back()
{
	SendDataToComputer();    //���ݷ��͵���λ��
}

//�����趨����
void Deep_Adjust()			  
{
	if(Deep_Flag == 1) 
	{
	Deep_Tar = Deep;
		Deep_Flag = 0;
	}
}


