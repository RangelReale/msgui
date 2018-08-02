#include "msgui/NewReleaseWindow.h"

#include <QApplication>
#include <QBoxLayout>

namespace msgui {

NewReleaseWindow *NewReleaseWindow::_instance = nullptr;

NewReleaseWindow::NewReleaseWindow() : QMainWindow()
{
	setWindowTitle("New Release");
	setAttribute(Qt::WA_DeleteOnClose);

	_instance = this;

	QWidget *root = new QWidget(this);
	setCentralWidget(root);

	QVBoxLayout *layout = new QVBoxLayout;
	root->setLayout(layout);

	QHBoxLayout *namelayout = new QHBoxLayout;
	layout->addLayout(namelayout);

	_name = new QLabel(this);
	_version = new QLabel(this);

	namelayout->addWidget(_name, 5);
	namelayout->addWidget(_version, 1, Qt::AlignRight);

	_body = new QPlainTextEdit(this);
	_body->setReadOnly(true);
	layout->addWidget(_body);

	_link = new QLabel(this);
	_link->setTextFormat(Qt::RichText);
	_link->setTextInteractionFlags(Qt::TextBrowserInteraction);
	_link->setOpenExternalLinks(true);
	layout->addWidget(_link);
}

NewReleaseWindow::~NewReleaseWindow()
{
	_instance = nullptr;
}

NewReleaseWindow* NewReleaseWindow::instance()
{
	return _instance;
}

void NewReleaseWindow::setInfo(msgwidget::GithubReleaseInfo info)
{
	_info = info;

	_name->setText(info.name);
	_version->setText(info.tag_name);
	_body->setPlainText(info.body);
	_link->setText(QString("<a href=\"%1\">Download update</a>").arg(info.html_url));
}

}
