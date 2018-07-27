#include "msgui/Application.h"
#include "msgui/MainWindow.h"
#include "msgui/LogConfig.h"

#include <QCommandLineParser>
#include <QCommandLineOption>

#include <log4qt/logger.h>
#include <log4qt/logmanager.h>
#include <log4qt/ttcclayout.h>

int main(int argc, char *argv[])
{
	msgui::Application app(argc, argv);
	app.setOrganizationName("Rangel Reale");
	app.setOrganizationDomain("rangelreale.com");
	app.setApplicationName("MSGUI");
	app.setWindowIcon(QIcon(":/msgui_icon.png"));

	// create logger
	auto logger = Log4Qt::Logger::rootLogger();
	logger->setLevel(Log4Qt::Level::ALL_INT);
	Log4Qt::LogManager::setHandleQtMessages(true);
	new msgui::LogConfig(&app);

	// parse command line
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);

	QString loadfile;
	if (!parser.positionalArguments().isEmpty())
		loadfile = parser.positionalArguments().first();

	msgui::MainWindow win(loadfile);
	win.show();

	return app.exec();
}