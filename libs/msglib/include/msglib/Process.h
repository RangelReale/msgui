#pragma once

#include <QProcess>
#include <QJsonDocument>

namespace msglib {

class Process : public QProcess
{
	Q_OBJECT
public:
	Process(QObject *parent = 0);

	const QString &program() const;
	void setProgram(const QString &program);

	QStringList &args();
	QStringList &startupCmds();

	QProcess::ProcessState processState() const;

	void writeRawCommand(const QJsonDocument &command);
	void writeCmd(const QString &cmd);
	void writeCmdList(const QStringList &cmdlist);
	void writeCodeCompletion(const QString &cmd);
signals:
	void onStandardOutput(const QString &line);
	void onStandardError(const QString &line);

	void onSendCommand(const QJsonDocument &cmd);

	void onError(const QString &message);
	void onCommand(const QJsonDocument &cmd);
	void onPrompt(const QJsonDocument &cmd);
	void onCommandError(const QJsonDocument &cmd);
public slots:
	void run();
	void quit();
	void restart();
private slots :
	void internalReadStdout();
	void internalReadStderr();
	void internalFinished(int exitCode, QProcess::ExitStatus exitstatus);
	void internalProcError(QProcess::ProcessError error);
private:
	QString escapeCmd(const QString &cmd);
	void writeCmdDirect(const QString &cmd);
	void parseLine(const QString &line);

	QString _program;
	QStringList _args;
	QStringList _startupcmds;

	QStringList _pendingstartupcmds;
	QList<QJsonDocument> _pendingcmds;

	bool _isrestart;
	bool _isprompt;
};

}