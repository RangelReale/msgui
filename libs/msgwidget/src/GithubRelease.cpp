#include "msgwidget/GithubRelease.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QSslError>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace msgwidget {

//
// Private
//
class GithubReleasePrivate : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(GithubReleasePrivate)
	Q_DECLARE_PUBLIC(GithubRelease)
	GithubRelease * const q_ptr;
	QNetworkAccessManager qnam;
	QNetworkReply *reply;
public:

	GithubReleasePrivate(GithubRelease *q) :
		QObject(q),
		q_ptr(q), reply(nullptr)
	{
		connect(&qnam, &QNetworkAccessManager::authenticationRequired,
			this, &GithubReleasePrivate::slotAuthenticationRequired);
		connect(&qnam, &QNetworkAccessManager::sslErrors,
			this, &GithubReleasePrivate::sslErrors);
	}

	void load(const QString &user, const QString &repo)
	{
		QUrl url(QString("https://api.github.com/repos/%1/%2/releases/latest").arg(user).arg(repo));
		reply = qnam.get(QNetworkRequest(url));

		//QSslConfiguration conf = reply->sslConfiguration();
		//conf.setPeerVerifyMode(QSslSocket::VerifyNone);
		//reply->setSslConfiguration(conf);

		connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &GithubReleasePrivate::httpError);
		connect(reply, &QIODevice::readyRead, this, &GithubReleasePrivate::httpReadyRead);
	}
private slots:
	void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
	{
		Q_Q(GithubRelease);
		emit q->onError("Error: authentication required");
	}

	void sslErrors(QNetworkReply *, const QList<QSslError> &errors)
	{
		Q_Q(GithubRelease);
		emit q->onError("Error: SSL error");
	}

	void httpError(QNetworkReply::NetworkError code)
	{
		Q_Q(GithubRelease);
		emit q->onError(QString("Network error: %1").arg(reply->errorString()));
	}

	void httpReadyRead()
	{
		Q_Q(GithubRelease);
		QByteArray data = reply->readAll();

		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson(data, &error);
		if (doc.isNull()) {
			emit q->onError(QString("Error: %1").arg(error.errorString()));
			return;
		}
		
		GithubReleaseInfo info;
		QJsonObject iobj = doc.object();
		info.url = iobj["url"].toString();
		info.id = iobj["id"].toInt();
		info.html_url = iobj["html_url"].toString();
		info.tag_name = iobj["tag_name"].toString();
		info.name = iobj["name"].toString();
		info.body = iobj["body"].toString();
		QJsonArray assets = iobj["assets"].toArray();
		emit q->onInfo(info);
	}
};


// Class
GithubRelease::GithubRelease(QObject *parent) : 
	QObject(parent), d_ptr(new GithubReleasePrivate(this))
{

}

GithubRelease::~GithubRelease()
{

}

void GithubRelease::load(const QString &user, const QString &repo)
{
	Q_D(GithubRelease);
	d->load(user, repo);
}

}

#include "GithubRelease.moc"