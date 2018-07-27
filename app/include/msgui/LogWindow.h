#pragma once

#include "msgui/LogConfig.h"
#include "msgui/Log.h"

#include <QMainWindow>
#include <QListWidget>

namespace msgui {

class LogWindow : public QMainWindow
{
	Q_OBJECT
public:
	LogWindow();
	~LogWindow();

	static LogWindow* instance();
public slots:
	void onLog(const Log4Qt::LoggingEvent &);
protected:
	void showEvent(QShowEvent *event) override;
private:
	static LogWindow* _instance;

	Log * _logs;
};

}
