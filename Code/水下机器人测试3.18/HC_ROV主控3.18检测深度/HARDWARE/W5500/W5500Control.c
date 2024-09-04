#include "W5500Control.h"	
#include "delay.h"

int ifconnect=0;
unsigned int W5500_Send_Delay_Counter=0; //W5500发送延时计数变量(ms)
unsigned int Timer2_Counter=0; //Timer2定时器计数变量(ms)

/*******************************************************************************
* 函数名  : W5500Control_Init
* 描述    : W5500启动
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void W5500Control_Init(void)
{
	//printf("W5500Control_Init \r\n"); 
	SPI_Configuration();		    //W5500 SPI初始化配置(STM32 SPI1)
	W5500_GPIO_Configuration();	//W5500 GPIO初始化配置	
	Load_Net_Parameters();		  //装载网络参数	
	W5500_Hardware_Reset();		  //硬件复位W5500
	W5500_Initialization();		  //W5500初始化配置
}

/*******************************************************************************
* 函数名  : W5500_Initialization
* 描述    : W5500初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void W5500_Initialization(void)
{
	W5500_Init();		//初始化W5500寄存器函数
	Detect_Gateway();	//检查网关服务器 
	Socket_Init(0);		//指定Socket(0~7)初始化,初始化端口0
}

/*******************************************************************************
* 函数名  : Load_Net_Parameters
* 描述    : 装载网络参数
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 网关、掩码、物理地址、本机IP地址、端口号、目的IP地址、目的端口号、端口工作模式
*******************************************************************************/
void Load_Net_Parameters(void)
{
	Gateway_IP[0] = 192;//加载网关参数
	Gateway_IP[1] = 168;
	Gateway_IP[2] = 3;
	Gateway_IP[3] = 1;

	Sub_Mask[0]=255;//加载子网掩码
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;

	Phy_Addr[0]=0x0c;//加载物理地址
	Phy_Addr[1]=0x29;
	Phy_Addr[2]=0xab;
	Phy_Addr[3]=0x7c;
	Phy_Addr[4]=0x00;
	Phy_Addr[5]=0x01;

	IP_Addr[0]=192;//加载本机IP地址
	IP_Addr[1]=168;
	IP_Addr[2]=3;
	IP_Addr[3]=199;

	S0_Port[0] = 0x13;//加载端口0的端口号5000 
	S0_Port[1] = 0x88;

//	S0_DIP[0]=192;//加载端口0的目的IP地址
//	S0_DIP[1]=168;
//	S0_DIP[2]=47;
//	S0_DIP[3]=1;
//	
//	S0_DPort[0] = 0x17;//加载端口0的目的端口号6000
//	S0_DPort[1] = 0x70;

	S0_Mode=TCP_SERVER;//加载端口0的工作模式,TCP服务端模式
}

/*******************************************************************************
* 函数名  : W5500_Socket_Set
* 描述    : W5500端口初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 分别设置4个端口,根据端口工作模式,将端口置于TCP服务器、TCP客户端或UDP模式.
*			从端口状态字节Socket_State可以判断端口的工作情况
*******************************************************************************/
void W5500_Socket_Set(void)
{
	if(S0_State==0)//端口0初始化配置
	{
		if(S0_Mode==TCP_SERVER)//TCP服务器模式 
		{
			if(Socket_Listen(0)==TRUE)
			{
				S0_State=S_INIT;
			}
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP客户端模式 
		{
			if(Socket_Connect(0)==TRUE)
			{
				S0_State=S_INIT;
			}
			else
				S0_State=0;
		}
		else//UDP模式 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}

/*******************************************************************************
* 函数名  : Process_Socket_Data
* 描述    : W5500接收并发送接收到的数据
* 输入    : s:端口号
* 输出    : 无
* 返回值  : 无
* 说明    : 本过程先调用S_rx_process()从W5500的端口接收数据缓冲区读取数据,
*			然后将读取的数据从Rx_Buffer拷贝到Temp_Buffer缓冲区进行处理。
*			处理完毕，将数据从Temp_Buffer拷贝到Tx_Buffer缓冲区。调用S_tx_process()
*			发送数据。
*******************************************************************************/
void Process_Socket_Data(SOCKET s)
{
  //此函数用于处理心跳包
}

/*******************************************************************************
* 函数名  : Delay
* 描述    : 延时函数(ms)
* 输入    : d:延时系数，单位为毫秒
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Delay(unsigned int d)
{
//	Timer2_Counter=0; 
//	while(Timer2_Counter < d);
	delay_ms(d);
}

/*******************************************************************************
* 函数名  : W5500_Send_Data
* 描述    : 通过W5500发送数据给上位机
* 输入    :data[] 数据;
					 data_type 数据类型 0:int、1:float；
					 data_category 数据类别：
																	1:
																	2:
																	3:
																	4:
																	5:
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void W5500_Send_Data(unsigned char data_type,unsigned char data_category,int data_size,DataToByte data[])
{
	
	int i,j,k;	
	Tx_Buffer[0]=data_type;
	Tx_Buffer[1]=data_category;
	Tx_Buffer[2]=data_size;
	Tx_Buffer[3]=0x04;
	
	for(i =0;i<data_size;i++)
	{
		for(j =(i+1)*4,k=0 ;k<4 ;j++,k++)
		{
			Tx_Buffer[j]=data[i].byte[k];
		}
	}						
	Write_SOCK_Data_Buffer(0, Tx_Buffer, 4*(data_size+1) );
	
}
