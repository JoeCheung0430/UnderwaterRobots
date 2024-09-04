#include "adc.h"
#include "delay.h"
u16 ADC1_Value;

void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);// ADC1 转换结束产生中断，在中断服务程序中读取转换值
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束

//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}			

void ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
//获得ADC值
//ch:通道值 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//设置指定ADC的规则组通道，一个序列，采样时间
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
//}

u16 Get_Adc_Average(u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=ADC_GetConversionValue(ADC1);
	}
	return temp_val/times;
} 	 

float Servo_Detect()			//返回值：舵机支路电流值 长时间：0.8A，短时间：2.5A
{
		float Current;	
		Current = (float)ADC1_Value*(3.3/4096)/0.26;
		return Current;
}

void ADC1_2_IRQHandler(void)
{	
	if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
	{	
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		ADC1_Value = Get_Adc_Average(5);               // 读取ADC的转换值
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}










