#include "MOTOR_P_N.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"

void motorIO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
	//水平舵机
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //水平四个舵机信号线
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_12);						 
	GPIO_SetBits(GPIOB,GPIO_Pin_13);						 
	GPIO_SetBits(GPIOB,GPIO_Pin_14);					
	GPIO_SetBits(GPIOB,GPIO_Pin_15);						

	//垂直舵机
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;	    		 //垂直舵机
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_5); 					
	GPIO_SetBits(GPIOB,GPIO_Pin_4); 
	GPIO_SetBits(GPIOB,GPIO_Pin_3); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOA,GPIO_Pin_15); 
}