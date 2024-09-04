#include "client.h"
#include <iostream>

Client::Client()
{
	ServoPWM[0].data_int = 180;
	ServoPWM[1].data_int = 174;
	ServoPWM[2].data_int = 255;
	ServoPWM[3].data_int = 222;
}

void Client::init()
{
	//分配空间并指定父类
	clientSocket = new QTcpSocket(this);
	connect(HCROV, SIGNAL(getAddr(QString, QString)), this, SLOT(Startconnect(QString, QString)));
	//connect(this->clientSocket, &QTcpSocket::stateChanged, this, &Client::StateChanged);
	connect(this->clientSocket, &QTcpSocket::readyRead,    this, &Client::Read_Data);
	//connect(HCROV, SIGNAL(sendMsgToHost(QByteArray)), this, SLOT(SendDataToHost(QByteArray)));
	connect(HCROV, SIGNAL(getGear(char, int)), this, SLOT(GetGearValue(char, int)));
	connect(HCROV, SIGNAL(getKey(QKeyEvent*)), this, SLOT(keyControl(QKeyEvent*)));
}

void Client::Startconnect(QString addr, QString port)
{
	if (addr != "0" && port != "0")
	{
		clientSocket->connectToHost(addr, quint16(port.toUInt()));

		if (!clientSocket->waitForConnected(5000))
		{
			QString str = QString::fromLocal8Bit("连接失败！") + addr + ":" + port+"\n";
			emit appendMsgToTxtBox(str);
			return;
		}
		else
		{
			QString str = QString::fromLocal8Bit("已连接至服务器") + addr + ":" + port + "\n";
			emit appendMsgToTxtBox(str);
		}
	}
	else
	{
		if (clientSocket->state() == QAbstractSocket::ConnectedState)
		{
			clientSocket->disconnectFromHost();
			QString str = QString::fromLocal8Bit("已断开至服务器");
			emit appendMsgToTxtBox(str);
		}
	}
}

//接收参数调试界面的参数
void Client::ReceiveParameter(QVariant pwm, QVariant pid)
{
	//qDebug() << "ReceiveParameter" << endl;
	QVector<int> D1 = pwm.value< QVector<int> >();
	QVector<float> D2 = pid.value< QVector<float> >();

	for (int i = 0; i < 8; i++)
	{
		MotorPWM[i].data_int = D1[i];
		//qDebug() << "pwm_motor" << i+1 << "=" << D1[i] << endl;
	}
	for (int i = 0; i < 2; i++)
	{
		LightPWM[i].data_int = D1[i + 8];
		//qDebug() << "pwm_light" << i + 1 << "=" << D1[i] << endl;
	}
	for (int i = 0; i < 4; i++)
	{
		ServoPWM[i].data_int = D1[i + 10];
		//qDebug() << "pwm_servo" << i + 1 << "=" << D1[i] << endl;
	}
	for (int i = 0; i < D2.size(); i++)
	{
		PidData[i].data_float = D2[i];
		//qDebug() << "pid" << i + 1 << "=" << D2[i] << endl;
	}
	SendDataToHost();//发送至下位机
}


void Client::StateChanged(int state)
{
	//if (state == QTcpSocket::UnconnectedState) {
	//	QString str = QString::fromLocal8Bit("已断开至服务器");
	//	emit appendMsgToTxtBox(str);
	//}
	//else if (state == QTcpSocket::ConnectedState) {
	//	QString str = QString::fromLocal8Bit("已连接至服务器");// +addr + ":" + port;
	//	emit appendMsgToTxtBox(str);
	//}
}


void Client::Read_Data()
{
	QByteArray buffer;
	//读取缓冲区数据
	buffer = clientSocket->readAll();
	//qDebug() << buffer;
	DataToByte receive_data;
	int i, j, k, x;
	QString name[14] = { "Pitch=","Roll=","Yaw=","Deep=" };
	x = 0;
	if (!buffer.isEmpty())
	{
		for (i = 4; i < buffer.size(); i += 4)
		{
			for (j = i, k = 0; k < 4; j++, k++)
			{
				receive_data.byte[k] = buffer[j];
			}
			if (!buffer[0])
			{
				emit appendMsgToTxtBox(QString("%1  ").arg(receive_data.data_int) + "\n");
			}
			else if (buffer[0])
			{
				emit appendMsgToTxtBox(name[x] + QString("%1  ").arg(receive_data.data_float) + "\n");
				x++;
				if (x>3)
				{
					x = 0;
				}
			}
		}
	}
}

