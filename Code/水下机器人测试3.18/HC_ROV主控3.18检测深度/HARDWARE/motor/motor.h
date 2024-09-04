#ifndef __MOTOR_H
#define __MOTOR_H

#include "delay.h"
#define Motor_PwmMax 2000
#define Motor_PwmMin 1000

#define Servo_PwmMax 3000
#define Servo_PwmMin 50

#define Light_PwmMax 20000
#define Light_PwmMin 0

#define BigLight_MaxPWM   200
#define BigLight_MiniPWM   0

#define SmallLight_MaxPWM   20000
#define SmallLight_MiniPWM   0

#define Motor_Pwm_Flag  0x04
#define Light_Pwm_Flag  0x05
#define Servo_PWM_Flag  0x06

#define LightUSART USART1  //电机和灯光的PWM通过USART1发送
#define MotorUSART USART1
#define ServoUSART USART2  //舵机的PWM通过USART2发送

void PWMfresh(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM,int16_t MOTO5_PWM,int16_t MOTO6_PWM,int16_t Motor7_PWM,int16_t Motor8_PWM);
void RangeSetting(void);
void Light_Control(void);
void Forward_Throttle(void);
void Vertical_Throttle(void);
void Servo_Throttle(void);
void SendPwmtoSlave(DataToByte pwm,int type,int num ,USART_TypeDef *usart);
void SendPwm(DataToByte pwm[],int size,int type,int num ,int max,int min,USART_TypeDef *usart);

#endif
