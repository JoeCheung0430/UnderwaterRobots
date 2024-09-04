#include "sys.h"
 
#define  printf_usartx   USART2 //串口几用printf  注意printf没兼容485 DMA模式最好用USARTx_printf

#pragma import(__use_no_semihosting)                    
struct __FILE {int handle;};     //标准库需要的支持函数 
FILE   __stdout;         
//避免使用半主机模式
void _sys_exit(int x){x = x;}
void _ttywrch(int ch){ch = ch;}
int fputc(int ch, FILE *f)       //重定义fputc函数 
{      
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
	USART_SendData(USART1, (u8) ch);	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC)  == RESET);   
	return ch;
}	

//stm32 通用外设初始化方案，方便不同芯片移植
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
	if(speed==GPIO_25MHz)                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//用GPIO_10MHz替代
	if(speed==GPIO_50MHz)                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	if(speed==GPIO_100MHz)                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//用GPIO_50MHz替代  	
 
	GPIO_Init(GPIOx, &GPIO_InitStructure);         	
}
 
//THUMB指令不支持汇编内联,采用如下方法实现执行汇编指令WFI  
//事件可分为中断事件或非中断事件,中断时间产生中断，非中断事件一般是产生标志位置位
void WFI_SET(void)//WFI: wait for Interrupt 等待中断，即下一次中断发生前都在此hold住不干活
{
	__ASM volatile("wfi");		  
}
void WFE_SET(void)//WFE: wait for Events 等待事件，即下一次事件发生前都在此hold住不干活
{
	__ASM volatile("wfe");		  
}
void NVIC_DISABLE(void)//关闭所有中断
{		  
	__ASM volatile("cpsid i");
}
void NVIC_ENABLE(void)//开启所有中断
{
	__ASM volatile("cpsie i");		  
}
void SOFT_RESET(void) 
{  
	__set_FAULTMASK(1);// 关闭所有中断
	NVIC_SystemReset();// 复位
}

//用于带freertos时候系统未启用调试带中断的外设 中断被freertos关闭,其他情况注释，防止初始化被中断打断
void My_Nvdc_Open_Max(s16 open)//-1全开
{
	if(open>=15)open=15;
	if(open<=-1)open=-1;
	__asm
	{
		msr basepri, ( ( (open+1)&0x0f )<<4)//开启中断优先级大于等于(数字小于等于)open的  关闭中断优先级小于(数字大于)open的
		dsb
		isb
	}
}
__asm void MSR_MSP(u32 addr) //设置栈顶地址//addr:栈顶地址
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

u32 Power(u32 x,u32 y){u32 mul = 1,i;for(i = 0;i<y;i++)mul = mul * x;return mul;}
u32 Log2(u32 x)       {u8  i=0;      while(1){if(Power(2,i)==x) return i; else i++;}}
void Nop_Function(void){}

void assert_failed(u8* file, u32 line)
{ 
	printf("断言:[文件:%s] [行:%d]\r\n",file,line);  
}

