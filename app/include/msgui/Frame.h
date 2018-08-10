#pragma once

#include "msgui/interfaces.h"

#include <msglib/Cmd.h>
#include <mredit/Editor.h>

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QContextMenuEvent>

namespace msgui {

class Frame_Editor : public mredit::Editor
{
	Q_OBJECT
public:
	using Editor::Editor;

	bool forceindent = false;
signals:
	void forceIndentChanged();
private slots:
	void forceIndentClicked();
protected:
	void contextMenuEvent(QContextMenuEvent *event);
};

class Frame : public QWidget
{
	Q_OBJECT
public:
	Frame(msglib::cmd::base::ptr frame, itf::Configuration *configuration, QWidget *parent);

	void setFrame(msglib::cmd::base::ptr frame);

	QSize sizeHint() const override;
signals:
	void showFileAndLine(const QString &fileAndFile);
public slots:
	void onProjectChanged();
	void onForceIndentChanged();
protected:
	void mousePressEvent(QMouseEvent *event) override;
private:
	itf::Configuration *_configuration;
	QLabel * _kind;
	Frame_Editor *_name;
	msglib::cmd::base::ptr _frame;
	bool _forceindent;
};

}