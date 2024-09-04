#ifndef	__W5500CONTROL_H_
#define	__W5500CONTROL_H_

#include "sys.h"
#include "W5500.h"

//data_typedain �궨��
#define INT		  0	
#define FLOAT		1	

//data_category �궨��
#define MPU_YPR       0	 //mpu ��ypr����
#define DEEP		      1	 //ˮ��
#define SERVO_CUR		  2	 //�������
#define MOTOR_PWM		  3	 //���pwm
#define BATTERY		    4	 //��ص���
#define DIRECTION		  5	 //����
#define PID_DATA      6  //PID����

typedef unsigned char SOCKET;			//�Զ���˿ں���������
typedef union  
{
		unsigned char  byte[4];   
		int            data_int;
	  float          data_float;
}DataToByte;                      //�Զ��巢�ͽ�����������

void Delay(unsigned int d);//��ʱ����(ms)
void W5500_Initialization(void);
void Load_Net_Parameters(void);
void W5500_Socket_Set(void);
void Process_Socket_Data(SOCKET s);
void W5500Control_Init(void);
void W5500_Send_Data(unsigned char data_type,unsigned char data_category,int data_size,DataToByte data[]);

extern unsigned int W5500_Send_Delay_Counter;
extern unsigned int Timer2_Counter;

#endif
