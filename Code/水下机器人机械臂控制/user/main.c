#include "stm32f10x.h"
#include "delay.h"
#include <usart.h>
#include "timer.h"
#include "adc.h"
#include "MOTOR_P_N.h"
#include <stm32F10x.h>


#define PWM   150
#define PWMMAX 2000
#define PWMMIN 1000

int flag=1;
//int pwm[6]={180,174,255,222,0,0};
int pwm[6]={0,0,0,0,0,0};
u16 len;
extern u16 ADC1_Value;
#define SEND_BUF_SIZE 20	//�������ݳ���
u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����




int main(void)
{
	int aaa=500;
	int i=0;
	delay_init();
	TIM1_PWM_Init(2000-1,72-1);	
	USART3_Init(115200);
	TIM3_PWM_Init(2000-1,72-1);
	TIM4_PWM_Init(2000-1,72-1);	
	while(1)
	{

			TIM_SetCompare1(TIM1, pwm[0]);//��еצ  ע�⣺���ô���180   ����130���أ�180
			TIM_SetCompare1(TIM4, pwm[1]);	//����				���ң�128			����210  ��ֵ��174
		  
			TIM_SetCompare4(TIM1, pwm[2]);//�ؽ�1��			��ֵ��255			��ǰ��158

			
			TIM_SetCompare4(TIM3, pwm[3]);//�ؽ�2				��ͣ�222			���66
			
			TIM_SetCompare3(TIM3, pwm[4]);//��ת�ؽ�
			
			TIM_SetCompare2(TIM4, pwm[5]);//�ؽ�1��
	}
}



			
