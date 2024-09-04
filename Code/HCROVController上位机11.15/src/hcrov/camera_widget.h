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

    //拖拽进入
    void dragEnterEvent(QDragEnterEvent* e) override;

    //拖拽松开
    void dropEvent(QDropEvent* e) override;

    //渲染
    void paintEvent(QPaintEvent* p);

    //打开rtsp 开始解封装 解码
    bool Open(const char* url);

    void Close();

    //渲染视频
    void Draw();

    //清理资源
    ~CameraWidget();
private:
    XDecodeTask* decode_ = nullptr;
    XDemuxTask* demux_ = nullptr;
    XVideoView* view_ = nullptr;
};

