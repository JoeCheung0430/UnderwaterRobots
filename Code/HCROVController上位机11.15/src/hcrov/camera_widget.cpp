#include "camera_widget.h"
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QListWidget>
#include "xdemux_task.h"
#include "xdecode_task.h"
#include "xvideo_view.h"
#include "camera_config.h"

//��Ⱦ
void CameraWidget::paintEvent(QPaintEvent* p)
{
    //��Ⱦ��ʽ��
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

CameraWidget::CameraWidget(QWidget* p) :QWidget(p)
{
    //������ק
    this->setAcceptDrops(true);
    this->setStyleSheet("background-color:rgb(51,51,51);");
}

//��ק����
void CameraWidget::dragEnterEvent(QDragEnterEvent* e)
{
    //������ק����
    e->acceptProposedAction();
}

//��Ⱦ��Ƶ
void CameraWidget::Draw()
{
    if (!demux_ || !decode_ || !view_)return;
    auto f = decode_->GetFrame();
    if (!f)return;
    view_->DrawFrame(f);
    XFreeFrame(&f);
}

//������Դ
CameraWidget::~CameraWidget()
{
    if (demux_)
    {
        demux_->Stop();
        delete demux_;
        demux_ = nullptr;
    }
    if (decode_)
    {
        decode_->Stop();
        delete decode_;
        decode_ = nullptr;
    }
    if (view_)
    {
        view_->Close();
        delete view_;
        view_ = nullptr;
    }
}

bool CameraWidget::Open(const char* url)
{
    if (demux_)
        demux_->Stop();
    if (decode_)
        decode_->Stop();
    //�򿪽��װ�߳�
    demux_ = new XDemuxTask();
    if (!demux_->Open(url))
    {
        return false;
    }
    //����Ƶ�������߳�
    decode_ = new XDecodeTask();
    auto para = demux_->CopyVideoPara();
    if (!decode_->Open(para->para))
    {
        return false;
    }
    //�趨�����߳̽��ս��װ����
    demux_->set_next(decode_);

    //��ʼ����Ⱦ����
    view_ = XVideoView::Create();
    view_->set_win_id((void*)winId());
    view_->Init(para->para);

    //�������װ�ͽ����߳�
    demux_->Start();
    decode_->Start();
    return true;
}

void CameraWidget::Close()
{
    if (demux_)
        demux_->Stop();
    if (decode_)
        decode_->Stop();
    this->setStyleSheet("background-color:rgb(51,51,51);");
}

//��ק�ɿ�
void CameraWidget::dropEvent(QDropEvent* e)
{
    //�õ�url
    qDebug() << e->source()->objectName();
    auto wid = (QListWidget*)e->source();
    qDebug() << wid->currentRow();
    auto cam = CameraConfig::Instance()->GetCam(wid->currentRow());
    Open(cam.sub_url);
}