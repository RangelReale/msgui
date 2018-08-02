#include "msgwidget/GithubRelease.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QSslError>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>

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
		connect(reply, &QNetworkReply::finished, this, &GithubReleasePrivate::httpFinished);
		connect(reply, &QIODevice::readyRead, this, &GithubReleasePrivate::httpReadyRead);
		connect(reply, &QNetworkReply::downloadProgress, this, &GithubReleasePrivate::httpDownloadProcess);
	}
private slots:
	void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
	{
		Q_Q(GithubRelease);
		emit q->onLog("slotAuthenticationRequired");
	}

	void sslErrors(QNetworkReply *, const QList<QSslError> &errors)
	{
		Q_Q(GithubRelease);
		emit q->onLog("sslErrors");
	}

	void httpError(QNetworkReply::NetworkError code)
	{
		Q_Q(GithubRelease);
		emit q->onLog(QString("httpError: %1").arg(reply->errorString()));
	}

	void httpFinished()
	{
		Q_Q(GithubRelease);
		emit q->onLog("httpFinished");
	}

	void httpReadyRead()
	{
		Q_Q(GithubRelease);
		QByteArray data = reply->readAll();
		emit q->onLog(QString("httpReadyRead: %1").arg(QString::fromUtf8(data)));

		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson(data, &error);
		if (doc.isNull()) {
			emit q->onError(QString("Error: %1").arg(error.errorString()));
			return;
		}
		
		GithubReleaseInfo info;
		QJsonObject iobj = doc.object();
		info.url = iobj.value("url").toString();
		info.id = iobj.value("id").toInt();
		info.html_url = iobj.value("html_url").toString();
		info.tag_name = iobj.value("tag_name").toString();
		info.name = iobj.value("name").toString();
		info.body = iobj.value("body").toString();
		info.filesize = iobj.value("assets").toObject().value("size").toInt();
		info.filename = iobj.value("assets").toObject().value("name").toString();
		info.browser_download_url = iobj.value("assets").toObject().value("browser_download_url").toString();
		info.body = iobj.value("body").toString();
		info.body = iobj.value("body").toString();

		//emit q->onLog(QString("httpReadyRead: %1").arg(QString::fromUtf8(data)));
		emit q->onLog(QString("httpReadyRead: %1").arg(QString::fromUtf8(doc.toJson(QJsonDocument::Indented))));
	}

	void httpDownloadProcess(qint64 bytesReceived, qint64 bytesTotal)
	{
		Q_Q(GithubRelease);
		emit q->onLog(QString("Process: %1 / %2").arg(bytesReceived).arg(bytesTotal));
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