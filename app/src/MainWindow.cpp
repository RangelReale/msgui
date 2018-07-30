#include "msgui/MainWindow.h"
#include "msgui/Settings.h"
#include "msgui/ProjectSettings.h"
#include "msgui/LogWindow.h"

#include <msgwidget/highlighter/HL_CPP.h>

#include <mredit/margin/MarginStacker.h>
#include <mredit/Label.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QBoxLayout>
#include <QTimer>
#include <QHeaderView>
#include <QJsonObject>
#include <QDockWidget>
#include <QSplitter>
#include <QCryptographicHash>

#include "version.h"

namespace msgui {

MainWindow::MainWindow(const QString &filename) : 
	QMainWindow(), _project(nullptr), _process(nullptr), _cmdmode(cmdmode_t::inactive), _cmdparser()
{
	_logconfig = new LogConfig(this);

	_windowtitle = "MSGUI - Metashell GUI";
	setWindowTitle(_windowtitle);

	createActions();
	createStatusBar();
	createWidgets();

	initLog();
	logger()->logger("app")->info(QString("MSGUI version %1").arg(QString::fromLocal8Bit(VER_FILEVERSION_STR)));

	readSettings();

	setUnifiedTitleAndToolBarOnMac(true);

	setEnabledCmd(false);

	if (filename.isEmpty())
	{
		// create empty file
		newFile();
	}
	else
	{
		loadFile(filename);
	}
}

MainWindow::~MainWindow()
{
	if (_process->processState() == QProcess::Running) {
		_process->quit();
	}

	if (LogWindow::instance())
	{
		LogWindow::instance()->close();
	}

	closeProject();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) 
	{
		writeSettings();
		event->accept();
	}
	else 
	{
		event->ignore();
	}
}

void MainWindow::newFile()
{
	if (maybeSave()) 
	{
		delete _project;
		_project = new Project(this);
		setCurrentFile(QString());

		loadProject();
	}
}

void MainWindow::open()
{
	if (maybeSave()) 
	{
		QString fileName = QFileDialog::getOpenFileName(this, "Load file", QString(), "MSGUI Project;*.msgp");
		if (!fileName.isEmpty()) 
		{
			loadFile(fileName);
		}
	}
}

bool MainWindow::save()
{
	if (curFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}

bool MainWindow::saveAs()
{
	if (!_project)
		return false;

	QFileDialog dialog(this, "Save as", QString(), "MSGUI Project;*.msgp");
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() != QDialog::Accepted)
		return false;
	return saveFile(dialog.selectedFiles().first());
}

void MainWindow::documentWasModified()
{
	if (_project)
		setWindowModified(_project->isModified());
}

