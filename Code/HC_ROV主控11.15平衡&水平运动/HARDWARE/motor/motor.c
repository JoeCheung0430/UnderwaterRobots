#include "Motor.h"
#include "usart.h"
#include "DataProtocol.h"
#include "W5500Control.h"	

//标志位
extern uint8_t Deep_Flag;
extern uint8_t Deep_Open; 
int Vertical_Pwm;
int lzq=1;
//灯光控制
void Light_Control()//照明灯 
{
	
	SendPwm(LightPWM_,LightCount,Light_Pwm_Flag ,0 ,SmallLight_MaxPWM,SmallLight_MiniPWM,LightUSART);
	
}

//水平油门控制
void Forward_Throttle()
{
	int i;
	delay_ms(10);				//不加延时会有震荡
	SendPwm(Motor_v,MotorCount/2,Motor_Pwm_Flag ,0 ,Motor_PwmMax,Motor_PwmMin,MotorUSART);
	//SendPwm(MotorPWM_,MotorCount/2,Motor_Pwm_Flag ,4 ,Motor_PwmMax,Motor_PwmMin,MotorUSART);
	Get_ypr();
	Horizontal_Control();
//	for(i=0;i<8;i++)
//	{
//		printf("motor[%d] = %d    ",i,Motor_v[i].data_int);
//	}
//	printf("\r\n");
}

//垂直方向控制
void Vertical_Throttle()
{
	Vertical_Pwm = Motor_v[4].data_int;
	if(Deep_Open == 1& MotorPWM_[4].data_int != 0) 
	{
		Deep_Flag = 1;
	}
}

//舵机控制
void Servo_Throttle()
{
	int i=0;
//	for(i=0;i<4;i++)
//	{
//		printf("ServoPWM_%d = %d\t",i,ServoPWM_[i].data_int);
//	}
	printf("\r\n");
	SendPwm(ServoPWM_,ServoCount,Servo_PWM_Flag,0 ,Servo_PwmMax,Servo_PwmMin,ServoUSART);
}

//发送pwm数据给从机
void SendPwm(DataToByte pwm[],int size,int type,int num ,int max,int min,USART_TypeDef *usart)
{
	u8 i;
	for (i=0;i<size;i++)
	{
		if (pwm[i+num].data_int<=min)
			pwm[i+num].data_int=min;
		
		if (pwm[i+num].data_int>max)
			pwm[i+num].data_int=max;
		
		SendPwmtoSlave( pwm[i+num],type,i+num,usart);
	}
}

//串口发送pwm数据到驱动板
void SendPwmtoSlave(DataToByte pwm,int type,int num ,USART_TypeDef *usart)
{
	int i;
	usart_Sendbyte(usart,type); //数据种类
	usart_Sendbyte(usart,num);  //序号位
	for(i =0;i<4;i++)
	{
		usart_Sendbyte(usart,pwm.byte[i]);
	}
	usart_Sendbyte(usart,0x01);  //占位 无意义
	usart_Sendbyte(usart,0x0a);  //占位 无意义
}

//以前函数，暂且保留
//电调解锁初始化
void RangeSetting(void)
{
	PWMfresh(2000,2000,2000,2000,2000,2000,2000,2000);

	PWMfresh(1500,1500,1500,1500,1500,1500,1500,1500);

}

//以前函数，暂且保留
//电调解锁函数
void PWMfresh(int16_t Motor1_PWM,int16_t Motor2_PWM,int16_t Motor3_PWM,int16_t Motor4_PWM,int16_t Motor5_PWM,int16_t Motor6_PWM,int16_t Motor7_PWM,int16_t Motor8_PWM)
{
	
	if(Motor1_PWM>=Motor_PwmMax) Motor1_PWM = Motor_PwmMax;
	if(Motor2_PWM>=Motor_PwmMax) Motor2_PWM = Motor_PwmMax;
	if(Motor3_PWM>=Motor_PwmMax) Motor3_PWM = Motor_PwmMax;
	if(Motor4_PWM>=Motor_PwmMax) Motor4_PWM = Motor_PwmMax;
	if(Motor5_PWM>=Motor_PwmMax) Motor5_PWM = Motor_PwmMax;
	if(Motor6_PWM>=Motor_PwmMax) Motor6_PWM = Motor_PwmMax;
	if(Motor7_PWM>=Motor_PwmMax) Motor7_PWM = Motor_PwmMax;
	if(Motor8_PWM>=Motor_PwmMax) Motor8_PWM = Motor_PwmMax;

	if(Motor1_PWM<=0) Motor1_PWM = 0;
	if(Motor2_PWM<=0) Motor2_PWM = 0;
	if(Motor3_PWM<=0) Motor3_PWM = 0;
	if(Motor4_PWM<=0) Motor4_PWM = 0;
	if(Motor5_PWM<=0) Motor5_PWM = 0;
	if(Motor6_PWM<=0) Motor6_PWM = 0;
	if(Motor7_PWM<=0) Motor7_PWM = 0;
	if(Motor8_PWM<=0) Motor8_PWM = 0;

//传数据到电机驱动
}
