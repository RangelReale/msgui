#pragma once

#include <msgwidget/GithubRelease.h>

#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>

namespace msgui {

class NewReleaseWindow : public QDialog
{
	Q_OBJECT
public:
	NewReleaseWindow(QWidget *parent);
	~NewReleaseWindow();

	void setInfo(msgwidget::GithubReleaseInfo info);
public slots:
	void onDownload();
	void onIgnore();
	void onLater();
private:
	QLabel *_name;
	QLabel *_version;
	QPlainTextEdit *_body;

	msgwidget::GithubReleaseInfo _info;
};

}