#pragma once

#include <msglib/Cmd.h>
#include <mredit/Label.h>

#include <QScrollArea>
#include <QMouseEvent>
#include <QResizeEvent>

namespace msgui {

class Error : public QScrollArea
{
	Q_OBJECT
public:
	Error(QWidget *parent);

	void setMessage(const QString &message = QString());
signals:
	void showFileAndLine(const QString &fileAndFile);
protected:
	void mousePressEvent(QMouseEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
private:
	QWidget *_root;
	mredit::Label *_error;
	QString _message;
};

}