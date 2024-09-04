#include "sys.h"
#include "system.h"
#include "send_ypr.h"
#include "Balance_PWM.h"
#include "MS5837.h"
#include "DataProtocol.h"

void Work();

int main(void)
{ 
	System_Init();    //系统初始化
	Data_Init();      //参数初始化
	delay_ms(1500);
	while(1)
	{
		Work();        //系统运行工作
	}
}

void Work()
{
	W5500_Socket_Set();                   //W5500端口初始化配置

	if(W5500_Interrupt)                   //处理W5500中断		
	{
		W5500_Interrupt_Process();          //W5500中断处理程序框架
	}
	if((S0_Data & S_RECEIVE) == S_RECEIVE)//如果Socket0接收到数据
	{ 
		S0_Data&=~S_RECEIVE;
		//Process_Socket_Data(0);//此函数用于处理心跳包			
		SetData();                          //接收上位机数据并设置参数	
	}
	
	Control();
	
	if(W5500_Send_Delay_Counter >= 50)    //定时发送字符串
	{
		if(S0_State == (S_INIT|S_CONN))
		{
			S0_Data&=~S_TRANSMITOK;
			
			//Data_Back();                      //发送数据给上位机
		}
		W5500_Send_Delay_Counter=0;
	}
	
//		if(S0_State == (S_INIT|S_CONN))
//		{
//			S0_Data&=~S_TRANSMITOK;
//			
//			Data_Back();//返回数据给上位机
//		}

	Deep_Adjust();
}


//外部中断8 mpu中断引脚  控制平衡 定深
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8) == SET)
	{		
		//printf("EXTI9_5_IRQHandler!\r\n");
		Get_ypr();					                //读取角度
		Horizontal_Control();			        	//计算、控制输出
		EXTI_ClearITPendingBit(EXTI_Line8); //清除LINE8上的中断标志位 
	}
}

//定时读取水深数据  采用定时器1 10ms一次
void TIM1_UP_TIM10_IRQHandler(void)   
{                      
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源   
    {  
				Deep_Get();                                   //读取水深
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);   //清除TIMx的中断待处理位:TIM 中断源   
    }          
}  
