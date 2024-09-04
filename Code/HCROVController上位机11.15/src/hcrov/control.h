#pragma once

#include <QThread>
#include <QWidget>
#include <QKeyEvent>

class Control : public QThread
{

public:
	  Control();
	  ~Control();
	  void run();
	  void close();

};
