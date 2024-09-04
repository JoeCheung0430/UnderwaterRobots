#include "sys.h"
 
#define  printf_usartx   USART2 //���ڼ���printf  ע��printfû����485 DMAģʽ�����USARTx_printf

#pragma import(__use_no_semihosting)                    
struct __FILE {int handle;};     //��׼����Ҫ��֧�ֺ��� 
FILE   __stdout;         
//����ʹ�ð�����ģʽ
void _sys_exit(int x){x = x;}
void _ttywrch(int ch){ch = ch;}
int fputc(int ch, FILE *f)       //�ض���fputc���� 
{      
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
	USART_SendData(USART1, (u8) ch);	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC)  == RESET);   
	return ch;
}	

//stm32 ͨ�������ʼ�����������㲻ͬоƬ��ֲ
void My_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_My_TypeDef mode,GPIO_My_TypeDef up_down,GPIO_My_TypeDef speed)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
   
	RCC_APB2PeriphClockCmd(Power(2,((uint32_t)GPIOx-(uint32_t)GPIOA)/0x0400+2), ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;        
 
	if(mode==GPIO_AD_IN)                         GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN; 
	if(mode==GPIO_FK_IN && up_down==GPIO_P_NO)   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; 
	if(mode==GPIO_FK_IN && up_down==GPIO_P_DOWN) GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;  
	if(mode==GPIO_FK_IN && up_down==GPIO_P_UP)   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;  
	if(mode==GPIO_KL_OUT)                        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
	if(mode==GPIO_TW_OUT)                        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	if(mode==GPIO_KL_AF_OUT)                     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
	if(mode==GPIO_TW_AF_OUT)                     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;

 	if(speed==GPIO_2MHz)                         GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	if(speed==GPIO_10MHz)                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	if(speed==GPIO_25MHz)                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//��GPIO_10MHz���
	if(speed==GPIO_50MHz)                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	if(speed==GPIO_100MHz)                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//��GPIO_50MHz���  	
 
	GPIO_Init(GPIOx, &GPIO_InitStructure);         	
}
 
//THUMBָ�֧�ֻ������,�������·���ʵ��ִ�л��ָ��WFI  
//�¼��ɷ�Ϊ�ж��¼�����ж��¼�,�ж�ʱ������жϣ����ж��¼�һ���ǲ�����־λ��λ
void WFI_SET(void)//WFI: wait for Interrupt �ȴ��жϣ�����һ���жϷ���ǰ���ڴ�holdס���ɻ�
{
	__ASM volatile("wfi");		  
}
void WFE_SET(void)//WFE: wait for Events �ȴ��¼�������һ���¼�����ǰ���ڴ�holdס���ɻ�
{
	__ASM volatile("wfe");		  
}
void NVIC_DISABLE(void)//�ر������ж�
{		  
	__ASM volatile("cpsid i");
}
void NVIC_ENABLE(void)//���������ж�
{
	__ASM volatile("cpsie i");		  
}
void SOFT_RESET(void) 
{  
	__set_FAULTMASK(1);// �ر������ж�
	NVIC_SystemReset();// ��λ
}

//���ڴ�freertosʱ��ϵͳδ���õ��Դ��жϵ����� �жϱ�freertos�ر�,�������ע�ͣ���ֹ��ʼ�����жϴ��
void My_Nvdc_Open_Max(s16 open)//-1ȫ��
{
	if(open>=15)open=15;
	if(open<=-1)open=-1;
	__asm
	{
		msr basepri, ( ( (open+1)&0x0f )<<4)//�����ж����ȼ����ڵ���(����С�ڵ���)open��  �ر��ж����ȼ�С��(���ִ���)open��
		dsb
		isb
	}
}
__asm void MSR_MSP(u32 addr) //����ջ����ַ//addr:ջ����ַ
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

u32 Power(u32 x,u32 y){u32 mul = 1,i;for(i = 0;i<y;i++)mul = mul * x;return mul;}
u32 Log2(u32 x)       {u8  i=0;      while(1){if(Power(2,i)==x) return i; else i++;}}
void Nop_Function(void){}

void assert_failed(u8* file, u32 line)
{ 
	printf("����:[�ļ�:%s] [��:%d]\r\n",file,line);  
}

