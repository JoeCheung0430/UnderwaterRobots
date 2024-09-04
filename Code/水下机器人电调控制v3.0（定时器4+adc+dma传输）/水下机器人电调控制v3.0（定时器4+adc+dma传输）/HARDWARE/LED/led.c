#include "led.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"

void LED_Init(void)
{        
  GPIO_InitTypeDef  GPIO_InitStructure;                       //把结构体地址定义成一个变量GPIO_InitStructure
	                                                                                                                                     	//使能gpioA的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);//涉及多个.h文件，注意#include不要漏掉
	
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;                           //第8脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;                           //第8脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
}

