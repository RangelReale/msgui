#pragma once

#include "msgui/LogConfig.h"

#include <QTreeWidget>

#include "log4qt/loggingevent.h"

namespace msgui {

class Log : public QTreeWidget
{
	Q_OBJECT
public:
	Log(QWidget *parent);

	void addLog(const Log4Qt::LoggingEvent &event);
public slots:
	void columnResized(int logicalIndex, int oldSize, int newSize);
};

}
