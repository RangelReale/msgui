#pragma once

#include <QString>

namespace msgui {
namespace itf {

class Configuration
{
public:
	virtual ~Configuration() {}
	virtual QString identCPPType(const QString &type) = 0;
};

} }