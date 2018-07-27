#include "msgui/Error.h"
#include "msgwidget/highlighter/HL_CPP.h"

#include <QBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include <QIcon>

namespace msgui {

Error::Error(QWidget *parent) :
	QFrame(parent), _message()
{
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Sunken);

	QHBoxLayout *layout = new QHBoxLayout;
	setLayout(layout);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

	QLabel *icon = new QLabel(this);
	icon->setPixmap(QIcon(":/dialog-error.png").pixmap(QSize(48, 48)));

	_error = new mredit::Label(this);
	_error->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	new msgwidget::highlighter::HL_CPP(_error->document());

	layout->addWidget(icon, 1, Qt::AlignCenter);
	layout->addWidget(_error, 10);

	setVisible(false);

	//setStyleSheet("border: 1px solid red");
}

void Error::setMessage(const QString &message)
{
	if (!message.isEmpty())
	{
		_error->setPlainText(message);
		setVisible(true);
	}
	else
	{
		_error->setPlainText("");
		setVisible(false);
	}
}

void Error::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		//emit showFileAndLine(c->source_location);
	}
}

}