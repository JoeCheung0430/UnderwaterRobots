#include "sys.h"
#include "system.h"
#include "send_ypr.h"
#include "Balance_PWM.h"
#include "MS5837.h"
#include "DataProtocol.h"

void Work();

int main(void)
{ 
	System_Init();    //ϵͳ��ʼ��
	Data_Init();      //������ʼ��
	delay_ms(1500);
	while(1)
	{
		Work();        //ϵͳ���й���
	}
}

void Work()
{
	W5500_Socket_Set();                   //W5500�˿ڳ�ʼ������

	if(W5500_Interrupt)                   //����W5500�ж�		
	{
		W5500_Interrupt_Process();          //W5500�жϴ��������
	}
	if((S0_Data & S_RECEIVE) == S_RECEIVE)//���Socket0���յ�����
	{ 
		S0_Data&=~S_RECEIVE;
		//Process_Socket_Data(0);//�˺������ڴ���������			
		SetData();                          //������λ�����ݲ����ò���	
	}
	
	Control();
	
	if(W5500_Send_Delay_Counter >= 50)    //��ʱ�����ַ���
	{
		if(S0_State == (S_INIT|S_CONN))
		{
			S0_Data&=~S_TRANSMITOK;
			
			//Data_Back();                      //�������ݸ���λ��
		}
		W5500_Send_Delay_Counter=0;
	}
	
//		if(S0_State == (S_INIT|S_CONN))
//		{
//			S0_Data&=~S_TRANSMITOK;
//			
//			Data_Back();//�������ݸ���λ��
//		}

	Deep_Adjust();
}


//�ⲿ�ж�8 mpu�ж�����  ����ƽ�� ����
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8) == SET)
	{		
		//printf("EXTI9_5_IRQHandler!\r\n");
		Get_ypr();					                //��ȡ�Ƕ�
		Horizontal_Control();			        	//���㡢�������
		EXTI_ClearITPendingBit(EXTI_Line8); //���LINE8�ϵ��жϱ�־λ 
	}
}

//��ʱ��ȡˮ������  ���ö�ʱ��1 10msһ��
void TIM1_UP_TIM10_IRQHandler(void)   
{                      
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ   
    {  
				Deep_Get();                                   //��ȡˮ��
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);   //���TIMx���жϴ�����λ:TIM �ж�Դ   
    }          
}  
