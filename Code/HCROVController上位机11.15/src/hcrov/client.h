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
typedef union                     //自定义发送接收数据类型
{
	unsigned char  byte[4];
	int            data_int;
	float          data_float;
}DataToByte;                      


class Client :public QObject      //不继承QWidget会导致信号槽connect失败
{
	Q_OBJECT

private:
	int forwardThrottle, transverseThrottle, verticalThorttle;
	int lightPwm = 300, ptz, steering = 0;
	int throttle = 0;

	int speed_gear_h = 0;     //水平速度档  共5档 0-5
	int speed_gear_v = 1;     //垂直速度档  共5档 0-5

	int light_gear = 1;       //大灯光档    共4档 0-4 
	int light_flag = 0;       //大灯开关标志位    1：开  0；关
	int small_light_flag = 0; //小灯开关标志位    1：开  0；关

	int left_flag = 1;		  //水平 左电机正反转标志  1：正转 、-1：反转  0；不转
	int right_flag = 1;		  //水平 右电机正反转标志  1：正转 、-1：反转  0；不转
	int up_down_flag = 0;     //水平 右电机正反转标志  1：正转 、-1：反转  0；不转
	int grap_flag = 1;


	float pitch = 0, roll = 0, yaw = 0;    //陀螺仪参数
	float direction = 0;                   //方向
	float temp = 0;                        //temperature  温度
	float depth = 0;                       //深度
	float electric = 0;                    //电压

	DataToByte MotorPWM[MotorCount];     //0-3 水平电机 4-7 垂直电机
	DataToByte LightPWM[LightCount];     //0 大灯 1 小灯
	DataToByte PidData[PidCount];        //bp bd dp dd
	DataToByte ServoPWM[ServoCount];     //舵机pwm  4个
	DataToByte ServoCurrent[ServoCount]; //舵机电流  4个
	DataToByte ElcAndDir[ElcAndDirCount]; //电池电压和方向 0：电池电压 、1：方向
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


