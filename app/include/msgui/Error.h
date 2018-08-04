#pragma once

#include "msgui/interfaces.h"

#include <msglib/Cmd.h>
#include <mredit/Editor.h>

#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QResizeEvent>

namespace msgui {

class Error : public QFrame
{
	Q_OBJECT
public:
	Error(itf::Configuration *configuration, QWidget *parent);

	void setMessage(const QString &message = QString());
signals:
	void showFileAndLine(const QString &fileAndFile);
public slots:
	void onProjectChanged();
protected:
	void mousePressEvent(QMouseEvent *event) override;
private:
	itf::Configuration *_configuration;
	QLabel *_icon;
	mredit::Editor *_error;
	QString _message;
};

}