void Client::SendDataToHost()
{
	Send_TO_W5500_Data(INT, Motor, sizeof(MotorPWM) / sizeof(MotorPWM[0]), MotorPWM);
	Send_TO_W5500_Data(INT, Light, sizeof(LightPWM) / sizeof(LightPWM[0]), LightPWM);
	Send_TO_W5500_Data(INT, Servo_PWM, sizeof(ServoPWM) / sizeof(ServoPWM[0]), ServoPWM);
	Send_TO_W5500_Data(FLOAT, PID, sizeof(PidData) / sizeof(PidData[0]), PidData);
	Send_TO_W5500_Data(FLOAT, Deep_LZQ, sizeof(Deep_lzq) / sizeof(Deep_lzq[0]), Deep_lzq);
}

void Client::Send_TO_W5500_Data(unsigned char data_type, unsigned char data_category, int data_size, DataToByte data[])
{
	int i, j, k;
	QByteArray send_data;
	send_data.resize(data_size*4+4);
	send_data[0] = data_type;
	send_data[1] = data_category;
	send_data[2] = data_size;
	send_data[3] = 0x04;

	for (i = 0; i < data_size; i++)
	{
		for (j = (i + 1) * 4, k = 0; k < 4; j++, k++)
		{
			send_data[j] = data[i].byte[k];
		}
	}
	clientSocket->write(send_data);
}

//处理主界面的电机速度、灯光参数
void Client::GetGearValue(char order, int gear)
{
	//switch (order)  
	//{
	//case s_velocitypwm: throttle = gear; break;
	//case s_lightpwm: lightPwm = gear; break;
	//default:
	//	emit appendMsgToTxtBox("[gearErr]" + QString::fromLocal8Bit("无此参数"));
	//	break;
	//}
}

