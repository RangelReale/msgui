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

	ProjectCodeHighlight() : enabled(true), regexp(), fgcolor(), bgcolor(), bold(false) {}
	ProjectCodeHighlight(bool checked, const QString &regexp, tl::optional<QColor> fgcolor, tl::optional<QColor> bgcolor, bool bold) : 
		enabled(checked), regexp(regexp), fgcolor(fgcolor), bgcolor(bgcolor), bold(bold) {}

	bool enabled;
	QString regexp;
	tl::optional<QColor> fgcolor;
	tl::optional<QColor> bgcolor;
	bool bold;
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
	QList<ProjectCodeHighlight::ptr> &codeHighlight();

	// serialization
	void loadFromXml(const QDomDocument &doc);
	void saveToXml(QDomDocument &doc);
signals:
	void onProjectChanged();
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