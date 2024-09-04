#include "led.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"

void LED_Init(void)
{        
  GPIO_InitTypeDef  GPIO_InitStructure;                       //�ѽṹ���ַ�����һ������GPIO_InitStructure
	                                                                                                                                     	//ʹ��gpioA��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);//�漰���.h�ļ���ע��#include��Ҫ©��
	
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;                           //��8��
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;                           //��8��
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
}

