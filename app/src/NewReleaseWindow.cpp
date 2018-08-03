#include "msgui/NewReleaseWindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QPushButton>
#include <QDesktopServices>
#include <QSettings>

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

	_name = new QLabel(root);
	_version = new QLabel(root);

	namelayout->addWidget(_name, 5);
	namelayout->addWidget(_version, 1, Qt::AlignRight);

	_body = new QPlainTextEdit(root);
	_body->setReadOnly(true);
	layout->addWidget(_body);

	QHBoxLayout *btnlayout = new QHBoxLayout;
	layout->addLayout(btnlayout);
	QPushButton *btDownload = new QPushButton("&Download", root);
	QPushButton *btIgnore = new QPushButton("&Ignore", root);
	QPushButton *btLater = new QPushButton("&Later", root);

	btnlayout->addWidget(btDownload);
	btnlayout->addWidget(btIgnore);
	btnlayout->addWidget(btLater);

	connect(btDownload, &QPushButton::clicked, this, &NewReleaseWindow::onDownload);
	connect(btIgnore, &QPushButton::clicked, this, &NewReleaseWindow::onIgnore);
	connect(btLater, &QPushButton::clicked, this, &NewReleaseWindow::onLater);
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
}

void NewReleaseWindow::onDownload()
{
	QDesktopServices::openUrl(_info.html_url);
	close();
}

void NewReleaseWindow::onIgnore()
{
	QSettings settings;
	settings.setValue("ignoreupdate", _info.tag_name);
	close();
}

void NewReleaseWindow::onLater()
{
	close();
}


}
