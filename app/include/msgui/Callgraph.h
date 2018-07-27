#pragma once

#include <msglib/Cmd.h>
#include <msgwidget/WidgetListWidget.h>

#include <QLabel>
#include <QTreeWidget>

namespace msgui {

class Callgraph : public QTreeWidget
{
	Q_OBJECT
public:
	Callgraph(QWidget *parent);

	void setCallgraph(msglib::cmd::call_graph::ptr callgraph);
signals:
	void showFileAndLine(const QString &fileAndFile);
private slots:
	void btItemClicked(QTreeWidgetItem *item, int column);
	void columnResized(int logicalIndex, int oldSize, int newSize);
private:
	msglib::cmd::call_graph::ptr _callgraph;
};

}