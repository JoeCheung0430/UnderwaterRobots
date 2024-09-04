#include "hcrov.h"
#include <QDebug>
#include "camera_config.h"
#include "camera_widget.h"
#include "playvideo.h"
#include <sstream>
#include <QDir>
#include <map>
#include <vector>
#include <QGridLayout>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QContextMenuEvent>

using namespace std;

#define CAM_CONF_PATH "camera.db"

Control* ctrl;

//渲染窗口
static CameraWidget* cam_wids[3] = { 0 };
//视频录制
static vector<CameraRecord*> records;

//解决中文乱码
#define C(s) QString::fromLocal8Bit(s)


hcrov::hcrov(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    parameter_ui = new ParameterSet();

    connect(client, SIGNAL(appendMsgToTxtBox(QString)), this, SLOT(ShowMsg(QString)));
    connect(client, SIGNAL(postureData(char, float)), this, SLOT(ShowPosture(char, float)));
    connect(client, SIGNAL(changeSlidervalue(char, int)), this, SLOT(ChangeSliderValue(char, int)));
    connect(parameter_ui, SIGNAL(send_parameter(QVariant, QVariant)), client, SLOT(ReceiveParameter(QVariant, QVariant)));

    ctrl = new Control();
    ctrl->start();

    ui.gaugeCompass->setValue(direction);





    //////////////////////////////////////
    // 初始化右键菜单

    auto a = right_menu_.addAction(C("全部停止\\播放"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopAll()));
    auto m = right_menu_.addMenu(C("停止\\播放"));
    a = m->addAction(C("主窗口"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopMaster()));
    a = m->addAction(C("辅窗口1"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopSub1()));
    a = m->addAction(C("辅窗口2"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopSub2()));
    right_menu_.addSeparator();

    a = right_menu_.addAction(C("全部关闭"));
    connect(a, SIGNAL(triggered()), this, SLOT(CloseAll()));

    m = right_menu_.addMenu(C("关闭"));
    a = m->addAction(C("主窗口"));
    connect(a, SIGNAL(triggered()), this, SLOT(CloseMaster()));
    a = m->addAction(C("辅窗口1"));
    connect(a, SIGNAL(triggered()), this, SLOT(CloseSub1()));
    a = m->addAction(C("辅窗口2"));
    connect(a, SIGNAL(triggered()), this, SLOT(CloseSub2()));

    right_menu_.addSeparator();
    a = right_menu_.addAction(C("全部开始录制"));
    connect(a, SIGNAL(triggered()), this, SLOT(StartRecord()));
    a = right_menu_.addAction(C("全部停止录制"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopRecord()));
    m = right_menu_.addMenu(C("录制"));
    a = m->addAction(C("录制主窗口"));
    connect(a, SIGNAL(triggered()), this, SLOT(RecordMaster()));
    a = m->addAction(C("录制辅窗口1"));
    connect(a, SIGNAL(triggered()), this, SLOT(RecordSub1()));
    a = m->addAction(C("录制辅窗口2"));
    connect(a, SIGNAL(triggered()), this, SLOT(RecordSub2()));
    m = right_menu_.addMenu(C("停止录制"));
    a = m->addAction(C("主窗口"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopRecordMaster()));
    a = m->addAction(C("辅窗口1"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopRecordSub1()));
    a = m->addAction(C("辅窗口2"));
    connect(a, SIGNAL(triggered()), this, SLOT(StopRecordSub2()));

    //刷新左侧摄像机列表
    CameraConfig::Instance()->Load(CAM_CONF_PATH);
    RefreshCams();
    ViewInit();
    //启动定时器渲染视频
    startTimer(1);
}

void hcrov::StopAll()
{
    if (is_stop_[3])
    {
        for (int i = 0; i < 4; i++)
        {
            is_stop_[i] = false;
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            is_stop_[i] = true;
        }
    }
}
void hcrov::StopMaster()
{
    if (is_stop_[0])
    {
        is_stop_[3] = false;
        is_stop_[0] = false;
    }
    else
    {
        is_stop_[0] = true;
    }
}
void hcrov::StopSub1()
{
    if (is_stop_[1])
    {
        is_stop_[3] = false;
        is_stop_[1] = false;
    }
    else
    {
        is_stop_[1] = true;
    }
}
void hcrov::StopSub2()
{
    if (is_stop_[2])
    {
        is_stop_[3] = false;
        is_stop_[2] = false;
    }
    else
    {
        is_stop_[2] = true;
    }
}

