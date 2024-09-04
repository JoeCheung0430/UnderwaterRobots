//#include "usart.h"	  

////����2��������
//#define Max_BUFF_Len 8     //һ֡����8�ֽ�
//unsigned char Uart2_Buffer[Max_BUFF_Len];
//unsigned int Uart2_Rx=0;

//extern int pwm[6];

////USART2_TX   GPIOA.2
////USART2_RX	  GPIOA.3

//u8  USART2_TX_BUF[USART2_MAX_TX_LEN]; 		//���ͻ���,���USART2_MAX_TX_LEN-1�ֽ�
//u8  USART2_RX_BUF[USART2_MAX_RX_LEN]; 		//���ջ���,���USART2_MAX_RX_LEN-1�ֽ�
//volatile u16 USART2_RX_STA=0;               //bit15:������ɱ�־   bit14~0:���յ�����Ч�ֽ���Ŀ 

//void USART2_Init(u32 bound)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	//ʱ������
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
// 
//	//USARTx_TX 
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//USARTx_RX 
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);  
//	
//	#if USART2_RS485==1
//	My_GPIO_Init(USART2_RS485_TX_EN_GPIOx,USART2_RS485_TX_EN_Pin,GPIO_TW_OUT,GPIO_P_DOWN,GPIO_100MHz);//������� ���� 100m
//	USART2_RS485_TX_EN=0;
//	#endif
//	
//	//NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0; 
//	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;		 
//	NVIC_Init(&NVIC_InitStructure);	 
//	
//	 //USARTx��ʼ�� 
//	USART_InitStructure.USART_BaudRate            = bound; 
//	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 
//	USART_InitStructure.USART_StopBits            = USART_StopBits_1; 
//	USART_InitStructure.USART_Parity              = USART_Parity_No; 
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
//	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; 

//	//ʹ������
//	USART_Init(USART2, &USART_InitStructure);     //��ʼ������
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�������ڿ���IDEL�ж�
//	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
//	USART2_printf("USART2 OK...\r\n");
//}

//void USART2_printf (char *fmt, ...)
//{
//	u16 i = 0;
//	va_list arg_ptr;
//	va_start(arg_ptr, fmt); 
//	vsnprintf((char *)USART2_TX_BUF, USART2_MAX_TX_LEN+1, fmt, arg_ptr);
//	va_end(arg_ptr);
//	
//	#if USART2_RS485==1
//	USART2_RS485_TX_EN=1;
//	#endif
//	
//	while ((i < USART2_MAX_TX_LEN) && USART2_TX_BUF[i])
//	{
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
//		USART_SendData(USART2, (u8) USART2_TX_BUF[i++]);
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC)  == RESET);
//	}
//	
//	#if USART2_RS485==1
//	USART2_RS485_TX_EN=0;
//	#endif
//}
//  
//void USART2_Send_Array (u8 *array,u16 num)
//{
//	u16 i=0;
//	
//	#if USART2_RS485==1
//	USART2_RS485_TX_EN=1;
//	#endif
//	
//	while (i < num)
//	{
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
//		USART_SendData(USART2, (u8) array[i++]);
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC)  == RESET);
//	}
//	
//	#if USART2_RS485==1
//	USART2_RS485_TX_EN=0;
//	#endif
//}

//void USART2_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)               // �����ж�
//	{
//		USART_ReceiveData(USART2);                                      //��������жϱ�־
//		USART2_RX_BUF[USART2_RX_STA&0X7FFF]='\0';		                //���\0,��ֹ�ַ���������������\0һֱ������
//		USART2_RX_STA|=0x8000;	                                        //��ǽ��������
//		//��Ӵ�����,�����������ѯ����

//	}	
//	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)               // ��� ORE ��־,��ֹ�������ж����������ڽ����ж�ǰ��
//	{
//		USART_ClearFlag(USART2,USART_FLAG_ORE);
//		USART_ReceiveData(USART2);
//	}
//	
//	Uart2_Buffer[Uart2_Rx] = USART_ReceiveData(USART2);     //���մ���1���ݵ�buff������
//		Uart2_Rx++; 
//	if(Uart2_Buffer[Uart2_Rx-1] == 0x0a || Uart2_Rx == Max_BUFF_Len)    //������յ�β��ʶ�ǻ��з������ߵ�������������������½��գ�
//		{
//			switch(Uart2_Buffer[0])
//			{
//				case Servo_head:
//					if(Uart2_Buffer[1]>=0&&Uart2_Buffer[1]<8)
//					{
//						pwm[Uart2_Buffer[1]]=(Uart2_Buffer[2])|(Uart2_Buffer[3]<<8)|(Uart2_Buffer[4]<<16)|(Uart2_Buffer[5]<<24);						
//						
//					}
//					Uart2_Rx=0; 						
////					break;
//				default:
//							Uart2_Rx=0; 
//					break;
//			}
//		}
//	//overflow_deal();
//}
//void overflow_deal()
//{
//	if(pwm[0]!=pwm0_min && pwm[0]!=pwm0_max)	pwm[0]=0;
//	if(pwm[1]<pwm1_min)		pwm[1]=pwm1_min;
//	if(pwm[1]>pwm1_max)		pwm[1]=pwm1_max;
//	if(pwm[2]<pwm2_min)		pwm[2]=pwm2_min;
//	if(pwm[2]>pwm2_max)		pwm[2]=pwm2_max;
//	if(pwm[3]<pwm2_min)		pwm[3]=pwm3_min;
//	if(pwm[3]>pwm2_max)		pwm[3]=pwm3_max;
//}

