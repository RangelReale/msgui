#pragma once

#include "msgwidget/highlighter/HL_CPP.h"

#include <QString>

namespace msgui {
namespace itf {

class Configuration
{
public:
	virtual ~Configuration() {}
	virtual QString identCPPType(const QString &type) = 0;
	virtual msgwidget::highlighter::HL_CPP *createCPPHighligher(QTextDocument *parent) = 0;
};

} }