void MainWindow::createActions()
{
	// File
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QToolBar *fileToolBar = addToolBar(tr("File"));
	fileToolBar->setObjectName("file_toolbar");

	QAction *fileNewMenu = fileMenu->addAction(QIcon(":/document-new.png"), tr("&New"), this, &MainWindow::newFile);
	QAction *fileLoadMenu = fileMenu->addAction(QIcon(":/document-open.png"), tr("&Load..."), this, &MainWindow::open);
	QAction *fileSaveMenu = fileMenu->addAction(QIcon(":/document-save.png"), tr("&Save"), this, &MainWindow::save);
	QAction *fileSaveAsMenu = fileMenu->addAction(QIcon(":/document-save-as.png"), tr("Save &as..."), this, &MainWindow::saveAs);
	fileMenu->addSeparator();
	QAction *fileSettingsMenu = fileMenu->addAction(QIcon(":/document-properties.png"), tr("&Settings..."), this, &MainWindow::menuFileSettings);
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Quit"), this, &MainWindow::close);

	recentFileSeparator = fileMenu->addSeparator();

	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = fileMenu->addAction(QIcon(":/document-new.png"), "", this, &MainWindow::menuFileRecent);
		recentFileActs[i]->setVisible(false);
	}

	fileToolBar->addAction(fileNewMenu);
	fileToolBar->addAction(fileLoadMenu);
	fileToolBar->addAction(fileSaveMenu);
	fileToolBar->addAction(fileSaveAsMenu);

	// View
	QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
	_viewWindowMenu = viewMenu->addMenu(tr("&Window"));

	// Project
	QMenu *projectMenu = menuBar()->addMenu(tr("&Project"));
	QToolBar *projectToolBar = addToolBar(tr("Project"));
	projectToolBar->setObjectName("project_toolbar");

	QAction *projectSettingsMenu = projectMenu->addAction(QIcon(":/story-editor.png"), tr("&Settings"), this, &MainWindow::menuProjectSettings);
    projectSettingsMenu->setMenuRole(QAction::NoRole);

	projectSettingsMenu->setShortcut(QKeySequence(Qt::Key_F4));

	projectToolBar->addAction(projectSettingsMenu);

	// Debug
	QMenu *debugMenu = menuBar()->addMenu(tr("&Debug"));
	QToolBar *debugToolBar = addToolBar(tr("Debug"));
	debugToolBar->setObjectName("debug_toolbar");

	_debugStartMenu = debugMenu->addAction(QIcon(":/arrow-right.png"), tr("&Start/Restart"), this, &MainWindow::menuDebugStart);
	_debugStopMenu = debugMenu->addAction(QIcon(":/process-stop.png"), tr("S&top"), this, &MainWindow::menuDebugStop);
	_debugContinueMenu = debugMenu->addAction(QIcon(":/debug-run.png"), tr("&Continue"), this, &MainWindow::menuDebugContinue);
	debugMenu->addSeparator();
	_debugStepIntoMenu = debugMenu->addAction(QIcon(":/debug-step-into.png"), tr("Step &into"), this, &MainWindow::menuDebugStepInto);
	_debugStepOverMenu = debugMenu->addAction(QIcon(":/debug-step-over.png"), tr("Step &over"), this, &MainWindow::menuDebugStepOver);
	_debugStepOutMenu = debugMenu->addAction(QIcon(":/debug-step-out.png"), tr("Step o&ut"), this, &MainWindow::menuDebugStepOut);
	_debugStepIntoBackMenu = debugMenu->addAction(QIcon(":/debug-step-into.png"), tr("Step into back"), this, &MainWindow::menuDebugStepIntoBack);
	_debugStepOverBackMenu = debugMenu->addAction(QIcon(":/debug-step-over.png"), tr("Step over back"), this, &MainWindow::menuDebugStepOverBack);
	debugMenu->addSeparator();
	_debugBacktracetMenu = debugMenu->addAction(QIcon(":/go-previous.png"), tr("&Backtrace"), this, &MainWindow::menuDebugBacktrace);
	_debugForwardtracetMenu = debugMenu->addAction(QIcon(":/go-next.png"), tr("&Forward trace"), this, &MainWindow::menuDebugForwardtrace);

	debugMenu->addSeparator();
    QMenu *debugMenuOptions = new QMenu(tr("&Options"));
    QAction *actDebugMenuOptions = debugMenu->addMenu(debugMenuOptions);
    actDebugMenuOptions->setMenuRole(QAction::NoRole);

	_debugStepOnStart = debugMenuOptions->addAction(tr("Ste&p on start"), this, &MainWindow::menuDebugStepOnStart);
	_debugStepOnStart->setCheckable(true);
	_debugForwardtraceOnStart = debugMenuOptions->addAction(tr("For&ward trace on start"), this, &MainWindow::menuDebugStepOnStart);
	_debugForwardtraceOnStart->setCheckable(true);


	_debugStartMenu->setShortcut(QKeySequence(Qt::Key_F5));
	_debugStopMenu->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	_debugContinueMenu->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	_debugStepIntoMenu->setShortcut(QKeySequence(Qt::Key_F11));
	_debugStepOverMenu->setShortcut(QKeySequence(Qt::Key_F10));
	_debugStepOutMenu->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F11));
	_debugStepIntoBackMenu->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F11));
	_debugStepOverBackMenu->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F10));

	_debugForwardtracetMenu->setShortcut(QKeySequence(Qt::Key_F7));

	debugToolBar->addAction(_debugStartMenu);
	debugToolBar->addAction(_debugStopMenu);
	debugToolBar->addAction(_debugContinueMenu);
	debugToolBar->addSeparator();
	debugToolBar->addAction(_debugStepIntoMenu);
	debugToolBar->addAction(_debugStepOverMenu);
	debugToolBar->addAction(_debugStepOutMenu);
	debugToolBar->addSeparator();
	debugToolBar->addAction(_debugBacktracetMenu);
	debugToolBar->addAction(_debugForwardtracetMenu);

	// Information
	QMenu *informationMenu = menuBar()->addMenu(tr("&Information"));
	QToolBar *informationToolBar = addToolBar(tr("Information"));
	informationToolBar->setObjectName("information_toolbar");

	_informationEnvironmentMenu = informationMenu->addAction(QIcon(":/games-config-background.png"), tr("&Environment"), this, &MainWindow::menuInformationEnvironment);
	_informationEnvironmentReloadMenu = informationMenu->addAction(QIcon(":/view-refresh.png"), tr("&Reload environment"), this, &MainWindow::menuInformationEnvironmentReload);
	_informationEnvironmentSysincludesMenu = informationMenu->addAction(tr("&System includes"), this, &MainWindow::menuInformationEnvironmentSysincludes);
	_informationEnvironmentMacroNamesMenu = informationMenu->addAction(tr("Macro &names"), this, &MainWindow::menuInformationEnvironmentMacroNames);
	_informationEnvironmentMacrosMenu = informationMenu->addAction(tr("&Macros"), this, &MainWindow::menuInformationEnvironmentMacros);

	informationToolBar->addAction(_informationEnvironmentMenu);
	informationToolBar->addAction(_informationEnvironmentReloadMenu);

	// Help
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	QToolBar *helpToolBar = addToolBar(tr("Help"));
	helpToolBar->setObjectName("help_toolbar");

	QAction *helpLogMenu = helpMenu->addAction(QIcon(":/view-calendar-list.png"), tr("&Log"), this, &MainWindow::menuHelpLog);
	helpMenu->addSeparator();
	QAction *helpAboutMenu = helpMenu->addAction(QIcon(":/help-about.png"), tr("&About"), this, &MainWindow::menuHelpAbout);

	helpToolBar->addAction(helpLogMenu);
	helpToolBar->addAction(helpAboutMenu);

	updateRecentFileActions();
}

