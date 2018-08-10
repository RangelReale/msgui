#include "msgui/Frame.h"
#include "msgui/Util.h"

#include <QBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QMenu>

namespace msgui {

void Frame_Editor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *menu = createStandardContextMenu();
	menu->addSeparator();
	QAction *actForceIndent = menu->addAction(tr("&Force indent C++ types"));
	connect(actForceIndent, &QAction::triggered, this, &Frame_Editor::forceIndentClicked);
	actForceIndent->setCheckable(true);
	actForceIndent->setChecked(forceindent);
	menu->exec(event->globalPos());
	delete menu;
}

void Frame_Editor::forceIndentClicked()
{
	forceindent = dynamic_cast<QAction*>(sender())->isChecked();
	emit forceIndentChanged();
}


Frame::Frame(msglib::cmd::base::ptr frame, itf::Configuration *configuration, QWidget *parent) :
	QWidget(parent), _configuration(configuration), _frame(frame), _forceindent()
{
	//setStyleSheet("border: 0px; background-color: yellow;");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	_kind = new QLabel(this);
	//_kind->setWordWrap(true);
	_kind->setContentsMargins(2, 2, 2, 2);

	_name = new Frame_Editor(this);
	connect(_name, &Frame_Editor::forceIndentChanged, this, &Frame::onForceIndentChanged);
	QFont font();
	_name->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	_name->setReadOnly(true);
	_name->setFrameStyle(QFrame::Plain);
	_name->setFrameShape(QFrame::NoFrame);
	_configuration->createCPPHighligher(_name->document());

	layout->addWidget(_kind, 0);
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
			_name->setPlainText(_configuration->indentCPPType(c->name, _name->forceindent));

			_name->setToolTip(QString("<pre>%1</pre>").arg(Util::indentCPPType(c->name).toHtmlEscaped()));
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::type_>(frame)) {
			_kind->setStyleSheet("background-color: yellow;");
			_kind->setText("Type");
			_name->setPlainText(_configuration->indentCPPType(c->type_name, _name->forceindent));
			_name->setToolTip(QString("<pre>%1</pre>").arg(Util::indentCPPType(c->type_name).toHtmlEscaped()));
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

void Frame::onForceIndentChanged()
{
	setFrame(_frame);
}

}