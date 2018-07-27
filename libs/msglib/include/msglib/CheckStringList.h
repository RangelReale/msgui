#pragma once

#include <QList>
#include <QString>

#include <memory>

namespace msglib {

struct CheckStringListData
{
	using ptr = std::shared_ptr<CheckStringListData>;

	CheckStringListData(const QString &str = QString(), bool checked = true) : str(str), checked(checked) {}

	QString str;
	bool checked;
};

class CheckStringList : public QList<CheckStringListData::ptr>
{
public:
	CheckStringList() : QList<CheckStringListData::ptr>() {}
};

}