//根据实际改动
void Client::keyControl(QKeyEvent* e)
{
	//qDebug() << "key press";
	////由于Qt的原因只能获取单个按键(最后一个按下)，shift、ctrl和alt等功能键除外。
	//if ((e->modifiers() & Qt::ShiftModifier)/* && (e->key() == Qt::Key_A)*/)
	//{
	//	if (e->key() == Qt::Key_A) { //左平移
	//		qDebug() << "shift + a";
	//		return;
	//	}
	//	else if (e->key() == Qt::Key_D) { //右平移
	//		qDebug() << "shift + d";
	//		return;
	//	}
	//}

	switch (e->key())
	{
		//1~5:速度档   Q:速度加档  E:速度减档  
		//水平 W:前  S:后  A:左  D:右
		//Z:上升  X:下潜
		//9:控制小灯开关  0:控制大灯开关  -:大灯光 减档  +:大灯光 加档

		case Qt::Key_1:
			speed_gear_h = 0;
			qDebug() << "speed_gear_h="<< speed_gear_h;
			break;

		case Qt::Key_2:
			speed_gear_h = 1;
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_3:
			speed_gear_h = 2;
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_4:
			speed_gear_h = 3;
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_5:
			speed_gear_h = 4;
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_6:
			speed_gear_h = 5;
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_9:
			if (small_light_flag) 
			{
				small_light_flag = 0;
			}
			else
			{
				small_light_flag = 1;
			}
			qDebug() << "small_light_flag=" << small_light_flag;
			break;

		case Qt::Key_0:
			if (light_flag)
			{
				light_flag = 0;
			}
			else
			{
				light_flag = 1;
			}
			qDebug() << "light_flag=" << light_flag;
			break;

		case Qt::Key_Equal:
			if (light_gear < 4)
			{
				light_gear++;
			}
			else
			{
				light_gear = 4;
			}
			qDebug() << "light_gear=" << light_gear;
			break;

		case Qt::Key_Minus:
			if (light_gear > 0)
			{
				light_gear--;
			}
			else
			{
				light_gear = 0;
			}
			qDebug() << "light_gear=" << light_gear;
			break;

		case Qt::Key_W:
			//qDebug() << "test W";
			left_flag = 1;
			right_flag = 1;
			qDebug() << "left_flag=" << left_flag;
			qDebug() << "right_flag=" << right_flag;
			break;

		case Qt::Key_S:
			left_flag = -1;
			right_flag = -1;
			qDebug() << "left_flag=" << left_flag;
			qDebug() << "right_flag=" << right_flag;
			break;

		case Qt::Key_A:
			left_flag = -1;
			right_flag = 1;
			/*qDebug() << "left_flag=" << left_flag;
			qDebug() << "right_flag=" << right_flag;*/
			break;

		case Qt::Key_D:
			left_flag = 1;
			right_flag = -1;
			qDebug() << "left_flag=" << left_flag;
			qDebug() << "right_flag=" << right_flag;
			break;

		case Qt::Key_Q:
			if (speed_gear_h<4)
			{
				speed_gear_h++;
			}
			else
			{
				speed_gear_h = 4;
			}
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_E:
			if (speed_gear_h >0)
			{
				speed_gear_h--;
			}
			else
			{
				speed_gear_h = 0;
			}
			qDebug() << "speed_gear_h=" << speed_gear_h;
			break;

		case Qt::Key_Z:
			if (speed_gear_v < 4)
			{
				speed_gear_v++;
			}
			else
			{
				speed_gear_v = 4;
			}
			Deep_lzq[0].data_float++;
			qDebug() << "Deep_lzq=" << Deep_lzq[0].data_float;
			break;

		case Qt::Key_X:
			if (speed_gear_v > 0)
			{
				speed_gear_v--;
			}
			else
			{
				speed_gear_v = 0;
			}
			Deep_lzq[0].data_float--;
			qDebug() << "Deep_lzq=" << Deep_lzq[0].data_float;
			break;

		case Qt::Key_B:
			up_down_flag = 0;
			qDebug() << "up_down_flag=" << up_down_flag;
			break;

		case Qt::Key_C:
			up_down_flag = 1;
			qDebug() << "up_down_flag=" << up_down_flag;
			break;

		case Qt::Key_V:
			up_down_flag = -1;
			qDebug() << "up_down_flag=" << up_down_flag;
			break;

		case Qt::Key_I:						//舵机前
			ServoPWM[2].data_int--;
			if (ServoPWM[2].data_int < 158) ServoPWM[2].data_int = 158;
			break;
		case Qt::Key_K:						//舵机后
			ServoPWM[2].data_int++;
			if (ServoPWM[2].data_int > 255) ServoPWM[2].data_int = 275;
			break;
		case Qt::Key_J:						//舵机左
			ServoPWM[1].data_int++;
			if (ServoPWM[1].data_int > 210) ServoPWM[1].data_int = 210;
			break; 
		case Qt::Key_L:						//舵机右
			ServoPWM[1].data_int--;
			if (ServoPWM[1].data_int < 128) ServoPWM[1].data_int = 128;
			break;
		case Qt::Key_U:						//舵机上
			ServoPWM[3].data_int--;
			if (ServoPWM[3].data_int < 66) ServoPWM[3].data_int = 66;
			break;
		case Qt::Key_O:						//舵机下
			ServoPWM[3].data_int++;
			if (ServoPWM[3].data_int > 222) ServoPWM[3].data_int = 222;
			break;
		case Qt::Key_H:						//舵机抓取
			if (grap_flag == 1)
			{
				qDebug() << "graping" << endl;
				ServoPWM[0].data_int = 180;
				grap_flag = 0;
			}
			else if (grap_flag == 0)
			{
				qDebug() << "releaseing" << endl;
				ServoPWM[0].data_int = 130;
				grap_flag = 1;
			}
			break;
		default:
			//qDebug() << e->key();
			return;
			break;
	}
	for (int i = 0; i < 4; i++)
	{
		qDebug() << "ServoPWM[" << i << "].data_int = " << ServoPWM[i].data_int << endl;
	}
	FixData();
}

void Client::CheckData(QByteArray data)
{

}

void Client::FixData()
{
	//水平 左电机
	MotorPWM[0].data_int = 1500 + (speed_gear_h * left_flag*100);
	MotorPWM[3].data_int = 1500 + (speed_gear_h * left_flag * 100);

	//水平 右电机
	MotorPWM[1].data_int = 1500 + (speed_gear_h * right_flag * 100);
	MotorPWM[2].data_int = 1500 + (speed_gear_h * right_flag * 100);

	MotorPWM[4].data_int = 1500 - (speed_gear_v * up_down_flag * 100);
	MotorPWM[5].data_int = 1500 - (speed_gear_v * up_down_flag * 100);
	MotorPWM[6].data_int = 1500 + (speed_gear_v * up_down_flag * 100);
	MotorPWM[7].data_int = 1500 + (speed_gear_v * up_down_flag * 100);

	LightPWM[0].data_int = 10000 * small_light_flag;
	LightPWM[1].data_int = 50 * light_gear * light_flag;

	/*for (int i = 0; i < 8; i++)
	{
		qDebug() << "MotorPWM["<<i<<"].data_int=" << MotorPWM[i].data_int;
	}
	for (int i = 0; i < 2; i++)
	{
		qDebug() << "LightPWM[" << i << "].data_int=" << LightPWM[i].data_int;
	}*/

	SendDataToHost();//发送至下位机
}

void Client::SendData()
{

}
