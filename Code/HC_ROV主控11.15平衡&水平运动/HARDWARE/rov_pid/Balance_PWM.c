#include "Balance_PWM.h"
#include "stdint.h"
#include "send_ypr.h"
#include "DataProtocol.h"
#include "Motor.h"

//PID����
float Balance_KP;
float Balance_KD;
float Deep_KP;
float Deep_KD;
//ʵ��
extern float Deep;
extern float Roll;
extern float Deep_Tar;
extern int   Vertical_Pwm;		//�ǵÿ�

//��־λ
extern uint8_t Deep_Flag;
extern uint8_t Deep_Open;

Cascade_Pid Rov;				//PID����

float Deep_fix(float input,float Target)			//���PD		8��3�� Deep_KP = 6.5��Deep_KD = 6.5
{
	float EX_Deep,Last_EX;
	float deep;
	EX_Deep = (Target - input)*100;	
	if(EX_Deep < 0) return 0;
	deep = 0.6*EX_Deep + 0.6*(EX_Deep - Last_EX);
	Last_EX = EX_Deep;
	return deep;
}

float Double_Balance_Pos(float input,Cascade_Pid Double_Pid)				//λ��ʽPI-PD				8��3�� ��Angle_P = 2,Angle_I = 0.4,Velocity = 6.5,Velocity_D = 0.89
{
	float	Angle_Ep,Angle_Ei;		//���������
	float	Velocity_Ep,Velocity_Ed;		//���������

	//�ǶȻ��������	
	printf("input = %f\r\n",input);
	Double_Pid.Angle_Ek = input;
	Angle_Ep = Double_Pid.Angle_Ek;
	Angle_Ei += Double_Pid.Angle_Ek;

	Double_Pid.Angle_Out = 2 * Angle_Ep + 0.4*Angle_Ei;

	//���ٶȻ��������
	Double_Pid.Velocity_Ek	= Double_Pid.Angle_Out;
	Velocity_Ep = Double_Pid.Velocity_Ek;
	Velocity_Ed = Double_Pid.Velocity_Ek - Double_Pid.Velocity_Ek1;

	Double_Pid.Velocity_Out = 6.5*Velocity_Ep + 0.89*Velocity_Ed;
	Double_Pid.Velocity_Ek1 = Double_Pid.Velocity_Ek;			//������
	return Double_Pid.Velocity_Out;							//���ؽ������ǰλ��
}

//float Double_Balance_Pos(float input,Cascade_Pid Double_Pid)				//λ��ʽPI-PD				8��3�� ��Angle_P = 2,Angle_I = 0.4,Velocity = 6.5,Velocity_D = 0.89
//{
//	float	Angle_Ep,Angle_Ei;		//���������
//	float	Velocity_Ep,Velocity_Ed;		//���������

//	//�ǶȻ��������	
//	Double_Pid.Angle_Ek = input;
//	Angle_Ep = Double_Pid.Angle_Ek;
//	Angle_Ei += Double_Pid.Angle_Ek;

//	Double_Pid.Angle_Out = Rov.Angle_P * Angle_Ep + Rov.Angle_I*Angle_Ei;
////	printf("Angle_P = %f\r\n",Double_Pid.Angle_P);
////	printf("Angle_I = %f\r\n",Double_Pid.Angle_I);
//	//���ٶȻ��������
//	Double_Pid.Velocity_Ek	= Double_Pid.Angle_Out;
//	Velocity_Ep = Double_Pid.Velocity_Ek;
//	Velocity_Ed = Double_Pid.Velocity_Ek - Double_Pid.Velocity_Ek1;

//	Double_Pid.Velocity_Out = Rov.Velocity_P*Velocity_Ep + Rov.Velocity_D*Velocity_Ed;
////	printf("Velocity_P = %f\r\n",Double_Pid.Velocity_P);
////	printf("Velocity_D = %f\r\n",Double_Pid.Velocity_D);
//	Double_Pid.Velocity_Ek1 = Double_Pid.Velocity_Ek;			//������
//	return Double_Pid.Velocity_Out;							//���ؽ������ǰλ��
//}

void Horizontal_Control()													//��PWM
{
	int num,i;
	static float Balance_PWM ,Deep_PWM ;
	if(Deep_Flag == 0){
//		Balance_PWM = Balance(Roll);							//ƽ�����
//		Balance_PWM += Double_Balance_Inc(Roll,Rov);	
		Balance_PWM = Double_Balance_Pos(Roll,Rov);
	if(Deep_Open == 1){
		Deep_PWM = Deep_fix(Deep,Deep_Tar);			  //��ȵ���
	}
	else { 
		Deep_PWM = 0;
	}
	}
	else{
		Balance_PWM = 0;
	}
	Deep_PWM = Deep_fix(Deep,DeepLzq[0].data_float);
	//Deep_PWM = Deep_fix(Deep,Deep);
	if(Balance_PWM < Balance_MIN) Balance_PWM = Balance_MIN;
	if(Deep_PWM < Deep_MIN) Deep_PWM = Deep_MIN;

	Balance_PWM = Double_Balance_Pos(Roll,Rov);
	MotorPWM_[5].data_int = (int)(+ Balance_PWM+ Deep_PWM + Vertical_Pwm +35);  //�ǵÿ�
	MotorPWM_[4].data_int = (int)(- Balance_PWM - Deep_PWM + Vertical_Pwm - 7);
	MotorPWM_[7].data_int = (int)(+ Balance_PWM + Deep_PWM + Vertical_Pwm + 33);
	MotorPWM_[6].data_int = (int)(- Balance_PWM- Deep_PWM + Vertical_Pwm - 6);
	printf("deep = %f			lzqDeep=%f\r\n",Deep,DeepLzq[0].data_float);
	for(i=4;i<8;i++)
	{
		printf("pwm[%d]=%d",i,MotorPWM_[i].data_int);
	}
	printf("\r\n");
	printf("Deep_PWM = %f\r\n",Deep_PWM);
	printf("Balance_PWM = %f\r\n",Balance_PWM);
//	printf("motor_pwm = %d\r\n",MotorPWM_[5].data_int);
//	printf("DeepLzq[1] = %d\r\n",DeepLzq[0].data_int);
	//��ֱ����������
	//SendPwm(MotorPWM_,MotorCount/2,Motor_Pwm_Flag ,4 ,Motor_PwmMax,Motor_PwmMin,MotorUSART);
//	delay_ms(100);
	
}

