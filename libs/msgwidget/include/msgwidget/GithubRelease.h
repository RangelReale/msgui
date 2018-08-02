#pragma once

#include <QObject>
#include <QScopedPointer>

namespace msgwidget {

class GithubReleaseInfo
{
public:
	GithubReleaseInfo() {}

	QString url;
	int id;
	QString html_url;
	QString tag_name;
	QString name;
	QString body;
};
Q_DECLARE_METATYPE(GithubReleaseInfo)


class GithubReleasePrivate;

class GithubRelease : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(GithubRelease)
public:
	GithubRelease(QObject *parent = nullptr);
	~GithubRelease();

	void load(const QString &user, const QString &repo);
signals:
	void onInfo(GithubReleaseInfo info);
	void onLog(const QString &message);
	void onError(const QString &message);
private:
	QScopedPointer<GithubReleasePrivate> const d_ptr;
};

}