#include "usart.h"	  

//USART3_TX   GPIOB.10
//USART3_RX	  GPIOB.11

#define Max_BUFF_Len 8     //һ֡����8�ֽ�
unsigned char Uart3_Buffer[Max_BUFF_Len];
unsigned int Uart3_Rx=0;

extern int pwm[6];

u8  USART3_TX_BUF[USART3_MAX_TX_LEN]; 		//���ͻ���,���USART3_MAX_TX_LEN-1�ֽ�
u8  USART3_RX_BUF[USART3_MAX_RX_LEN]; 		//���ջ���,���USART3_MAX_RX_LEN-1�ֽ�
volatile u16 USART3_RX_STA=0;               //bit15:������ɱ�־   bit14~0:���յ�����Ч�ֽ���Ŀ 

void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//ʱ������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
 
	//USARTx_TX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USARTx_RX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
	
	#if USART3_RS485==1
	My_GPIO_Init(USART3_RS485_TX_EN_GPIOx,USART3_RS485_TX_EN_Pin,GPIO_TW_OUT,GPIO_P_DOWN,GPIO_100MHz);//������� ���� 100m
	USART3_RS485_TX_EN=0;
	#endif
	
	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;		 
	NVIC_Init(&NVIC_InitStructure);	 
	
	 //USARTx��ʼ�� 
	USART_InitStructure.USART_BaudRate            = bound; 
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits            = USART_StopBits_1; 
	USART_InitStructure.USART_Parity              = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; 

	//ʹ������
	USART_Init(USART3, &USART_InitStructure);     //��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//�������ڿ���IDEL�ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
	USART3_printf("USART3 OK...\r\n");
}

void USART3_printf (char *fmt, ...)
{
	u16 i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt); 
	vsnprintf((char *)USART3_TX_BUF, USART3_MAX_TX_LEN+1, fmt, arg_ptr);
	va_end(arg_ptr);
	
	#if USART3_RS485==1
	USART3_RS485_TX_EN=1;
	#endif
	
	while ((i < USART3_MAX_TX_LEN) && USART3_TX_BUF[i])
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
		USART_SendData(USART3, (u8) USART3_TX_BUF[i++]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC)  == RESET);
	}
	
	#if USART3_RS485==1
	USART3_RS485_TX_EN=0;
	#endif
}
  
void USART3_Send_Array (u8 *array,u16 num)
{
	u16 i=0;
	
	#if USART3_RS485==1
	USART3_RS485_TX_EN=1;
	#endif
	
	while (i < num)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
		USART_SendData(USART3, (u8) array[i++]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC)  == RESET);
	}
	
	#if USART3_RS485==1
	USART3_RS485_TX_EN=0;
	#endif
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)               // �����ж�
	{
		USART_ReceiveData(USART3);                                      //��������жϱ�־
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]='\0';		                //���\0,��ֹ�ַ���������������\0һֱ������
		USART3_RX_STA|=0x8000;	                                        //��ǽ��������
		//��Ӵ�����,�����������ѯ����

	}	
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)               // ��� ORE ��־,��ֹ�������ж����������ڽ����ж�ǰ��
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);
		USART_ReceiveData(USART3);
	}
	Uart3_Buffer[Uart3_Rx] = USART_ReceiveData(USART3);     //���մ���1���ݵ�buff������
		Uart3_Rx++; 
	if(Uart3_Buffer[Uart3_Rx-1] == 0x0a || Uart3_Rx == Max_BUFF_Len)    //������յ�β��ʶ�ǻ��з������ߵ�������������������½��գ�
		{
			switch(Uart3_Buffer[0])
			{
				case Servo_head:
					if(Uart3_Buffer[1]>=0&&Uart3_Buffer[1]<8)
					{
						pwm[Uart3_Buffer[1]]=(Uart3_Buffer[2])|(Uart3_Buffer[3]<<8)|(Uart3_Buffer[4]<<16)|(Uart3_Buffer[5]<<24);						
						
					}
					Uart3_Rx=0; 						
//					break;
				default:
							Uart3_Rx=0; 
					break;
			}
		}
}
 