#pragma once

#include <QWidget>
#include "ui_parameterset.h"
#include <QVector>
#include <QMetaType>
#include <QVariant>
#include <QDebug>



#define PID_DATA "pid_arameter.db"
#define Motor_MaxiMum   2000
#define Motor_MiniMum   1000
#define Motor_SingStep  1
#define Motor_PageStep  100
#define Motor_InitValue 1500

#define Light_MaxiMum   200
#define Light_MiniMum   0
#define Light_SingStep  1
#define Light_PageStep  50
#define Light_InitValue 0

#define Small_Light_MaxiMum   20000
#define Small_Light_MiniMum   0
#define Small_Light_SingStep  1
#define Small_Light_PageStep  5000
#define Small_Light_InitValue 0

#define Servo_MaxiMum   2500
#define Servo_MiniMum   500
#define Servo_SingStep  1
#define Servo_PageStep  100
#define Servo_InitValue 1500


Q_DECLARE_METATYPE(QVector<float>);
Q_DECLARE_METATYPE(QVector<int>);

class ParameterSet : public QWidget
{
	Q_OBJECT

public:
	ParameterSet(QWidget *parent = Q_NULLPTR);
	~ParameterSet();
	
signals:
	void send_parameter(QVariant, QVariant);

public slots:
	void parameter_send();
	void updata_parameter();
private:
	Ui::ParameterSet ui;

	QVector<int>   pwm;
	QVector<float> pid;
};
