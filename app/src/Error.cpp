#include "msgui/Error.h"
#include "msgwidget/highlighter/HL_CPP.h"

#include <QBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include <QIcon>
#include <QDebug>

namespace msgui {

Error::Error(QWidget *parent) :
	QScrollArea(parent), _message()
{
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Sunken);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// root widget
	_root = new QWidget(this);

	QHBoxLayout *layout = new QHBoxLayout;
	_root->setLayout(layout);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSizeConstraint(QLayout::SetMaximumSize);

	setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

	QLabel *icon = new QLabel(_root);
	icon->setContentsMargins(10, 10, 10, 0);
	icon->setPixmap(QIcon(":/dialog-error.png").pixmap(QSize(48, 48)));

	_error = new mredit::Label(_root);
	_error->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	new msgwidget::highlighter::HL_CPP(_error->document());

	layout->addWidget(icon, 1, Qt::AlignHCenter | Qt::AlignTop);
	layout->addWidget(_error, 10);

	setWidget(_root);

	setVisible(false);
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

void Error::resizeEvent(QResizeEvent *event)
{
	QScrollArea::resizeEvent(event);
	// mredit::Label::sizeHint() returns the size of the QTextDocument
	_root->resize(_error->sizeHint());
}

}