void hcrov::CloseAll()
{
    for (int i = 0; i < 3; i++)
    {
        cam_wids[i]->Close();
        is_stop_[i] = false;
    }
    is_stop_[4] = false;
}
void hcrov::CloseMaster()
{
    cam_wids[0]->Close();
    is_stop_[0] = false;
}
void hcrov::CloseSub1()
{
    cam_wids[1]->Close();
    is_stop_[1] = false;
}
void hcrov::CloseSub2()
{
    cam_wids[2]->Close();
    is_stop_[2] = false;
}

void hcrov::Record(int Recordcam)
{
    auto conf = CameraConfig::Instance();
    auto cam = conf->GetCam(Recordcam);
    stringstream ss;
    ss << cam.save_path << "/" << Recordcam << "/";
    QDir dir;
    dir.mkpath(C(ss.str().c_str()));
    qDebug() << "path" << ss.str().c_str() << endl;
    if (!rec_[Recordcam])
    {
        rec_[Recordcam] = new CameraRecord();
    }
    rec_[Recordcam]->set_rtsp_url(cam.url);
    rec_[Recordcam]->set_save_path(ss.str().c_str());
    rec_[Recordcam]->set_file_sec(1800);
    rec_[Recordcam]->Start();
}

void hcrov::StartRecord()
{
    StopRecord();
    for (int i = 0; i < 3; i++)
    {
        Record(i);
    }
}

void hcrov::StopRecord()  //停止全部摄像头录制
{
    for (int i = 0; i < 3; i++)
    {
        if (rec_[i])
        {
            rec_[i]->Stop();
        }
    }
}

void hcrov::RecordMaster()
{
    StopRecordMaster();
    Record(0);
}
void hcrov::RecordSub1()
{
    StopRecordSub1();
    Record(1);
}
void hcrov::RecordSub2()
{
    StopRecordSub2();
    Record(2);
}

void hcrov::StopRecordMaster()
{
    if (rec_[0])
    {
        rec_[0]->Stop();
    }
}
void hcrov::StopRecordSub1()
{
    if (rec_[1])
    {
        rec_[1]->Stop();
    }
}

void hcrov::StopRecordSub2()
{
    if (rec_[2])
    {
        rec_[2]->Stop();
    }
}

void hcrov::ViewInit()
{
    auto lay1 = (QGridLayout*)ui.view_master->layout();
    if (!lay1)
    {
        lay1 = new QGridLayout();
        lay1->setContentsMargins(0, 0, 0, 0);
        ui.view_master->setLayout(lay1);
    }
    cam_wids[0] = new CameraWidget();
    lay1->addWidget(cam_wids[0], 1, 1);

    auto lay2 = (QGridLayout*)ui.view_sub1->layout();
    if (!lay2)
    {
        lay2 = new QGridLayout();
        lay2->setContentsMargins(0, 0, 0, 0);
        ui.view_sub1->setLayout(lay2);
    }
    cam_wids[1] = new CameraWidget();
    lay2->addWidget(cam_wids[1], 1, 1);

    auto lay3 = (QGridLayout*)ui.view_sub2->layout();
    if (!lay3)
    {
        lay3 = new QGridLayout();
        lay3->setContentsMargins(0, 0, 0, 0);
        ui.view_sub2->setLayout(lay3);
    }
    cam_wids[2] = new CameraWidget();
    lay3->addWidget(cam_wids[2], 1, 1);
}

hcrov::~hcrov()
{
    ctrl->quit();
}

void hcrov::closeEvent(QCloseEvent* event) //根据不同的需求进行添加，比如：
{
    ctrl->close();
    delete ctrl;
    ctrl = nullptr;
    StopRecord();
    qDebug() << "quit" << endl;
}

//UI初始化
void hcrov::resizeEvent(QResizeEvent* e)
{

}

void hcrov::keyPressEvent(QKeyEvent* e)
{
    emit getKey(e);
}

