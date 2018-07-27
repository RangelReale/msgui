#include "msgui/LogWindow.h"

#include <QApplication>
#include <QDesktopWidget>

namespace msgui {

LogWindow *LogWindow::_instance = nullptr;

LogWindow::LogWindow() : QMainWindow()
{
	setWindowTitle("Log");

	_instance = this;

	_logs = new Log(this);
	setCentralWidget(_logs);

	if (LogConfig::instance())
	{
		for (auto &log : LogConfig::instance()->list()) {
			onLog(log);
		}

		connect(LogConfig::instance(), &LogConfig::onLog, this, &LogWindow::onLog);
	}

	setAttribute(Qt::WA_DeleteOnClose);

	const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
	resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
	move((availableGeometry.width() - width()) / 2,
		(availableGeometry.height() - height()) / 2);
}

LogWindow::~LogWindow()
{
	if (LogConfig::instance())
	{
		disconnect(LogConfig::instance(), &LogConfig::onLog, this, &LogWindow::onLog);
	}

	_instance = nullptr;
}

LogWindow* LogWindow::instance()
{
	return _instance;
}

void LogWindow::onLog(const Log4Qt::LoggingEvent &event)
{
	_logs->addLog(event);
}

void LogWindow::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);
	_logs->columnResized(0, 0, 0);
	_logs->scrollToBottom();
}

}
