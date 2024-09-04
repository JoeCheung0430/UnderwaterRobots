#include "DataProtocol.h"
#include "usart.h"
#include "stm32f4xx.h"
#include "send_ypr.h"
#include "MS5837.h"
//#include "system.h"

//接收的部分参数
DataToByte MotorPWM_[MotorCount];     //0-3 垂直电机 4-7 水平电机
DataToByte Motor_v[MotorCount];
DataToByte LightPWM_[LightCount];     //0 大灯 1 小灯
DataToByte PidData_[PidCount];        //6个  Rov.Angle_P 、Rov.Angle_I 、Rov.Velocity_P 、Rov.Velocity_D、Deep_KP 、Deep_KD
DataToByte ServoPWM_[ServoCount];     //舵机pwm  4个
DataToByte ServoCurrent_[ServoCount]; //舵机电流  4个
DataToByte DeepLzq[1];								//存储深度
DataToByte ElcAndDir[ElcAndDirCount]; //电池电压和方向 0：电池电压 、1：方向
extern int ifconnect;

//标志位
uint8_t Deep_Flag;
uint8_t Deep_Open;

//未完成
void SendDataToComputer(void)
{
	DataToByte data[MpuDataCount];
	data[0].data_float=Pitch;
	data[1].data_float=Roll;
	data[2].data_float=Yaw;
	data[3].data_float=Deep;
	
//	W5500_Send_Data(FLOAT,MPU_Data,MpuDataCount,data);
//	W5500_Send_Data(FLOAT,Servo_Current,ServoCount,ServoCurrent_);
//	W5500_Send_Data(FLOAT,Elc_Dir,ElcAndDirCount,ElcAndDir);
}



void SetData(void)
{
	u8 num;
	int i;
	int head_now=0;	
	unsigned short size;
	size=Read_SOCK_Data_Buffer(0, Rx_Buffer);	
	while(head_now<size)
	{
		switch (Rx_Buffer[head_now+1]) //判断数据种类
		{
			case Motor:
				Set(Rx_Buffer[head_now+2],Motor_v,head_now);
			printf("enter motor\r\n");
				break;
			case Light:
				Set(Rx_Buffer[head_now+2],LightPWM_,head_now);
				break;
			case PID:
				Set(Rx_Buffer[head_now+2],PidData_,head_now);
				break;
			case Servo_PWM:
				Set(Rx_Buffer[head_now+2],ServoPWM_,head_now);
				break;
			case Deep_LZQ:
				Set(Rx_Buffer[head_now+2],DeepLzq,head_now);
				printf("enter deep\r\n");
				break;
		}
		head_now+=(Rx_Buffer[head_now+2]+1)*4;
		
	}
}

void Set(unsigned short size,DataToByte data[],int head_now)
{
	int i, j, k, h;
	
	for (i = head_now+4,h = 0; i < head_now+(size+1)*4; i += 4, h++)
	{
			for (j = i, k = 0; k < 4; j++, k++)
			{
					data[h].byte[k] = Rx_Buffer[j];	  
			}
	}	
}








