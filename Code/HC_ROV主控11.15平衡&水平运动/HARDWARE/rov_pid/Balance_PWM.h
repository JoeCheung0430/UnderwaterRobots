#ifndef __Balance_PWM_H_
#define __Balance_PWM_H_
#define Balance_MIN 0
#define Deep_MIN 0
typedef struct
{
		//外环PI
		float Angle_P;
		float Angle_I;
		float Angle_Ek;			//当前误差
		float Angle_Ek1;		//上一次误差
		float Angle_Out;		
	
		//内环PD
		float Velocity_P;
		float Velocity_D;
		float	Velocity_Ek;
		float Velocity_Ek1;
		float Velocity_Ek2;
	  float Velocity_Out;
	
}Cascade_Pid;

float Double_Balance_Inc(float input,Cascade_Pid Double_Pid);
float Double_Balance_Pos_test(float input,Cascade_Pid Double_Pid);
float Deep_fix(float input,float Target);
void Horizontal_Control(void);

#endif
