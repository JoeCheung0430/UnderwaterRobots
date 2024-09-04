#pragma once

#include <QtWidgets/QWidget>
#include "camera_record.h"
#include "parameterset.h"
#include "ui_hcrov.h"
#include <QTcpSocket>
#include "client.h"
#include "control.h"
#include <QMenu>

enum sdata
{
	s_pitch = 1,
	s_roll = 2,
	s_yaw = 3,
	s_direction = 4,//指南针
	s_temp = 5,
	s_depth = 6, //depth是保留的
	s_electric = 7,
	s_velocitypwm = 8,
	s_lightpwm = 9
};


class hcrov : public QWidget
{
    Q_OBJECT

public:
    hcrov(QWidget *parent = Q_NULLPTR);
	~hcrov();

	//刷新左侧相机列表
	void RefreshCams();

	//编辑摄像机
	void SetCam(int index);

    //定时器渲染视频 回调函数
    void timerEvent(QTimerEvent* ev) override;
	//右键菜单
	void contextMenuEvent(QContextMenuEvent* event) override;

	//窗口尺寸变化
	void resizeEvent(QResizeEvent* e)override;

	//双击全屏
	void mouseDoubleClickEvent(QMouseEvent* e)override;
	void keyPressEvent(QKeyEvent* e)override;
	void keyReleaseEvent(QKeyEvent* e)override;
	void SetPause(bool isPause);
	void ViewInit();
	void Record(int cam);

signals:
	void getAddr(QString addr, QString port);
	void sendMsgToHost(QByteArray msg);
	void getGear(char order, int gear);
	void getKey(QKeyEvent* e);
	void send_file_path(QString file_path);

public slots:
	void AddCam();  //新增摄像机配置
	void SetCam();  //
	void DelCam();  //

	void OpenFile();
	void GetFilePath();
	void PlayOrPause();
	void StartRecord();
	void StopRecord();
	void ConnectOrDis();
	void ShowMsg(QString msg);
	void ShowPosture(char order, float data);
	void SendMsg();
	void ClearMsg();
	void VelSliderPress();
	void BriSliderPress();
	void ChangeSliderValue(char order, int value);
	void Parameter();
	void videoUrlOpen();

	void StopAll();
	void StopMaster();
	void StopSub1();
	void StopSub2();

	void CloseAll();
	void CloseMaster();
	void CloseSub1();
	void CloseSub2();

	void RecordMaster();
	void RecordSub1();
	void RecordSub2();

	void StopRecordMaster();
	void StopRecordSub1();
	void StopRecordSub2();

private:
    Ui::hcrovClass ui;
	QTextCodec* gbk = QTextCodec::codecForName("GBK");
	QMenu right_menu_;
	bool is_stop_[4] = {0};
	CameraRecord* rec_[3] = { 0 };
	int direction =0;
	ParameterSet *parameter_ui;

protected:
	void closeEvent(QCloseEvent* event);
};
extern hcrov* HCROV;