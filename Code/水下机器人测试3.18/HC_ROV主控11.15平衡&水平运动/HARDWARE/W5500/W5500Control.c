#include "W5500Control.h"	
#include "delay.h"

int ifconnect=0;
unsigned int W5500_Send_Delay_Counter=0; //W5500������ʱ��������(ms)
unsigned int Timer2_Counter=0; //Timer2��ʱ����������(ms)

/*******************************************************************************
* ������  : W5500Control_Init
* ����    : W5500����
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500Control_Init(void)
{
	//printf("W5500Control_Init \r\n"); 
	SPI_Configuration();		    //W5500 SPI��ʼ������(STM32 SPI1)
	W5500_GPIO_Configuration();	//W5500 GPIO��ʼ������	
	Load_Net_Parameters();		  //װ���������	
	W5500_Hardware_Reset();		  //Ӳ����λW5500
	W5500_Initialization();		  //W5500��ʼ������
}

/*******************************************************************************
* ������  : W5500_Initialization
* ����    : W5500��ʼ������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_Initialization(void)
{
	W5500_Init();		//��ʼ��W5500�Ĵ�������
	Detect_Gateway();	//������ط����� 
	Socket_Init(0);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
}

/*******************************************************************************
* ������  : Load_Net_Parameters
* ����    : װ���������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ���ء����롢�����ַ������IP��ַ���˿ںš�Ŀ��IP��ַ��Ŀ�Ķ˿ںš��˿ڹ���ģʽ
*******************************************************************************/
void Load_Net_Parameters(void)
{
	Gateway_IP[0] = 192;//�������ز���
	Gateway_IP[1] = 168;
	Gateway_IP[2] = 3;
	Gateway_IP[3] = 1;

	Sub_Mask[0]=255;//������������
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;

	Phy_Addr[0]=0x0c;//���������ַ
	Phy_Addr[1]=0x29;
	Phy_Addr[2]=0xab;
	Phy_Addr[3]=0x7c;
	Phy_Addr[4]=0x00;
	Phy_Addr[5]=0x01;

	IP_Addr[0]=192;//���ر���IP��ַ
	IP_Addr[1]=168;
	IP_Addr[2]=3;
	IP_Addr[3]=199;

	S0_Port[0] = 0x13;//���ض˿�0�Ķ˿ں�5000 
	S0_Port[1] = 0x88;

//	S0_DIP[0]=192;//���ض˿�0��Ŀ��IP��ַ
//	S0_DIP[1]=168;
//	S0_DIP[2]=47;
//	S0_DIP[3]=1;
//	
//	S0_DPort[0] = 0x17;//���ض˿�0��Ŀ�Ķ˿ں�6000
//	S0_DPort[1] = 0x70;

	S0_Mode=TCP_SERVER;//���ض˿�0�Ĺ���ģʽ,TCP�����ģʽ
}

/*******************************************************************************
* ������  : W5500_Socket_Set
* ����    : W5500�˿ڳ�ʼ������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : �ֱ�����4���˿�,���ݶ˿ڹ���ģʽ,���˿�����TCP��������TCP�ͻ��˻�UDPģʽ.
*			�Ӷ˿�״̬�ֽ�Socket_State�����ж϶˿ڵĹ������
*******************************************************************************/
void W5500_Socket_Set(void)
{
	if(S0_State==0)//�˿�0��ʼ������
	{
		if(S0_Mode==TCP_SERVER)//TCP������ģʽ 
		{
			if(Socket_Listen(0)==TRUE)
			{
				S0_State=S_INIT;
			}
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP�ͻ���ģʽ 
		{
			if(Socket_Connect(0)==TRUE)
			{
				S0_State=S_INIT;
			}
			else
				S0_State=0;
		}
		else//UDPģʽ 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}

/*******************************************************************************
* ������  : Process_Socket_Data
* ����    : W5500���ղ����ͽ��յ�������
* ����    : s:�˿ں�
* ���    : ��
* ����ֵ  : ��
* ˵��    : �������ȵ���S_rx_process()��W5500�Ķ˿ڽ������ݻ�������ȡ����,
*			Ȼ�󽫶�ȡ�����ݴ�Rx_Buffer������Temp_Buffer���������д���
*			������ϣ������ݴ�Temp_Buffer������Tx_Buffer������������S_tx_process()
*			�������ݡ�
*******************************************************************************/
void Process_Socket_Data(SOCKET s)
{
  //�˺������ڴ���������
}

/*******************************************************************************
* ������  : Delay
* ����    : ��ʱ����(ms)
* ����    : d:��ʱϵ������λΪ����
* ���    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void Delay(unsigned int d)
{
//	Timer2_Counter=0; 
//	while(Timer2_Counter < d);
	delay_ms(d);
}

/*******************************************************************************
* ������  : W5500_Send_Data
* ����    : ͨ��W5500�������ݸ���λ��
* ����    :data[] ����;
					 data_type �������� 0:int��1:float��
					 data_category �������
																	1:
																	2:
																	3:
																	4:
																	5:
* ���    : ��
* ����    : �� 
* ˵��    : 
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
