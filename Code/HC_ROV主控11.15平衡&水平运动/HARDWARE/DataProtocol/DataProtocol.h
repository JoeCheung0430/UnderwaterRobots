#ifndef __DATAProtocol_H
#define __DATAProtocol_H	 

#include "W5500Control.h"	

//数据种类标志
#define Motor          0x01
#define Light          0x02
#define PID            0x03
#define Servo_PWM      0x04  
#define Servo_Current  0x05 
#define MPU_Data       0x06
#define Elc_Dir        0x07
#define Deep_LZQ    	 0x08

//参数数量宏
#define MotorCount      8
#define LightCount      2
#define PidCount        6
#define ServoCount      4
#define ElcAndDirCount  2
#define MpuDataCount    4


void SendDataToComputer(void);
void SetData(void);
void Set(unsigned short size,DataToByte data[],int start_now);

extern DataToByte MotorPWM_[MotorCount];
extern DataToByte Motor_v[MotorCount];
extern DataToByte LightPWM_[LightCount];  
extern DataToByte PidData_[PidCount];
extern DataToByte DeepLzq[1];
extern DataToByte ServoPWM_[ServoCount];
extern DataToByte ServoCurrent_[ServoCount];
extern DataToByte ElcAndDir[ElcAndDirCount];
#endif
