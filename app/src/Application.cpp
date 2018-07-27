#include "msgui/Application.h"

#include <QMessageBox>

namespace msgui {

bool Application::notify(QObject *receiver, QEvent *event)
{
	try
	{
		return QApplication::notify(receiver, event);
	}
	catch (std::exception &e)
	{
		QString msg;
		msg.append("Exception thrown : ");
		msg.append(e.what());
		QMessageBox::critical(0, "Error", msg);
		QMessageBox::critical(0, "Object", receiver->objectName());
	}
	return false;
}

}