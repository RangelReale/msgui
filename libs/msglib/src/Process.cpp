#include "msglib/Process.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include <iostream>

namespace msglib {

Process::Process(QObject *parent) :
	QProcess(parent), _program("metashell"), _args(), _startupcmds(), _pendingstartupcmds(), _pendingcmds(), _isrestart(false), _isprompt(false)
{
	setProcessChannelMode(QProcess::MergedChannels);

	connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(internalReadStdout()));
	connect(this, SIGNAL(readyReadStandardError()), this, SLOT(internalReadStderr()));
	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(internalFinished(int, QProcess::ExitStatus)));
	connect(this, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(internalProcError(QProcess::ProcessError)));
}

const QString &Process::program() const
{
	return _program;
}

void Process::setProgram(const QString &program)
{
	_program = program;
}

QStringList &Process::args()
{
	return _args;
}

QStringList &Process::startupCmds()
{
	return _startupcmds;
}

QProcess::ProcessState Process::processState() const
{
	return QProcess::state();
}

void Process::writeRawCommand(const QJsonDocument &command)
{
	emit onSendCommand(command);
	_isprompt = false;
	QByteArray cmd(command.toJson(QJsonDocument::JsonFormat::Compact));
	QProcess::write(cmd + "\n");
}

void Process::writeCmd(const QString &cmd)
{
	QJsonObject cmddata;
	cmddata["type"] = "cmd";
	cmddata["cmd"] = cmd;
	
	if (_isprompt)
		writeRawCommand(QJsonDocument(cmddata));
	else
		_pendingcmds.append(QJsonDocument(cmddata));
}

void Process::writeCmdList(const QStringList &cmdlist)
{
	if (cmdlist.length() == 0) return;

	QJsonDocument first;
	if (_isprompt && _pendingcmds.size() == 0)
	{
		QJsonObject cmddata;
		cmddata["type"] = "cmd";
		cmddata["cmd"] = cmdlist.front();
		first = QJsonDocument(cmddata);
	}

	bool is_first = true;
	for (auto &c : cmdlist)
	{
		if (is_first) {
			is_first = false;
			if (!first.isNull()) {
				continue;
			}
		}

		QJsonObject cmddata;
		cmddata["type"] = "cmd";
		cmddata["cmd"] = c;
		_pendingcmds.append(QJsonDocument(cmddata));
	}

	if (!first.isNull())
	{
		writeRawCommand(first);
	}
}

void Process::writeCmdDirect(const QString &cmd)
{
	QJsonObject cmddata;
	cmddata["type"] = "cmd";
	cmddata["cmd"] = cmd;

	writeRawCommand(QJsonDocument(cmddata));
}

void Process::writeCodeCompletion(const QString &cmd)
{
	QJsonObject cmddata;
	cmddata["type"] = "code_completion";
	cmddata["cmd"] = cmd;

	if (_isprompt)
		writeRawCommand(QJsonDocument(cmddata));
	else
		_pendingcmds.append(QJsonDocument(cmddata));
}

void Process::run()
{
	_pendingstartupcmds = _startupcmds;
	start(_program, _args);
}

void Process::quit()
{
	writeCmdDirect("#msh quit");
	QProcess::waitForFinished(100);
	if (QProcess::state() == QProcess::Running) {
		QProcess::terminate();
	}
	QProcess::waitForFinished(1000);
	if (QProcess::state() == QProcess::Running) {
		QProcess::kill();
	}
	QProcess::waitForFinished(-1);
}

void Process::restart()
{
	if (state() != NotRunning)
	{
		_isrestart = true;
		quit();
	}
	else
	{
		run();
	}
}


void Process::internalReadStdout()
{
	while (canReadLine()) {
		QString line = QString::fromLocal8Bit(readLine());
		emit onStandardOutput(line);
		parseLine(line);
	}
}

void Process::internalReadStderr()
{
	while (canReadLine()) {
		QString line = QString::fromLocal8Bit(readLine());
		emit onStandardError(line);
		parseLine(line);
	}
}

void Process::internalFinished(int exitCode, QProcess::ExitStatus exitstatus)
{
	if (_isrestart)
	{
		_isrestart = false;
		QTimer::singleShot(0, this, &Process::run);
	}
}

void Process::internalProcError(QProcess::ProcessError error)
{
	if (_isrestart && error == QProcess::Crashed)
	{
		return;
	}

	QString msg;
	switch (error)
	{
	case QProcess::FailedToStart:
		msg = "Process failed to start";
		break;
	case QProcess::Crashed:
		msg = "Process crashed";
		break;
	case QProcess::Timedout:
		msg = "Process timed out";
		break;
	case QProcess::ReadError:
		msg = "Process read error";
		break;
	case QProcess::WriteError:
		msg = "Process write error";
		break;
	case QProcess::UnknownError:
		msg = "Process unknown error";
		break;
	}
	emit onError(msg);
}

// Parses a line of metashell output
void Process::parseLine(const QString &line)
{
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &err);
	if (doc.isNull()) {
		emit onError(QString("JSON parse error: %1 [%2]").arg(err.errorString()).arg(line));
		return;
	}

	emit onCommand(doc);

	if (doc.object()["type"] == "error")
	{
		// cancel all pending commands if error
		_pendingcmds.clear();
	}

	if (doc.object()["type"] == "prompt")
	{
		if (!_pendingstartupcmds.isEmpty())
		{
			QString scmd = _pendingstartupcmds.first();
			_pendingstartupcmds.removeFirst();
			writeCmdDirect(scmd);
			return;
		}

		if (!_pendingcmds.isEmpty())
		{
			QJsonDocument scmd = _pendingcmds.first();
			_pendingcmds.removeFirst();
			writeRawCommand(scmd);
			return;
		}

		_isprompt = true;
		emit onPrompt(doc);
	}
}

}