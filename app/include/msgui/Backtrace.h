#pragma once

#include "msgui/interfaces.h"

#include <msglib/Cmd.h>

#include <QLabel>
#include <QTreeWidget>

namespace msgui {

class Backtrace : public QTreeWidget
{
	Q_OBJECT
public:
	Backtrace(itf::Configuration *configuration, QWidget *parent);

	void setBacktrace(msglib::cmd::backtrace::ptr backtrace);
signals:
	void showFileAndLine(const QString &fileAndFile);
private slots:
	void btItemClicked(QTreeWidgetItem *item, int column);
	void columnResized(int logicalIndex, int oldSize, int newSize);
private:
	itf::Configuration *_configuration;
	msglib::cmd::backtrace::ptr _backtrace;
};

}