#include "exti.h"
#include "delay.h"

void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	  //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//大于等于50最好开启IO补偿 //GPIO_Speed_100MHz;//100M
	GPIO_Init(GPIOC, &GPIO_InitStructure);	    //根据设定参数初始化GPIOC
		
    //GPIOE.0 中断线以及中断初始化配置   下降沿触发
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);//PC8

  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}