void hcrov::keyReleaseEvent(QKeyEvent* e)
{
    //qDebug() << e->key();
}

//双击全屏
void hcrov::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (isFullScreen())
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }
}

void hcrov::ShowMsg(QString msg)
{
    ui.txt_recvbox->append(msg);
}

void hcrov::ShowPosture(char order, float data)
{
    switch (order)
    {
    case s_direction:ui.lable_direction_data->setText(QString("%2").arg(data));
                     ui.gaugeCompass->setValue(data);
                     ui.gaugeCompass->update();
                     break;
    case s_depth:ui.lable_depth_data->setText(QString("%2").arg(data)); break;

    case s_electric:ui.pgb_electric->setValue((int)data); break;
    default:
        ShowMsg(QString("%2").arg(data));
        break;
    }
}

void hcrov::SendMsg()
{
    QString msg = ui.txt_sendbox->toPlainText();
    QByteArray str = gbk->fromUnicode(msg);
    emit sendMsgToHost(str);
}

void hcrov::ClearMsg()
{
    ui.txt_recvbox->clear();
}

void hcrov::VelSliderPress()
{
    qDebug() << this->ui.slider_velocity->value();
    emit getGear(s_velocitypwm, this->ui.slider_velocity->value());
}

void hcrov::BriSliderPress()
{
    qDebug() << this->ui.slider_bright->value();
    emit getGear(s_lightpwm, this->ui.slider_bright->value());
}

void hcrov::ChangeSliderValue(char order, int value)
{
    switch (order)
    {
    case s_velocitypwm:this->ui.slider_velocity->setValue(value); break;
    case s_lightpwm: this->ui.slider_bright->setValue(value); break;
    default:
        ShowMsg("[gearErr]" + QString::fromLocal8Bit("无此参数可设置"));
        break;
    }
}

void hcrov::PlayOrPause()
{
    //播放本地视频的时候才能用

}

void hcrov::SetPause(bool isPause)
{    

}

void hcrov::ConnectOrDis()
{
    if (ui.btn_connect->text() == QString::fromLocal8Bit("\n连接\n"))
    {
        emit getAddr(ui.line_IP->text().toLocal8Bit(), ui.line_PORT->text().toLocal8Bit());
        ui.btn_connect->setText(QString::fromLocal8Bit("\n断开\n"));
    }
    else {
        emit getAddr("0", "0");
        ui.btn_connect->setText(QString::fromLocal8Bit("\n连接\n"));
    }
}

void hcrov::GetFilePath()
{
    //选择文件
    QString file_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择保存路径"));
    if (file_path.isEmpty()) return;
    file_path = QDir::toNativeSeparators(file_path);
    file_path.append("\\");
    emit send_file_path(file_path);
}




void hcrov::OpenFile()
{
    ////选择文件
    //QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
    //if (path.isEmpty()) return;
    //ui.line_paly_video_addr->setText(path.toLocal8Bit());
    //static PlayVideo play;
    //play.Open(path.toLocal8Bit());
    //play.show();
}

//定时器渲染视频 回调函数
void hcrov::timerEvent(QTimerEvent* ev)
{
    //总窗口数量
    int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
    if (!is_stop_[3])
    {
        for (int i = 0; i < wid_size; i++)
        {
            if (cam_wids[i]&& !is_stop_[i])
            {
                //渲染多窗口视频
                cam_wids[i]->Draw();
            }
        }
    }
}

void hcrov::Parameter()
{
    parameter_ui->show();
    qDebug() << "send_parameter" << endl;
}

void hcrov::videoUrlOpen()
{

}

void hcrov::contextMenuEvent(QContextMenuEvent* event)
{
    //鼠标位置显示右键菜单
    right_menu_.exec(QCursor::pos());
    event->accept();
}