void MainWindow::createStatusBar()
{
	_st_cmdmode = new QLabel("Command mode", statusBar());
	_st_process = new QLabel("Process", statusBar());

	_st_cmdmode->setMinimumWidth(150);
	_st_process->setMinimumWidth(120);

	_st_cmdmode->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	_st_process->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	statusBar()->addWidget(_st_cmdmode);
	statusBar()->addWidget(_st_process);
}

void MainWindow::createDockedWidgets()
{
	// dock windows

	//
	// right docks
	//

	// dock: log
	QDockWidget *dk_log = new QDockWidget("Log", this);
	dk_log->setObjectName("dock_log");

	_log = new Log(dk_log);

	dk_log->setWidget(_log);
	addDockWidget(Qt::RightDockWidgetArea, dk_log);

	_viewWindowMenu->addAction(dk_log->toggleViewAction());

	// dock: call graph
	QDockWidget *dk_callgraph = new QDockWidget("Forward trace", this);
	dk_callgraph->setObjectName("dock_callgraph");

	_callgraph = new Callgraph(dk_callgraph);
	connect(_callgraph, &Callgraph::showFileAndLine, this, &MainWindow::showFileAndLine, Qt::QueuedConnection);

	dk_callgraph->setWidget(_callgraph);
	addDockWidget(Qt::RightDockWidgetArea, dk_callgraph);

	_viewWindowMenu->addAction(dk_callgraph->toggleViewAction());

	// default docks size
	resizeDocks({ dk_log, dk_callgraph }, { 600, 600 }, Qt::Horizontal);
}

