#include "msgui/NewReleaseWindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QPushButton>
#include <QDesktopServices>
#include <QSettings>

namespace msgui {

NewReleaseWindow::NewReleaseWindow(QWidget *parent) : QDialog(parent, Qt::MSWindowsFixedSizeDialogHint)
{
	setWindowTitle("New Release");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	QHBoxLayout *namelayout = new QHBoxLayout;
	layout->addLayout(namelayout);

	_name = new QLabel(this);
	_version = new QLabel(this);

	namelayout->addWidget(_name, 5);
	namelayout->addWidget(_version, 1, Qt::AlignRight);

	_body = new QPlainTextEdit(this);
	_body->setReadOnly(true);
	layout->addWidget(_body);

	QHBoxLayout *btnlayout = new QHBoxLayout;
	layout->addLayout(btnlayout);
	QPushButton *btDownload = new QPushButton("&Download", this);
	QPushButton *btIgnore = new QPushButton("&Ignore", this);
	QPushButton *btLater = new QPushButton("&Later", this);

	btnlayout->addWidget(btDownload);
	btnlayout->addWidget(btIgnore);
	btnlayout->addWidget(btLater);

	connect(btDownload, &QPushButton::clicked, this, &NewReleaseWindow::onDownload);
	connect(btIgnore, &QPushButton::clicked, this, &NewReleaseWindow::onIgnore);
	connect(btLater, &QPushButton::clicked, this, &NewReleaseWindow::onLater);
}

NewReleaseWindow::~NewReleaseWindow()
{
	
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
