#pragma once

#include <QWidget>
#include "ui_playvideo.h"
#include "xdemux_task.h"
#include "xdecode_task.h"
#include "xvideo_view.h"

class PlayVideo : public QWidget
{
	Q_OBJECT

public:
	PlayVideo(QWidget *parent = Q_NULLPTR);
	~PlayVideo();

	bool Open(const char* url);

	void timerEvent(QTimerEvent* ev) override;
	void Close();
	void closeEvent(QCloseEvent* ev) override;

private:
	Ui::PlayVideo ui;
	XDemuxTask demux_;
	XDecodeTask decode_;
	XVideoView* view_ = nullptr;
};
