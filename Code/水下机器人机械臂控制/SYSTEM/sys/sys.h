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
	GPIO_FK_IN     = 0, //��������
	GPIO_AD_IN     = 1, //AD����

	GPIO_KL_OUT    = 2, //��©��� ���Զ�IO�����ƽ�仯��ʵ��C51��IO˫���ܣ��Ͳ���Ҫ�л����������,����Ҫ�ⲿ��������
	GPIO_KL_AF_OUT = 3, //��©����
	GPIO_TW_OUT    = 4, //�������
	GPIO_TW_AF_OUT = 5, //���츴��

	GPIO_P_NO      = 6, //��������
	GPIO_P_UP      = 7, //����
	GPIO_P_DOWN    = 8, //����

	GPIO_2MHz      = 9,
	GPIO_10MHz     =10,
	GPIO_25MHz     =10, //��GPIO_10MHz���
	GPIO_50MHz     =12,
	GPIO_100MHz    =12  //��GPIO_50MHz���
}GPIO_My_TypeDef;
void My_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_My_TypeDef mode,GPIO_My_TypeDef up_down,GPIO_My_TypeDef speed);

typedef unsigned long long  u64;

//λ������ 
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��! ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
 
void WFI_SET(void);		        //ִ��WFIָ��
void WFE_SET(void);		        //ִ��WFEָ��
void NVIC_DISABLE(void);        //�ر������ж�
void NVIC_ENABLE(void);	        //���������ж�
#define INTX_DISABLE            NVIC_DISABLE
#define INTX_ENABLE             NVIC_ENABLE
void My_Nvdc_Open_Max(s16 open);//���ڴ�freertosʱ��ϵͳδ���õ��Դ��жϵ����� �жϱ�freertos�ر�,�������ע�ͣ���ֹ��ʼ�����жϴ��
void SOFT_RESET(void);          //�����λ
void MSR_MSP(u32 addr);	        //���ö�ջ��ַ

u32  Power(u32 x,u32 y);        //ָ������
u32  Log2(u32 x);               //��������
void Nop_Function(void);        //�պ���

#define _BIN8(a,b,c,d,e,f,g,h) ((a<<7)+(b<<6)+(c<<5)+(d<<4)+(e<<3)+(f<<2)+(g<<1)+(h<<0))
#define _BIN16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) ((a<<15)+(b<<14)+(c<<13)+(d<<12)+(e<<11)+(f<<10)+(g<<9)+(h<<8)+(i<<7)+(j<<6)+(k<<5)+(l<<4)+(m<<3)+(n<<2)+(o<<1)+(p<<0))
#define LongToBin8(n)( \
(n>>21 & 0x80)   | (n>>18 & 0x40)   | (n>>15 & 0x20)   | (n>>12 & 0x10)   | (n>> 9 & 0x08)   | (n>> 6 & 0x04)   | (n>> 3 & 0x02)   | (n     & 0x01))
#define  bin8(n) (u8)LongToBin8(0x##n##l)//�滻"0x##n##L"�е�"n"������"0x"��"L"����Ϊһ�壬������"0x"��"L"��ֱ�Ϊʮ����������Long�����ı�Ƿ�

#define  LongToBin16(n)( \
(n>>45 & 0X8000) | (n>>42 & 0X4000) | (n>>39 & 0X2000) | (n>>36 & 0X1000) | (n>>33 & 0X800)  | (n>>30 & 0X400)  | (n>>27 & 0X200)  | (n>>24 & 0X100)  | \
(n>>21 & 0X80)   | (n>>18 & 0X40)   | (n>>15 & 0X20)   | (n>>12 & 0X10)   | (n>> 9 & 0X08)   | (n>> 6 & 0X04)   | (n>> 3 & 0X02)   | (n     & 0X01) )
#define  bin16(n) (u16)LongToBin16(0x##n##ll)//�滻"0x##n##L"�е�"n"������"0x"��"LL"����Ϊһ�壬������"0x"��"LL"��ֱ�Ϊʮ����������LongLong�����ı�Ƿ�
#define  bin   bin16

#endif