void MainWindow::createWidgets()
{
	createDockedWidgets();

	// root
	QWidget *root = new QWidget(this);
	setCentralWidget(root);
	QVBoxLayout *rootlayout = new QVBoxLayout;
	root->setLayout(rootlayout);

	// body

	// body: command
	QHBoxLayout *cmdlayout = new QHBoxLayout;

	_cmdprompt = new QLabel(root);
	cmdlayout->addWidget(_cmdprompt);

	_cmd = new HistoryLineEdit(root);
	connect(_cmd, &HistoryLineEdit::lineExecuted, this, &MainWindow::cmdExecute);
	cmdlayout->addWidget(_cmd);

	// body: error
	_error = new Error(root);
	_error->setVisible(false);
	connect(_error, &Error::showFileAndLine, this, &MainWindow::showFileAndLine, Qt::QueuedConnection);

	// body: frame
	_frame = new Frame(nullptr, root);
	_frame->setVisible(false);
	connect(_frame, &Frame::showFileAndLine, this, &MainWindow::showFileAndLine, Qt::QueuedConnection);

	// body: backtrace
	_backtrace = new Backtrace(root);
	connect(_backtrace, &Backtrace::showFileAndLine, this, &MainWindow::showFileAndLine, Qt::QueuedConnection);
	
	// body: editor
	_editor = new mredit::Editor(root);
	_editor->setReadOnly(true);

	new msgwidget::highlighter::HL_CPP(_editor->document());

	_editor->marginStacker()->setVisible(mredit::Global::Margin::BookmarkMargin, true);
	_editor->marginStacker()->setVisible(mredit::Global::Margin::NumberMargin, true);
	_editor->marginStacker()->setVisible(mredit::Global::Margin::SpaceMargin, true);

	// create a bookmark group to show current line position with right arrow and yellow background
	_editor_bmgroup_showpos = _editor->bookmarkGroups().addGroup("Position", QIcon(":/arrow-right.png"));
	QTextBlockFormat spformat;
	spformat.setBackground(QColor(Qt::yellow));
	_editor->bookmarkGroups().getGroup(_editor_bmgroup_showpos)->setTextBlockFormat(spformat);

	QSplitter *splitter1 = new QSplitter(root);
	splitter1->setOrientation(Qt::Vertical);
	splitter1->addWidget(_backtrace);
	splitter1->addWidget(_editor);

	splitter1->setStretchFactor(0, 3);
	splitter1->setStretchFactor(1, 10);

	rootlayout->setSizeConstraint(QLayout::SetMinimumSize);
	rootlayout->addLayout(cmdlayout, 1);
	rootlayout->addWidget(_error, 2);
	rootlayout->addWidget(_frame, 1);
	rootlayout->addWidget(splitter1, 13);
}


void MainWindow::readSettings()
{
	QSettings settings;

	_debugStepOnStart->setChecked(settings.value("dbg_steponstart", true).toBool());
	_debugForwardtraceOnStart->setChecked(settings.value("dbg_forwardtraceonstart", true).toBool());

	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	if (geometry.isEmpty()) {
		setWindowState(Qt::WindowMaximized);
	}
	else {
		restoreGeometry(geometry);
	}
	const QByteArray windowState = settings.value("windowState", QByteArray()).toByteArray();
	if (!windowState.isEmpty()) {
		restoreState(windowState);
	}

	_program = settings.value("metashell_program", "").toString();
	if (_program.isEmpty()) {
		_program = Settings::findMetashellExecutable();
	}
}

void MainWindow::writeSettings()
{
	QSettings settings;

	settings.setValue("dbg_steponstart", _debugStepOnStart->isChecked());
	settings.setValue("dbg_forwardtraceonstart", _debugForwardtraceOnStart->isChecked());

	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
}

void MainWindow::readProjectSettings()
{
	if (curFile.isEmpty())
		return;

	QSettings settings;
	QString chkey = QString(QCryptographicHash::hash(curFile.toUtf8(), QCryptographicHash::Md5).toHex());
	_cmd->setHistory(settings.value(QString("cmd_prj_history/%1").arg(chkey)).toStringList());
}

void MainWindow::writeProjectSettings()
{
	if (curFile.isEmpty())
		return;

	QSettings settings;
	QString chkey = QString(QCryptographicHash::hash(curFile.toUtf8(), QCryptographicHash::Md5).toHex());
	settings.setValue(QString("cmd_prj_history/%1").arg(chkey), _cmd->history());
}


