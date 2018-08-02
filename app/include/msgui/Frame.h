#pragma once

#include "msgui/interfaces.h"

#include <msglib/Cmd.h>
#include <mredit/Label.h>

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

namespace msgui {

class Frame : public QWidget
{
	Q_OBJECT
public:
	Frame(msglib::cmd::base::ptr frame, itf::Configuration *configuration, QWidget *parent);

	void setFrame(msglib::cmd::base::ptr frame);

	QSize sizeHint() const override;
signals:
	void showFileAndLine(const QString &fileAndFile);
protected:
	void mousePressEvent(QMouseEvent *event) override;
private:
	itf::Configuration *_configuration;
	QLabel * _kind;
	mredit::Label *_name;
	msglib::cmd::base::ptr _frame;
};

}