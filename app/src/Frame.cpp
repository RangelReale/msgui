#include "msgui/Frame.h"
#include "msgui/Util.h"

#include <QBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include <QFontMetrics>

namespace msgui {

Frame::Frame(msglib::cmd::base::ptr frame, itf::Configuration *configuration, QWidget *parent) :
	QWidget(parent), _configuration(configuration), _frame(frame)
{
	//setStyleSheet("border: 0px; background-color: yellow;");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	_kind = new QLabel(this);
	_kind->setWordWrap(true);
	_kind->setContentsMargins(2, 2, 2, 2);

	_name = new mredit::Label(this);
	_name->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	_name->setFrameStyle(QFrame::Sunken);
	_name->setFrameShape(QFrame::Panel);
	QPalette pal(_name->palette());
	pal.setColor(_name->backgroundRole(), Qt::white);
	_name->setPalette(pal);
	_name->setAutoFillBackground(true);
	_configuration->createCPPHighligher(_name->document());
	_name->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

	layout->addWidget(_kind, 1);
	layout->addWidget(_name, 5);

	setFrame(frame);
}

QSize Frame::sizeHint() const 
{
	QSize sh(QWidget::sizeHint());
	// default height of 6 lines of text
	int fh = fontMetrics().height() * 6;
	if (fh > sh.height())
		sh.setHeight(fh);
	return sh;
}

void Frame::setFrame(msglib::cmd::base::ptr frame)
{
	if (frame)
	{
		if (auto c = std::dynamic_pointer_cast<msglib::cmd::frame_base>(frame)) {
			_kind->setStyleSheet("background-color: yellow;");
			_kind->setText(c->kind);
			_name->setPlainText(_configuration->identCPPType(c->name));

			_name->setToolTip(QString("<pre>%1</pre>").arg(Util::identCPPType(c->name).toHtmlEscaped()));
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::type_>(frame)) {
			_kind->setStyleSheet("background-color: yellow;");
			_kind->setText("Type");
			_name->setPlainText(_configuration->identCPPType(c->type_name));
			_name->setToolTip(QString("<pre>%1</pre>").arg(Util::identCPPType(c->type_name).toHtmlEscaped()));
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::cpp_code>(frame)) {
			_kind->setStyleSheet("background-color: yellow;");
			_kind->setText("Code");
			_name->setPlainText(c->code);
			_name->setToolTip("");
		}
		else {
			_kind->setStyleSheet("");
			_kind->setText("");
			_name->setPlainText("");
			_name->setToolTip("");
		}
	}
	else
	{
		_kind->setStyleSheet("");
	}

	_frame = frame;
}

void Frame::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (_frame) {
			if (auto c = std::dynamic_pointer_cast<msglib::cmd::frame_base>(_frame)) {
				emit showFileAndLine(c->source_location);
			}
		}
	}
}

void Frame::onProjectChanged()
{
	_configuration->createCPPHighligher(_name->document());
	setFrame(_frame);
}

}