#include "msgui/LogConfig.h"

#include <iostream>

namespace msgui {

LogConfig *LogConfig::_instance = nullptr;
	
LogConfig::LogConfig(QObject *parent) : Log4Qt::AppenderSkeleton(parent), _MaxCount(200), _list()
{
	_instance = this;

	initialize();
}

LogConfig::~LogConfig()
{
	finalize();

	_instance = nullptr;
}

LogConfig* LogConfig::instance()
{
	return _instance;
}

void LogConfig::initialize()
{
	Log4Qt::Logger::rootLogger()->addAppender(this);
}

void LogConfig::finalize()
{
	Log4Qt::Logger::rootLogger()->removeAppender(this);
}

QList<Log4Qt::LoggingEvent> LogConfig::list() const
{
	QMutexLocker locker(&mObjectGuard);
	return _list;
}

void LogConfig::append(const Log4Qt::LoggingEvent &event)
{
	_list << event;
	while (_list.size() > _MaxCount) {
		_list.pop_front();
	}

	emit onLog(event);
}

}