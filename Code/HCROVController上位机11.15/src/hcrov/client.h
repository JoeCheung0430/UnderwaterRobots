#pragma once
#include <QTcpSocket>
#include <QTextCodec>
#include <QLocalSocket>
#include <QObject>
#include <iostream>
#include "ui_hcrov.h"
#include "hcrov.h"
#include <deque>

#include <QVector>
#include <QMetaType>
#include <QVariant>

#define Motor          0x01
#define Light          0x02
#define PID            0x03
#define Servo_PWM      0x04  
#define Servo_Current  0x05 
#define MPU_Data       0x06
#define Elc_Dir        0x07
#define Deep_LZQ	   0x08

#define MotorCount      8
#define LightCount      2
#define PidCount        6
#define ServoCount      4
#define ElcAndDirCount  2
#define MpuDataCount    4

#define INT		    0	
#define FLOAT		1	
typedef union                     //�Զ��巢�ͽ�����������
{
	unsigned char  byte[4];
	int            data_int;
	float          data_float;
}DataToByte;                      


class Client :public QObject      //���̳�QWidget�ᵼ���źŲ�connectʧ��
{
	Q_OBJECT

private:
	int forwardThrottle, transverseThrottle, verticalThorttle;
	int lightPwm = 300, ptz, steering = 0;
	int throttle = 0;

	int speed_gear_h = 0;     //ˮƽ�ٶȵ�  ��5�� 0-5
	int speed_gear_v = 1;     //��ֱ�ٶȵ�  ��5�� 0-5

	int light_gear = 1;       //��ƹ⵵    ��4�� 0-4 
	int light_flag = 0;       //��ƿ��ر�־λ    1����  0����
	int small_light_flag = 0; //С�ƿ��ر�־λ    1����  0����

	int left_flag = 1;		  //ˮƽ ��������ת��־  1����ת ��-1����ת  0����ת
	int right_flag = 1;		  //ˮƽ �ҵ������ת��־  1����ת ��-1����ת  0����ת
	int up_down_flag = 0;     //ˮƽ �ҵ������ת��־  1����ת ��-1����ת  0����ת
	int grap_flag = 1;


	float pitch = 0, roll = 0, yaw = 0;    //�����ǲ���
	float direction = 0;                   //����
	float temp = 0;                        //temperature  �¶�
	float depth = 0;                       //���
	float electric = 0;                    //��ѹ

	DataToByte MotorPWM[MotorCount];     //0-3 ˮƽ��� 4-7 ��ֱ���
	DataToByte LightPWM[LightCount];     //0 ��� 1 С��
	DataToByte PidData[PidCount];        //bp bd dp dd
	DataToByte ServoPWM[ServoCount];     //���pwm  4��
	DataToByte ServoCurrent[ServoCount]; //�������  4��
	DataToByte ElcAndDir[ElcAndDirCount]; //��ص�ѹ�ͷ��� 0����ص�ѹ ��1������
	DataToByte Deep_lzq[1] = {0};

	int sensorData[7];
	std::deque <QByteArray> dataList;

private:
	void CheckData(QByteArray data);
	void FixData();
	void SendData();

public:
	void init();
	Client();
	void Send_TO_W5500_Data(unsigned char data_type, unsigned char data_category, int data_size, DataToByte data[]);

signals:
	void appendMsgToTxtBox(QString msg);
	void postureData(char order, float value);
	void changeSlidervalue(char order, int value);

private slots:
	void Startconnect(QString addr, QString port);
	void StateChanged(int state);
	void Read_Data();
	void SendDataToHost();
	void keyControl(QKeyEvent* e);
	void GetGearValue(char order, int gear);
	void ReceiveParameter(QVariant pwm, QVariant pid);

private:
	//QTextCodec* gbk = QTextCodec::codecForName("GBK");
	QTcpSocket* clientSocket;
	//std::mutex mux;
	QByteArray controlData = { 0 };
};

extern Client* client;


