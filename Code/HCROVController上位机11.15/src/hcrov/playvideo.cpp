#include "playvideo.h"

void PlayVideo::timerEvent(QTimerEvent* ev)
{
    if (!view_)return;
    auto f = decode_.GetFrame();
    if (!f)return;
    view_->DrawFrame(f);
    XFreeFrame(&f);
}
void PlayVideo::Close()
{
    //关闭上次数据
    demux_.Stop();
    decode_.Stop();
    if (view_)
    {
        view_->Close();
        delete view_;
        view_ = nullptr;
    }
}

void PlayVideo::closeEvent(QCloseEvent* ev)
{
    Close();
}
bool PlayVideo::Open(const char* url)
{
    if (!demux_.Open(url)) //解封装
    {
        return false;
    }
    auto vp = demux_.CopyVideoPara();
    if (!vp)
        return false;
    if (!decode_.Open(vp->para))//解码
    {
        return false;
    }
    demux_.set_next(&decode_);

    if (!view_)
        view_ = XVideoView::Create();
    view_->set_win_id((void*)winId());
    if (!view_->Init(vp->para)) //SDL渲染
        return false;
    demux_.set_syn_type(XSYN_VIDEO);
    demux_.Start();
    decode_.Start();
    return true;
}
PlayVideo::PlayVideo(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    startTimer(1);
}

PlayVideo::~PlayVideo()
{
    Close();
}