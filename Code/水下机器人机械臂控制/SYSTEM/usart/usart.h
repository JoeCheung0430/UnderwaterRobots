//#ifndef _USART_H_
//#define _USART_H_
//#include "sys.h" 

//#define  USART2_RS485                        0 
//#if USART2_RS485==1
////USART_SendData函数单独使用前后需要开关使能
//#define  USART2_RS485_TX_EN_GPIOx        GPIOG
//#define  USART2_RS485_TX_EN_Pin     GPIO_Pin_6
//#define  USART2_RS485_TX_EN           PGout(6)
//#endif

//#define  USART2_MAX_TX_LEN		           600 //最大发送缓存字节数，刨去\0 最大-1
//#define  USART2_MAX_RX_LEN		           400 //最大接收缓存字节数，刨去\0 最大-1
//extern   volatile u16 USART2_RX_STA;           //接收状态标记
//extern   u8   USART2_TX_BUF[USART2_MAX_TX_LEN];//发送缓冲区
//extern   u8   USART2_RX_BUF[USART2_MAX_RX_LEN];//接收缓冲区

//#define Servo_head      0x06
//#define motor_head      0x04
//#define light_head      0x05

//#define pwm0_max  180				//机械爪
//#define pwm0_min	130
//#define pwm1_max  210
//#define pwm1_min	128
//#define pwm2_max  255
//#define pwm2_min	158
//#define pwm3_max  222
//#define pwm3_min	66

// 
//void     USART2_Init(u32 botelv);
//void     USART2_printf (char *fmt, ...);
//void     USART2_Send_Array (u8 *array,u16 num);
//void 		 overflow_deal();
//#endif

#ifndef _USART3_H_
#define _USART3_H_
#include "sys.h" 

#define  USART3_RS485                        0 
#if USART3_RS485==1
//USART_SendData函数单独使用前后需要开关使能
#define  USART3_RS485_TX_EN_GPIOx        GPIOG
#define  USART3_RS485_TX_EN_Pin     GPIO_Pin_7
#define  USART3_RS485_TX_EN           PGout(7)
#endif

#define Servo_head      0x06
#define  USART3_MAX_TX_LEN		           600 //最大发送缓存字节数，刨去\0 最大-1
#define  USART3_MAX_RX_LEN		           400 //最大接收缓存字节数，刨去\0 最大-1
extern   volatile u16 USART3_RX_STA;           //接收状态标记
extern   u8   USART3_TX_BUF[USART3_MAX_TX_LEN];//发送缓冲区
extern   u8   USART3_RX_BUF[USART3_MAX_RX_LEN];//接收缓冲区
 
void     USART3_Init(u32 botelv);
void     USART3_printf (char *fmt, ...);
void     USART3_Send_Array (u8 *array,u16 num);
#endif

	


