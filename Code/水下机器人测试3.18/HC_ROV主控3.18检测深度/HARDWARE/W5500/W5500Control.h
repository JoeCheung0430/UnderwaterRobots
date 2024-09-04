#ifndef	__W5500CONTROL_H_
#define	__W5500CONTROL_H_

#include "sys.h"
#include "W5500.h"

//data_typedain 宏定义
#define INT		  0	
#define FLOAT		1	

//data_category 宏定义
#define MPU_YPR       0	 //mpu 的ypr参数
#define DEEP		      1	 //水深
#define SERVO_CUR		  2	 //舵机电流
#define MOTOR_PWM		  3	 //电机pwm
#define BATTERY		    4	 //电池电量
#define DIRECTION		  5	 //方向
#define PID_DATA      6  //PID参数

typedef unsigned char SOCKET;			//自定义端口号数据类型
typedef union  
{
		unsigned char  byte[4];   
		int            data_int;
	  float          data_float;
}DataToByte;                      //自定义发送接收数据类型

void Delay(unsigned int d);//延时函数(ms)
void W5500_Initialization(void);
void Load_Net_Parameters(void);
void W5500_Socket_Set(void);
void Process_Socket_Data(SOCKET s);
void W5500Control_Init(void);
void W5500_Send_Data(unsigned char data_type,unsigned char data_category,int data_size,DataToByte data[]);

extern unsigned int W5500_Send_Delay_Counter;
extern unsigned int Timer2_Counter;

#endif
