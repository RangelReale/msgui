#pragma once

#include <msglib/Cmd.h>
#include <mredit/Label.h>

#include <QFrame>
#include <QMouseEvent>

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
	mredit::Label *_error;
	QString _message;
};

}