void hcrov::SetCam(int index)
{
    auto c = CameraConfig::Instance();
    QDialog dlg(this);
    dlg.resize(800, 200);
    QFormLayout lay;
    dlg.setLayout(&lay);
    //  标题1 输入框1
    //  标题2 输入框2
    QLineEdit name_edit;
    lay.addRow(C("名称"), &name_edit);

    QLineEdit url_edit;
    lay.addRow(C("主码流"), &url_edit);

    QLineEdit sub_url_edit;
    lay.addRow(C("辅码流"), &sub_url_edit);

    QWidget save_path;
    QHBoxLayout hlay;
    QLineEdit save_path_edit;
    QPushButton selectfile;
    sub_url_edit.resize(sub_url_edit.width(), sub_url_edit.height());
    selectfile.resize(100, sub_url_edit.height());
    selectfile.setText(C("选择"));
    hlay.setContentsMargins(0, 0, 0, 0);
    hlay.addWidget(&save_path_edit);
    hlay.addWidget(&selectfile);
    save_path.setLayout(&hlay);
    lay.addRow(C("保存路径"), &save_path);
    connect(&selectfile, SIGNAL(clicked()), this, SLOT(GetFilePath()));
    connect(this, SIGNAL(send_file_path(QString)), &save_path_edit, SLOT(setText(QString)));

    QPushButton save;
    save.setText(C("保存"));
    connect(&save, SIGNAL(clicked()), &dlg, SLOT(accept()));
    lay.addRow("", &save);

    //编辑 读入原数据显示
    if (index >= 0)
    {
        auto cam = c->GetCam(index);
        name_edit.setText(C(cam.name));
        url_edit.setText(C(cam.url));
        sub_url_edit.setText(C(cam.sub_url));
        save_path_edit.setText(C(cam.save_path));
    }

    for (;;)
    {
        if (dlg.exec() == QDialog::Accepted) //点击了保存
        {
            if (name_edit.text().isEmpty())
            {
                QMessageBox::information(0, "error", C("请输入名称"));
                continue;
            }
            if (url_edit.text().isEmpty())
            {
                QMessageBox::information(0, "error", C("请输入主码流"));
                continue;
            }
            if (sub_url_edit.text().isEmpty())
            {
                QMessageBox::information(0, "error", C("请输入辅码流"));
                continue;
            }
            if (save_path_edit.text().isEmpty())
            {
                QMessageBox::information(0, "error", C("请输入保存路径"));
                continue;
            }
            break;
        }
        return;
    }

    XCameraData data;
    strcpy(data.name, name_edit.text().toLocal8Bit());
    strcpy(data.url, url_edit.text().toLocal8Bit());
    strcpy(data.sub_url, sub_url_edit.text().toLocal8Bit());
    strcpy(data.save_path, save_path_edit.text().toLocal8Bit());
    if (index >= 0) //修改
    {
        c->SetCam(index, data);
    }
    else //新增
    {
        c->Push(data);  //插入数据
    }
    c->Save(CAM_CONF_PATH); //保存到文件
    RefreshCams();  //刷新显示
}

void hcrov::SetCam()
{
    int row = ui.cam_list->currentIndex().row();
    if (row < 0)
    {
        QMessageBox::information(this, "error", C("请选择摄像机"));
        return;
    }
    SetCam(row);
}

void hcrov::DelCam()
{
    int row = ui.cam_list->currentIndex().row();
    if (row < 0)
    {
        QMessageBox::information(this, "error", C("请选择摄像机"));
        return;
    }
    stringstream ss;
    ss << "您确认需要删除摄像机"
        << ui.cam_list->currentItem()->text().toLocal8Bit().constData();
    ss << "吗？";

    if (
        QMessageBox::information(this,
            "confirm",
            C(ss.str().c_str()),
            QMessageBox::Yes,
            QMessageBox::No) != QMessageBox::Yes
        )
    {
        return;
    }
    CameraConfig::Instance()->DelCam(row);
    CameraConfig::Instance()->Save(CAM_CONF_PATH);
    RefreshCams();
}
void hcrov::AddCam()
{
    SetCam(-1);
}

void hcrov::RefreshCams()
{
  auto c = CameraConfig::Instance();
      ui.cam_list->clear();
    int count = c->GetCamCount();
    for (int i = 0; i < count; i++)
    {
        auto cam = c->GetCam(i);
        auto item = new QListWidgetItem(
            QIcon(":/hcrov/img/cam.png"), C(cam.name));
        ui.cam_list->addItem(item);
    }

}
