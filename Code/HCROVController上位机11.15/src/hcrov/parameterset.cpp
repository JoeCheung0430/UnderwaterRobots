#include "parameterset.h"

ParameterSet::ParameterSet(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	qRegisterMetaType<QVariant>("QVariant");
	pwm.resize(14);
	pid.resize(6);

	ui.S_motor1->setMaximum(Motor_MaxiMum);
	ui.S_motor2->setMaximum(Motor_MaxiMum);
	ui.S_motor3->setMaximum(Motor_MaxiMum);
	ui.S_motor4->setMaximum(Motor_MaxiMum);
	ui.S_motor5->setMaximum(Motor_MaxiMum);
	ui.S_motor6->setMaximum(Motor_MaxiMum);
	ui.S_motor7->setMaximum(Motor_MaxiMum);
	ui.S_motor8->setMaximum(Motor_MaxiMum);
	ui.S_light1->setMaximum(Small_Light_MaxiMum);
	ui.S_light2->setMaximum(Light_MaxiMum);
	ui.S_servo1->setMaximum(Servo_MaxiMum);
	ui.S_servo2->setMaximum(Servo_MaxiMum);
	ui.S_servo3->setMaximum(Servo_MaxiMum);
	ui.S_servo4->setMaximum(Servo_MaxiMum);

	ui.S_motor1->setMinimum(Motor_MiniMum);
	ui.S_motor2->setMinimum(Motor_MiniMum);
	ui.S_motor3->setMinimum(Motor_MiniMum);
	ui.S_motor4->setMinimum(Motor_MiniMum);
	ui.S_motor5->setMinimum(Motor_MiniMum);
	ui.S_motor6->setMinimum(Motor_MiniMum);
	ui.S_motor7->setMinimum(Motor_MiniMum);
	ui.S_motor8->setMinimum(Motor_MiniMum);
	ui.S_light1->setMinimum(Small_Light_MiniMum);
	ui.S_light2->setMinimum(Light_MiniMum);
	ui.S_servo1->setMinimum(Servo_MiniMum);
	ui.S_servo2->setMinimum(Servo_MiniMum);
	ui.S_servo3->setMinimum(Servo_MiniMum);
	ui.S_servo4->setMinimum(Servo_MiniMum);

	ui.S_motor1->setSingleStep(Motor_SingStep);
	ui.S_motor2->setSingleStep(Motor_SingStep);
	ui.S_motor3->setSingleStep(Motor_SingStep);
	ui.S_motor4->setSingleStep(Motor_SingStep);
	ui.S_motor5->setSingleStep(Motor_SingStep);
	ui.S_motor6->setSingleStep(Motor_SingStep);
	ui.S_motor7->setSingleStep(Motor_SingStep);
	ui.S_motor8->setSingleStep(Motor_SingStep);
	ui.S_light1->setSingleStep(Small_Light_SingStep);
	ui.S_light2->setSingleStep(Light_SingStep);
	ui.S_servo1->setSingleStep(Servo_SingStep);
	ui.S_servo2->setSingleStep(Servo_SingStep);
	ui.S_servo3->setSingleStep(Servo_SingStep);
	ui.S_servo4->setSingleStep(Servo_SingStep);

	ui.S_motor1->setPageStep(Motor_PageStep);
	ui.S_motor2->setPageStep(Motor_PageStep);
	ui.S_motor3->setPageStep(Motor_PageStep);
	ui.S_motor4->setPageStep(Motor_PageStep);
	ui.S_motor5->setPageStep(Motor_PageStep);
	ui.S_motor6->setPageStep(Motor_PageStep);
	ui.S_motor7->setPageStep(Motor_PageStep);
	ui.S_motor8->setPageStep(Motor_PageStep);
	ui.S_light1->setPageStep(Small_Light_PageStep);
	ui.S_light2->setPageStep(Light_PageStep);
	ui.S_servo1->setPageStep(Servo_PageStep);
	ui.S_servo2->setPageStep(Servo_PageStep);
	ui.S_servo3->setPageStep(Servo_PageStep);
	ui.S_servo4->setPageStep(Servo_PageStep);

	ui.S_motor1->setValue(Motor_InitValue);
	ui.S_motor2->setValue(Motor_InitValue);
	ui.S_motor3->setValue(Motor_InitValue);
	ui.S_motor4->setValue(Motor_InitValue);
	ui.S_motor5->setValue(Motor_InitValue);
	ui.S_motor6->setValue(Motor_InitValue);
	ui.S_motor7->setValue(Motor_InitValue);
	ui.S_motor8->setValue(Motor_InitValue);
	ui.S_light1->setValue(Small_Light_InitValue);
	ui.S_light2->setValue(Light_InitValue);
	ui.S_servo1->setValue(Servo_InitValue);
	ui.S_servo2->setValue(Servo_InitValue);
	ui.S_servo3->setValue(Servo_InitValue);
	ui.S_servo4->setValue(Servo_InitValue);

}

ParameterSet::~ParameterSet()
{

}

void ParameterSet::parameter_send()
{
	QVariant data_pwm, data_pid;
	data_pwm.setValue(pwm);
	data_pid.setValue(pid);
	emit send_parameter(data_pwm, data_pid);
}

void ParameterSet::updata_parameter()
{
	pwm[0] = ui.S_motor1->value();
	pwm[1] = ui.S_motor2->value();
	pwm[2] = ui.S_motor3->value();
	pwm[3] = ui.S_motor4->value();
	pwm[4] = ui.S_motor5->value();
	pwm[5] = ui.S_motor6->value();
	pwm[6] = ui.S_motor7->value();
	pwm[7] = ui.S_motor8->value();

	pwm[8] = ui.S_light1->value();
	pwm[9] = ui.S_light2->value();

	pwm[10] = ui.S_servo1->value();
	pwm[11] = ui.S_servo2->value();
	pwm[12] = ui.S_servo3->value();
	pwm[13] = ui.S_servo4->value();

	pid[0] = ui.dsb_pid1->value();
	pid[1] = ui.dsb_pid2->value();
	pid[2] = ui.dsb_pid3->value();
	pid[3] = ui.dsb_pid4->value();
	pid[4] = ui.dsb_pid5->value();
	pid[5] = ui.dsb_pid6->value();

	QVariant data_pwm, data_pid;
	data_pwm.setValue(pwm);
	data_pid.setValue(pid);
	emit send_parameter(data_pwm, data_pid);
}
