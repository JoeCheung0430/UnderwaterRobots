#pragma once
#include <QWidget>
class XDecodeTask;
class XDemuxTask;
class XVideoView;
class CameraWidget :public QWidget
{
    Q_OBJECT

public:
    CameraWidget(QWidget* p = nullptr);

    //��ק����
    void dragEnterEvent(QDragEnterEvent* e) override;

    //��ק�ɿ�
    void dropEvent(QDropEvent* e) override;

    //��Ⱦ
    void paintEvent(QPaintEvent* p);

    //��rtsp ��ʼ���װ ����
    bool Open(const char* url);

    void Close();

    //��Ⱦ��Ƶ
    void Draw();

    //������Դ
    ~CameraWidget();
private:
    XDecodeTask* decode_ = nullptr;
    XDemuxTask* demux_ = nullptr;
    XVideoView* view_ = nullptr;
};

