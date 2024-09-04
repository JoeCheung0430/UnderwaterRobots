#ifndef _SYS_H_
#define _SYS_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"

#include "stdbool.h" 
#include "stdio.h"	
#include <stdarg.h>
#include "string.h"	 
#include "math.h"
#include "stdlib.h" 
 
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif 
#ifndef NULL
#define NULL    0
#endif	

typedef enum
{ 
	GPIO_FK_IN     = 0, //浮空输入
	GPIO_AD_IN     = 1, //AD输入

	GPIO_KL_OUT    = 2, //开漏输出 可以读IO输入电平变化，实现C51的IO双向功能，就不需要切换输入输出了,但需要外部上拉电阻
	GPIO_KL_AF_OUT = 3, //开漏复用
	GPIO_TW_OUT    = 4, //推挽输出
	GPIO_TW_AF_OUT = 5, //推挽复用

	GPIO_P_NO      = 6, //不上下拉
	GPIO_P_UP      = 7, //上拉
	GPIO_P_DOWN    = 8, //下拉

	GPIO_2MHz      = 9,
	GPIO_10MHz     =10,
	GPIO_25MHz     =10, //用GPIO_10MHz替代
	GPIO_50MHz     =12,
	GPIO_100MHz    =12  //用GPIO_50MHz替代
}GPIO_My_TypeDef;
void My_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_My_TypeDef mode,GPIO_My_TypeDef up_down,GPIO_My_TypeDef speed);

typedef unsigned long long  u64;

//位带操作 
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 
 
//IO口操作,只对单一的IO口! 确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
 
void WFI_SET(void);		        //执行WFI指令
void WFE_SET(void);		        //执行WFE指令
void NVIC_DISABLE(void);        //关闭所有中断
void NVIC_ENABLE(void);	        //开启所有中断
#define INTX_DISABLE            NVIC_DISABLE
#define INTX_ENABLE             NVIC_ENABLE
void My_Nvdc_Open_Max(s16 open);//用于带freertos时候系统未启用调试带中断的外设 中断被freertos关闭,其他情况注释，防止初始化被中断打断
void SOFT_RESET(void);          //软件复位
void MSR_MSP(u32 addr);	        //设置堆栈地址

u32  Power(u32 x,u32 y);        //指数函数
u32  Log2(u32 x);               //对数函数
void Nop_Function(void);        //空函数

#define _BIN8(a,b,c,d,e,f,g,h) ((a<<7)+(b<<6)+(c<<5)+(d<<4)+(e<<3)+(f<<2)+(g<<1)+(h<<0))
#define _BIN16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) ((a<<15)+(b<<14)+(c<<13)+(d<<12)+(e<<11)+(f<<10)+(g<<9)+(h<<8)+(i<<7)+(j<<6)+(k<<5)+(l<<4)+(m<<3)+(n<<2)+(o<<1)+(p<<0))
#define LongToBin8(n)( \
(n>>21 & 0x80)   | (n>>18 & 0x40)   | (n>>15 & 0x20)   | (n>>12 & 0x10)   | (n>> 9 & 0x08)   | (n>> 6 & 0x04)   | (n>> 3 & 0x02)   | (n     & 0x01))
#define  bin8(n) (u8)LongToBin8(0x##n##l)//替换"0x##n##L"中的"n"，并与"0x"和"L"连接为一体，而符号"0x"和"L"则分别为十六进制数和Long型数的标记符

#define  LongToBin16(n)( \
(n>>45 & 0X8000) | (n>>42 & 0X4000) | (n>>39 & 0X2000) | (n>>36 & 0X1000) | (n>>33 & 0X800)  | (n>>30 & 0X400)  | (n>>27 & 0X200)  | (n>>24 & 0X100)  | \
(n>>21 & 0X80)   | (n>>18 & 0X40)   | (n>>15 & 0X20)   | (n>>12 & 0X10)   | (n>> 9 & 0X08)   | (n>> 6 & 0X04)   | (n>> 3 & 0X02)   | (n     & 0X01) )
#define  bin16(n) (u16)LongToBin16(0x##n##ll)//替换"0x##n##L"中的"n"，并与"0x"和"LL"连接为一体，而符号"0x"和"LL"则分别为十六进制数和LongLong型数的标记符
#define  bin   bin16

#endif
