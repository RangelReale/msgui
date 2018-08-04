#include "msgui/Project.h"

#include <QException>

#include <stdexcept>

namespace msgui {

Project::Project(QObject *parent) : 
	QObject(parent), _modified(false), _startupCodes(),  _includePaths(), _compilerflags(), _codehighlight()
{
	setDefaults();
}

Project::~Project()
{

}

bool Project::isModified() const
{
	return _modified;
}

void Project::setModified(bool value)
{
	_modified = value;
	emit onProjectChanged();
}

msglib::CheckStringList &Project::startupCodes()
{
	return _startupCodes;
}

QStringList &Project::includePaths()
{
	return _includePaths;
}

QStringList &Project::compilerFlags()
{
	return _compilerflags;
}

QList<ProjectCodeHighlight::ptr> &Project::codeHighlight()
{
	return _codehighlight;
}

void Project::clear()
{
	_startupCodes.clear();
	_includePaths.clear();
	_compilerflags.clear();
	_codehighlight.clear();
}

void Project::setDefaults()
{
	clear();

	_compilerflags << "-fcxx-exceptions" << "-std=c++14";
}


void Project::loadFromXml(const QDomDocument &doc)
{
	if (doc.documentElement().tagName() != "msgui") {
		throw std::runtime_error("Invalid project file");
	}

	clear();

	QDomElement e = doc.documentElement().firstChildElement();
	while (!e.isNull())
	{
		if (e.tagName() == "startupcodes")
		{
			QDomElement ie = e.firstChildElement();
			while (!ie.isNull())
			{
				if (ie.tagName() != "item")
					throw std::runtime_error("Invalid startupcode inside project file");

				auto sdata(std::make_shared<msglib::CheckStringListData>(ie.text()));
				if (ie.hasAttribute("enabled"))
				{
					sdata->checked = ie.attribute("enabled") != "false";
				}

				_startupCodes.append(sdata);
				ie = ie.nextSiblingElement();
			}
		}
		else if (e.tagName() == "codehighlight")
		{
			QDomElement ie = e.firstChildElement();
			while (!ie.isNull())
			{
				if (ie.tagName() != "item")
					throw std::runtime_error("Invalid startupcode inside project file");

				auto sdata(std::make_shared<ProjectCodeHighlight>());

				if (ie.hasAttribute("enabled"))
				{
					sdata->enabled = ie.attribute("enabled") != "false";
				}

				QDomElement iedata;
				iedata = ie.firstChildElement("regexp");
				if (!iedata.isNull())
					sdata->regexp = iedata.text();
				iedata = ie.firstChildElement("fgcolor");
				if (!iedata.isNull() && !iedata.text().trimmed().isNull())
					sdata->fgcolor = QColor(iedata.text());
				iedata = ie.firstChildElement("bgcolor");
				if (!iedata.isNull() && !iedata.text().trimmed().isNull())
					sdata->bgcolor = QColor(iedata.text());
				iedata = ie.firstChildElement("bold");
				if (!iedata.isNull())
					sdata->bold = iedata.text() == "true";

				_codehighlight.append(sdata);
				ie = ie.nextSiblingElement();
			}
		}
		else if (e.tagName() == "includepaths")
		{
			QDomElement ie = e.firstChildElement();
			while (!ie.isNull())
			{
				if (ie.tagName() != "item")
					throw std::runtime_error("Invalid includepaths inside project file");

				_includePaths.append(ie.text());
				ie = ie.nextSiblingElement();
			}
		}
		else if (e.tagName() == "compilerflags")
		{
			QDomElement ie = e.firstChildElement();
			while (!ie.isNull())
			{
				if (ie.tagName() != "item")
					throw std::runtime_error("Invalid compilerflags inside project file");

				_compilerflags.append(ie.text());
				ie = ie.nextSiblingElement();
			}
		}

		e = e.nextSiblingElement();
	}
}

void Project::saveToXml(QDomDocument &doc)
{
	QDomNode xmltag(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
	doc.appendChild(xmltag);

	QDomElement root = doc.createElement("msgui");
	doc.appendChild(root);

	if (!_startupCodes.isEmpty()) {
		QDomElement eStartupCode = doc.createElement("startupcodes");
		root.appendChild(eStartupCode);
		for (auto &i : _startupCodes) {
			QDomElement eItem = doc.createElement("item");
			if (!i->checked) {
				eItem.setAttribute("enabled", "false");
			}
			eItem.appendChild(doc.createCDATASection(i->str));
			eStartupCode.appendChild(eItem);
		}
	}
	if (!_codehighlight.isEmpty()) {
		QDomElement eCodeHighlight = doc.createElement("codehighlight");
		root.appendChild(eCodeHighlight);
		for (auto &i : _codehighlight) {
			QDomElement eItem = doc.createElement("item");
			if (!i->enabled) {
				eItem.setAttribute("enabled", "false");
			}

			QDomElement iRegExp = doc.createElement("regexp");
			iRegExp.appendChild(doc.createCDATASection(i->regexp));
			eItem.appendChild(iRegExp);

			if (i->fgcolor)
			{
				QDomElement iColor = doc.createElement("fgcolor");
				iColor.appendChild(doc.createCDATASection(i->fgcolor.value().name(QColor::HexRgb)));
				eItem.appendChild(iColor);
			}
			if (i->bgcolor)
			{
				QDomElement iColor = doc.createElement("bgcolor");
				iColor.appendChild(doc.createCDATASection(i->bgcolor.value().name(QColor::HexRgb)));
				eItem.appendChild(iColor);
			}

			QDomElement iBold = doc.createElement("bold");
			iBold.appendChild(doc.createTextNode(i->bold ? "true" : "false"));
			eItem.appendChild(iBold);

			eCodeHighlight.appendChild(eItem);
		}
	}
	if (!_includePaths.isEmpty()) {
		QDomElement eInclude = doc.createElement("includepaths");
		root.appendChild(eInclude);
		for (auto &i : _includePaths) {
			QDomElement eItem = doc.createElement("item");
			eItem.appendChild(doc.createCDATASection(i));
			eInclude.appendChild(eItem);
		}
	}
	if (!_compilerflags.isEmpty()) {
		QDomElement eCompilerFlags = doc.createElement("compilerflags");
		root.appendChild(eCompilerFlags);
		for (auto &i : _compilerflags) {
			QDomElement eItem = doc.createElement("item");
			eItem.appendChild(doc.createCDATASection(i));
			eCompilerFlags.appendChild(eItem);
		}
	}
}

}