bool MainWindow::maybeSave()
{
	if (!_project || !_project->isModified())
		return true;
	const QMessageBox::StandardButton ret = QMessageBox::warning(this, qApp->applicationDisplayName(),
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	switch (ret) {
	case QMessageBox::Save:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}
	return true;
}

void MainWindow::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName), file.errorString()));
		return;
	}

	QTextStream in(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	if (_project)
		delete _project;
	_project = new Project(this);

	QDomDocument doc;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	_project->loadFromXml(doc);

	file.close();
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
	loadProject();
	statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
	if (!_project)
		return false;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName),
				file.errorString()));
		return false;
	}

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	QDomDocument xml;
	_project->saveToXml(xml);
	QTextStream output(&file);
	output << xml.toString();
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	// write previous file settings
	writeProjectSettings();

	curFile = fileName;
	_project->setModified(false);
	setWindowModified(false);

	QString shownName = curFile;
	if (curFile.isEmpty())
		shownName = "untitled.msgp";
	setWindowTitle(QString("%1 - %2").arg(_windowtitle).arg(shownName));

	if (!curFile.isEmpty())
	{
		QSettings settings;
		QStringList files = settings.value("recentFileList").toStringList();
		files.removeAll(fileName);
		files.prepend(fileName);
		while (files.size() > MaxRecentFiles)
			files.removeLast();

		settings.setValue("recentFileList", files);
		updateRecentFileActions();
	}

	// read current project file settings
	readProjectSettings();
}

