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
#define SEND_BUF_SIZE 20	//发送数据长度
u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区




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

			TIM_SetCompare1(TIM1, pwm[0]);//机械爪  注意：不得大于180   开：130，关：180
			TIM_SetCompare1(TIM4, pwm[1]);	//底座				最右：128			最左：210  中值：174
		  
			TIM_SetCompare4(TIM1, pwm[2]);//关节1右			初值：255			往前：158

			
			TIM_SetCompare4(TIM3, pwm[3]);//关节2				最低：222			最高66
			
			TIM_SetCompare3(TIM3, pwm[4]);//旋转关节
			
			TIM_SetCompare2(TIM4, pwm[5]);//关节1左
	}
}



			
