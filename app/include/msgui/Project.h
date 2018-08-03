#pragma once

#include <msglib/CheckStringList.h>

#include <QObject>
#include <QStringList>
#include <QDomDocument>
#include <QColor>
#include <QFont>

#include <tl/optional.hpp>

namespace msgui {

class ProjectCodeHighlight
{
public:
	typedef std::shared_ptr<ProjectCodeHighlight> ptr;

	ProjectCodeHighlight() : text(), fgcolor(), bgcolor() {}

	QString text;
	tl::optional<QColor> fgcolor;
	tl::optional<QColor> bgcolor;
};

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
	QList<ProjectCodeHighlight::ptr> codeHighlight();

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
	QList<ProjectCodeHighlight::ptr> _codehighlight;
};

}