void MainWindow::updateRecentFileActions()
{
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll("");

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

	recentFileSeparator->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::setEnabledCmd(bool enabled)
{
	if (enabled != _cmd->isEnabled())
	{
		_cmd->setEnabled(enabled);
		if (enabled) {
			QApplication::restoreOverrideCursor();
		} 
		else {
			if (_cmdmode != cmdmode_t::inactive) {
				QApplication::setOverrideCursor(Qt::WaitCursor);
			}
		}

		updateEnabled();
	}
}

void MainWindow::setCmdMode(cmdmode_t mode)
{
	if (mode == cmdmode_t::inactive || mode != _cmdmode)
	{
		_cmdmode = mode;

		switch (mode)
		{
		case cmdmode_t::inactive:
			_st_cmdmode->setText("Inactive");
			break;
		case cmdmode_t::immediate:
			_st_cmdmode->setText("Immediate mode");
			break;
		case cmdmode_t::metadebug:
			_st_cmdmode->setText("Meta debugger");
			break;
		case cmdmode_t::preprocessordebug:
			_st_cmdmode->setText("Preprocessor debugger");
			break;
		}

		updateEnabled();
	}
}

void MainWindow::updateEnabled()
{
	bool is_inactive = !_cmd->isEnabled() || _cmdmode == cmdmode_t::inactive;
	bool is_debug = _cmdmode == cmdmode_t::metadebug || _cmdmode == cmdmode_t::preprocessordebug;
	bool is_debug_and_active = !is_inactive && is_debug;

	_debugStartMenu->setEnabled(!is_inactive);
	_debugStopMenu->setEnabled(is_debug_and_active);
	_debugContinueMenu->setEnabled(is_debug_and_active);
	_debugStepIntoMenu->setEnabled(is_debug_and_active);
	_debugStepOverMenu->setEnabled(is_debug_and_active);
	_debugStepOutMenu->setEnabled(is_debug_and_active);
	_debugStepIntoBackMenu->setEnabled(is_debug_and_active);
	_debugStepOverBackMenu->setEnabled(is_debug_and_active);
	_debugBacktracetMenu->setEnabled(is_debug_and_active);
	_debugForwardtracetMenu->setEnabled(is_debug_and_active);

	_informationEnvironmentMenu->setEnabled(!is_debug_and_active);
	_informationEnvironmentReloadMenu->setEnabled(!is_debug_and_active);
	_informationEnvironmentSysincludesMenu->setEnabled(!is_debug_and_active);
	_informationEnvironmentMacroNamesMenu->setEnabled(!is_debug_and_active);
	_informationEnvironmentMacrosMenu->setEnabled(!is_debug_and_active);

	_backtrace->setVisible(is_debug);
}

void MainWindow::initLog()
{
	for (auto &log : LogConfig::instance()->list()) {
		onLog(log);
	}
	connect(LogConfig::instance(), &LogConfig::onLog, this, &MainWindow::onLog);
}

MainWindow::SFileAndLine MainWindow::parseFileAndLine(const QString &fileAndLine)
{
	SFileAndLine ret;
	ret.filename = fileAndLine;
	ret.row = -1;
	ret.col = -1;
	// col
	int sep = ret.filename.lastIndexOf(":");
	if (sep > -1) {
		ret.col = ret.filename.mid(sep + 1).toInt() - 1;
		ret.filename = ret.filename.mid(0, sep);
	}
	// row
	sep = ret.filename.lastIndexOf(":");
	if (sep > -1) {
		ret.row = ret.filename.mid(sep + 1).toInt() - 1;
		ret.filename = ret.filename.mid(0, sep);
	}
	return ret;
}

void MainWindow::processCommandClass(msglib::cmd::base::ptr cc)
{
	if (cc) {
		if (auto c = std::dynamic_pointer_cast<msglib::cmd::backtrace>(cc)) {
			showBacktrace(c);
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::call_graph>(cc)) {
			showCallgraph(c);
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::code_completion_result>(cc)) {
			logger()->logger("code completion")->debug(c->completions.join(", "));
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::comment>(cc)) {
			for (auto &comment : c->paragraphs) {
				logger()->logger("comment")->info(comment->content);
			}
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::cpp_code>(cc)) {
			showCode(c->code);
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::error>(cc)) {
			logger()->logger("error")->error(c-> message);
			_error->setMessage(c->message);
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::filename_list>(cc)) {
			showFilenameList(c);
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::filename_set>(cc)) {
			showFilenameList(c);
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::frame>(cc)) {
			showFrame(c);
			if (!c->source_location.isEmpty()) {
				showFileAndLine(c->source_location);
			}
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::prompt>(cc)) {
			// handled in other event
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::raw_text>(cc)) {
			if (!c->text.isEmpty()) {
				logger()->logger("raw_text")->info(c->text);
			}
		}
		else if (auto c = std::dynamic_pointer_cast<msglib::cmd::type_>(cc)) {
			showFrame(c);
		}
	}
}

void MainWindow::closeProject()
{
	writeProjectSettings();
}

void MainWindow::loadProject()
{
	QTimer::singleShot(0, this, &MainWindow::processRestart);
}

void MainWindow::cmdExecute(QString cmd)
{
	if (_process)
	{
		_process->writeCmd(cmd);
	}
}

void MainWindow::onLog(const Log4Qt::LoggingEvent &event)
{
	if (event.loggename() != "Qt" && event.level() <= Log4Qt::Level::DEBUG_INT) {
		return;
	}
	_log->addLog(event);
}

void MainWindow::showCode(const QString &code)
{
	_editor->clearBookmarks(_editor_bmgroup_showpos);
	_editor->setPlainText(code);
	_editor_current_file = "";
}

void MainWindow::showFileAndLine(const QString &fileAndFile)
{
	SFileAndLine fl = parseFileAndLine(fileAndFile);

	if (!fl.filename.isEmpty())
	{
		openSourceFile(fl.filename);
		markSourceFile(fl.row, fl.col);
	}
}

void MainWindow::showFrame(msglib::cmd::base::ptr frame)
{
	if (frame) {
		_frame->setFrame(frame);
		_frame->setVisible(true);
	}
	else
	{
		_frame->setVisible(false);
	}
}

void MainWindow::showBacktrace(msglib::cmd::backtrace::ptr backtrace)
{
	_backtrace->setBacktrace(backtrace);

	// when showing backtrace for the first time, it changes the editor size, and can sometimes make the cursor hidden
	QTimer::singleShot(500, this, &MainWindow::ensureSourceVisible);
}

void MainWindow::showCallgraph(msglib::cmd::call_graph::ptr callgraph)
{
	_callgraph->setCallgraph(callgraph);
}

void MainWindow::showFilenameList(msglib::cmd::filename_list_base::ptr filename_list)
{
	QString code;
	for (auto &f : filename_list->filenames) {
		code.append(f);
		code.append("\n");
	}

	_editor->clearBookmarks(_editor_bmgroup_showpos);
	_editor->setPlainText(code);
	_editor_current_file = "";
}

void MainWindow::openSourceFile(const QString &filename)
{
	//logger()->info("Open source file: %1", filename);

	if (_editor_current_file != filename)
	{
		_editor->openFile(filename);
		_editor_current_file = filename;
	}
}

void MainWindow::markSourceFile(int row, int col)
{
	//logger()->info("Mark source file: %1 %2", row, col);

	_editor->clearBookmarks(_editor_bmgroup_showpos);
	if (row >= 0)
	{
		_editor->setBookmark(_editor_bmgroup_showpos, row, true);
		_editor->ensureLineCenter(row, col);
	}
}

void MainWindow::ensureSourceVisible()
{
	_editor->ensureCursorVisible();
}

void MainWindow::processRun()
{
	if (_process && _process->state() == QProcess::NotRunning)
	{
		logger()->logger("process")->info("Starting metashell process: %1 %2", _process->program(), _process->args().join(" "));
		_process->run();
	}
}

void MainWindow::processRestart()
{
	if (_process && _process->state() == QProcess::Running) {
		logger()->logger("process")->info("Restarting process");
	}

	createProcess();
	processRun();
}

void MainWindow::processStateChanged(QProcess::ProcessState newState)
{
	switch (newState)
	{
	case QProcess::NotRunning:
		_st_process->setText("Metashell stopped");
		logger()->logger("process")->info("Metashell stopped");
		setCmdMode(cmdmode_t::inactive);
		setEnabledCmd(false);
		QApplication::restoreOverrideCursor();
		break;
	case QProcess::Running:
		_st_process->setText("Metashell running");
		logger()->logger("process")->info("Metashell process started");
		setCmdMode(cmdmode_t::immediate);
		break;
	case QProcess::Starting:
		_st_process->setText("Metashell starting");
		logger()->logger("process")->info("Metashell starting");
		setEnabledCmd(false);
		setCmdMode(cmdmode_t::inactive);
		break;
	}
}

void MainWindow::processSendCommand(const QJsonDocument &line)
{
	setEnabledCmd(false);
	_error->setMessage();

	logger()->logger("process")->trace("Sending command: %1", QString(line.toJson()));
}

void MainWindow::processStandardOutput(const QString &line)
{
	logger()->logger("process")->trace("Process response: %1", line);
}

void MainWindow::processStandardError(const QString &line)
{
	logger()->logger("process")->trace("Process response [stderr]: %1", line);
}

void MainWindow::processError(const QString &message)
{
	logger()->logger("process")->error("Process error: %1", message);
}

void MainWindow::processCommand(const QJsonDocument &cmd)
{
	processCommandClass(_cmdparser.parse(cmd));
}

void MainWindow::processPrompt(const QJsonDocument &cmd)
{
	QString prompt(cmd.object()["prompt"].toString());

	setEnabledCmd(true);
	_cmdprompt->setText(prompt);
	_cmd->setFocus();

	if (prompt == "(mdb)")
		setCmdMode(cmdmode_t::metadebug);
	else if (prompt == "(pdb)")
		setCmdMode(cmdmode_t::preprocessordebug);
	else
		setCmdMode(cmdmode_t::immediate);
}

//
// menus
//
void MainWindow::menuFileSettings()
{
	Settings settings(this);
	if (settings.exec()) {
		readSettings();
	}
}

void MainWindow::menuFileRecent()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}

