#pragma once

#include <QObject>
#include <QList>
#include <QDateTime>

#include <log4qt/appenderskeleton.h>

#include <memory>

namespace msgui {

class LogConfig : public Log4Qt::AppenderSkeleton
{
	Q_OBJECT
public:
	LogConfig(QObject *parent);
	~LogConfig();

	static LogConfig* instance();

	QList<Log4Qt::LoggingEvent> list() const;

	bool requiresLayout() const override;
signals:
	void onLog(const Log4Qt::LoggingEvent &);
protected:
	void append(const Log4Qt::LoggingEvent &event) override;
private:
	void initialize();
	void finalize();

	static LogConfig *_instance;
	int _MaxCount;
	QList<Log4Qt::LoggingEvent> _list;
};

inline bool LogConfig::requiresLayout() const
{
	return false;
}

}
