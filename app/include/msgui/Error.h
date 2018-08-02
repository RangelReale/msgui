#pragma once

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
	Error(QWidget *parent);

	void setMessage(const QString &message = QString());
signals:
	void showFileAndLine(const QString &fileAndFile);
protected:
	void mousePressEvent(QMouseEvent *event) override;
private:
	QLabel *_icon;
	mredit::Editor *_error;
	QString _message;
};

}