void MainWindow::menuProjectSettings()
{
	if (!_project)
		return;

	ProjectSettings settings(_project, this);
	if (settings.exec()) {
		loadProject();

		processRestart();
	}
}

void MainWindow::menuInformationEnvironment()
{
	if (_process)
	{
		_process->writeCmd("#msh environment");
	}
}

void MainWindow::menuInformationEnvironmentReload()
{
	if (_process)
	{
		if (_process->state() == QProcess::NotRunning)
		{
			processRestart();
		}
		else
		{
			_process->writeCmd("#msh environment reload");
		}
	}
}

void MainWindow::menuInformationEnvironmentSysincludes()
{
	if (_process)
	{
		_process->writeCmd("#msh sysincludes");
	}
}

void MainWindow::menuInformationEnvironmentMacroNames()
{
	if (_process)
	{
		_process->writeCmd("#msh macro names");
	}
}

void MainWindow::menuInformationEnvironmentMacros()
{
	if (_process)
	{
		_process->writeCmd("#msh macros");
	}
}

void MainWindow::menuDebugStepOnStart()
{

}

void MainWindow::menuDebugForwardtraceOnStart()
{

}


void MainWindow::menuDebugStart()
{
	if (_cmdmode == cmdmode_t::metadebug || _cmdmode == cmdmode_t::preprocessordebug)
	{
		_process->writeCmd("evaluate");
	}
	else if (_cmd->text() != "")
	{
		QString dcmd(_cmd->text());
		if (!dcmd.startsWith("#msh mdb")) 
		{
			dcmd = QString("#msh mdb %1").arg(_cmd->text());
		}

		_cmd->simulateExecuteLine(_cmd->text());

		QStringList cmdlist;
		cmdlist << dcmd;
		if (_debugForwardtraceOnStart->isChecked()) {
			cmdlist << "forwardtrace";
		}
		if (_debugStepOnStart->isChecked()) {
			cmdlist << "step 1" << "backtrace";
		}

		_process->writeCmdList(cmdlist);
	}
}

