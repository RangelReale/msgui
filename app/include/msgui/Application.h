#pragma once

#include <QApplication>

namespace msgui {

class Application : public QApplication
{
public:
	using QApplication::QApplication;

	bool notify(QObject *receiver, QEvent *event) override;
};

}