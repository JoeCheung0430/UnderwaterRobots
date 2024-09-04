#include "exti.h"
#include "delay.h"

void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //ʹ��PE�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	  //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���ڵ���50��ÿ���IO���� //GPIO_Speed_100MHz;//100M
	GPIO_Init(GPIOC, &GPIO_InitStructure);	    //�����趨������ʼ��GPIOC
		
    //GPIOE.0 �ж����Լ��жϳ�ʼ������   �½��ش���
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);//PC8

  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}
