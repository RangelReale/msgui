#pragma once

#include "msgui/interfaces.h"

#include <msglib/Cmd.h>
#include <msgwidget/WidgetListWidget.h>

#include <QLabel>
#include <QTreeWidget>

namespace msgui {

class Callgraph : public QTreeWidget
{
	Q_OBJECT
public:
	Callgraph(itf::Configuration *configuration, QWidget *parent);

	void setCallgraph(msglib::cmd::call_graph::ptr callgraph);
signals:
	void showFileAndLine(const QString &fileAndFile);
public slots:
	void onProjectChanged();
private slots:
	void btItemClicked(QTreeWidgetItem *item, int column);
	void columnResized(int logicalIndex, int oldSize, int newSize);
private:
	itf::Configuration *_configuration;
	msglib::cmd::call_graph::ptr _callgraph;
};

}