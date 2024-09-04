#ifndef __SYSTEM_H
#define __SYSTEM_H 

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "myiic.h"
#include "stm32_iic.h"
#include "motor.h"
#include "MS5837.h"
#include "send_ypr.h"
#include "exti.h"
#include "Balance_PWM.h"
#include "Timer.h"
#include "DataProtocol.h"
#include "W5500Control.h"	

void System_Init(void);
void Data_Init(void);
void Control(void);
void Deep_Adjust(void);
void Data_Back(void);
void UpdataPid();

#endif 

