#pragma once

#include <msglib/Cmd.h>

#include <QJsonDocument>

#include <memory>
#include <vector>

namespace msglib {
namespace cmd {

//
// parser
//
class parser
{
public:
	parser();

	base::ptr parse(QJsonDocument doc);
private:
	QString cleanString(QString str);
};

//
// list
//
class list_
{
public:
	using ptr = std::shared_ptr<list_>;

	list_();

	std::vector<base::ptr> list;
};

} }