#pragma once

#include <msglib/CheckStringList.h>

#include <QObject>
#include <QStringList>
#include <QDomDocument>

namespace msgui {

class Project : public QObject
{
	Q_OBJECT
public:
	Project(QObject *parent);
	~Project();

	bool isModified() const;
	void setModified(bool value = true);

	msglib::CheckStringList &startupCodes();
	QStringList &includePaths();
	QStringList &compilerFlags();

	// serialization
	void loadFromXml(const QDomDocument &doc);
	void saveToXml(QDomDocument &doc);
private:
	void clear();
	void setDefaults();

	bool _modified;

	msglib::CheckStringList _startupCodes;
	QStringList _includePaths;
	QStringList _compilerflags;
};

}