void MainWindow::menuDebugStop()
{
	_process->writeCmd("quit");
}

void MainWindow::menuDebugContinue()
{
	_process->writeCmd("continue");
}

void MainWindow::menuDebugStepInto()
{
	_process->writeCmdList(QStringList() << "step 1" << "backtrace");
}

void MainWindow::menuDebugStepOver()
{
	_process->writeCmdList(QStringList() << "next" << "backtrace");
}

void MainWindow::menuDebugStepOut()
{
	_process->writeCmdList(QStringList() << "step out" << "backtrace");
}

void MainWindow::menuDebugStepIntoBack()
{
	_process->writeCmdList(QStringList() << "step -1" << "backtrace");
}

void MainWindow::menuDebugStepOverBack()
{
	_process->writeCmdList(QStringList() << "next -1" << "backtrace");
}


void MainWindow::menuDebugBacktrace()
{
	_process->writeCmd("backtrace");
}

void MainWindow::menuDebugForwardtrace()
{
	_process->writeCmd("forwardtrace");
}

void MainWindow::menuHelpLog()
{
	if (!LogWindow::instance())
	{
		new LogWindow;
		LogWindow::instance()->show();
	}
	else
	{
		LogWindow::instance()->raise();
	}
}

void MainWindow::menuHelpAbout()
{
	QMessageBox::about(this, 
		QString("MSGUI %1 - Metashell GUI").arg(QString::fromLocal8Bit(VER_FILEVERSION_STR)),
		"Copyright (c) 2018 Rangel Reale (rangelreale@gmail.com)");
}

void MainWindow::createProcess()
{
	if (_process)
	{
		_process->quit();
		delete _process;
		_process = nullptr;
	}

	if (_project)
	{
		_process = new msglib::Process(this);
		_process->setProgram(_program);
		_process->args() << "--console=json" << "--";

		for (auto &i : _project->includePaths()) {
			_process->args() << "-I" << i;
		}
		for (auto &i : _project->compilerFlags()) {
			_process->args() << i;
		}

		for (auto &i : _project->startupCodes()) {
			if (i->checked)
				_process->startupCmds() << i->str;
		}

		connect(_process, &msglib::Process::stateChanged, this, &MainWindow::processStateChanged);
		//connect(_process, &msglib::Process::errorOccurred, this, &MainWindow::processErrorOcurred);

		connect(_process, &msglib::Process::onSendCommand, this, &MainWindow::processSendCommand);
		connect(_process, &msglib::Process::onStandardOutput, this, &MainWindow::processStandardOutput);
		connect(_process, &msglib::Process::onStandardError, this, &MainWindow::processStandardError);
		connect(_process, &msglib::Process::onError, this, &MainWindow::processError);

		connect(_process, &msglib::Process::onCommand, this, &MainWindow::processCommand);
		connect(_process, &msglib::Process::onPrompt, this, &MainWindow::processPrompt);
	}
}

}
