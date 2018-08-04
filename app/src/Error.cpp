#include "msgui/Error.h"

#include <QBoxLayout>
#include <QFontDatabase>
#include <QIcon>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

namespace msgui {

Error::Error(itf::Configuration *configuration, QWidget *parent) :
	QFrame(parent), _configuration(configuration), _message()
{
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Sunken);

	QHBoxLayout *layout = new QHBoxLayout;
	setLayout(layout);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSizeConstraint(QLayout::SetMaximumSize);

	setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

	_icon = new QLabel(this);
	_icon->setContentsMargins(10, 10, 10, 0);
	_icon->setPixmap(QIcon(":/dialog-error.png").pixmap(QSize(48, 48)));

	_error = new mredit::Editor(this);
	_error->setReadOnly(true);
	_error->setLabelLayout(true);
	_configuration->createCPPHighligher(_error->document());

	layout->addWidget(_icon, 1, Qt::AlignHCenter | Qt::AlignTop);
	layout->addWidget(_error, 10);

	setVisible(false);
}

void Error::setMessage(const QString &message)
{
	if (!message.isEmpty())
	{
		QString msg(_error->toPlainText());
		if (!msg.isEmpty()) msg.append("\n");
		msg.append(message);

		_error->setPlainText(msg);
		setVisible(true);
	}
	else
	{
		_error->setPlainText("");
		setVisible(false);
	}
	_message = message;
}

void Error::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		//emit showFileAndLine(c->source_location);
	}
}

void Error::onProjectChanged()
{
	setMessage(_message);
}

}