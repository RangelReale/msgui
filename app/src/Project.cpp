#include "msgui/Project.h"

#include <QException>

#include <stdexcept>

namespace msgui {

Project::Project(QObject *parent) : 
	QObject(parent), _modified(false), _startupCodes(),  _includePaths(), _compilerflags()
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

void Project::clear()
{
	_startupCodes.clear();
	_includePaths.clear();
	_compilerflags.clear();
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