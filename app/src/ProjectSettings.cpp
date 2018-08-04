#include "msgui/ProjectSettings.h"
#include "msgui/CodeDialog.h"

#include <QSettings>
#include <QDir>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>

#define FILEDATA_ROLE 9588

namespace msgui {

ProjectSettings::ProjectSettings(itf::Configuration *configuration, Project *project, QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint), _configuration(configuration), _project(project)
{
	setWindowTitle("Project settings");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	// settings
	QFormLayout *form = new QFormLayout;
	layout->addLayout(form);

	// startup code
	_startupcodes = new msgwidget::UpDownListWidget(this);
	_startupcodes->showAddButton();
	_startupcodes->showEditButton();
	_startupcodes->showRemoveButton();
	connect(_startupcodes, &msgwidget::UpDownListWidget::addClicked, this, &ProjectSettings::btnStartupCodeAddClicked);
	connect(_startupcodes, &msgwidget::UpDownListWidget::editClicked, this, &ProjectSettings::btnStartupCodeEditClicked);
	connect(_startupcodes, &msgwidget::UpDownListWidget::removeClicked, this, &ProjectSettings::btnStartupCodeRemoveClicked);
	form->addRow(tr("Startup code"), _startupcodes);

	// metashell params
	_msparams = new QTabWidget(this);

	// includePaths
	_includepaths = new msgwidget::UpDownListWidget(nullptr);
	_includepaths->showAddButton();
	_includepaths->showRemoveButton();
	connect(_includepaths, &msgwidget::UpDownListWidget::addClicked, this, &ProjectSettings::btnIncludeAddClicked);
	connect(_includepaths, &msgwidget::UpDownListWidget::removeClicked, this, &ProjectSettings::btnIncludeRemoveClicked);
	//form->addRow(tr("Include paths"), _includepaths);

	// compilerflags
	_compilerflags = new msgwidget::UpDownListWidget(nullptr);
	_compilerflags->showAddButton();
	_compilerflags->showRemoveButton();
	connect(_compilerflags, &msgwidget::UpDownListWidget::addClicked, this, &ProjectSettings::btnCompilerFlagsAddClicked);
	connect(_compilerflags, &msgwidget::UpDownListWidget::removeClicked, this, &ProjectSettings::btnCompilerFlagsRemoveClicked);
	//form->addRow(tr("Compiler flags"), _compilerflags);

	_msparams->addTab(_includepaths, "Include paths");
	_msparams->addTab(_compilerflags, "Compiler flags");

	form->addRow(tr("Process params"), _msparams);

	// buttons
	QHBoxLayout *btnlayout = new QHBoxLayout;
	layout->addLayout(btnlayout);

	QPushButton *btnOk = new QPushButton(tr("Ok"), this);
	connect(btnOk, &QPushButton::clicked, this, &ProjectSettings::onBtnOk);
	QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
	connect(btnCancel, &QPushButton::clicked, this, &ProjectSettings::onBtnCancel);

	btnlayout->addWidget(btnOk);
	btnlayout->addWidget(btnCancel);

	// read settings
	readSettings();
}

QString ProjectSettings::firstLine(const QString &data)
{
	auto lines = data.split("\n");
	if (lines.length() > 0) {
		QString ret = lines[0].trimmed();
		if (lines.length() > 1)
			ret.append("      ...");
		return ret;
	}
	return data;
}


void ProjectSettings::readSettings()
{
	for (auto &i : _project->startupCodes()) {
		QListWidgetItem *li = new QListWidgetItem(firstLine(i->str), _startupcodes->listWidget());
		li->setData(FILEDATA_ROLE, i->str);
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(i->checked ? Qt::Checked : Qt::Unchecked);
		_startupcodes->listWidget()->addItem(li);
	}

	for (auto &i : _project->includePaths()) {
		_includepaths->listWidget()->addItem(i);
	}

	for (auto &i : _project->compilerFlags()) {
		_compilerflags->listWidget()->addItem(i);
	}
}

void ProjectSettings::writeSettings()
{
	_project->startupCodes().clear();
	for (int i = 0; i < _startupcodes->listWidget()->count(); i++) {
		_project->startupCodes().append(std::make_shared<msglib::CheckStringListData>(_startupcodes->listWidget()->item(i)->data(FILEDATA_ROLE).toString(), _startupcodes->listWidget()->item(i)->checkState() != Qt::Unchecked));
	}

	_project->includePaths().clear();
	for (int i = 0; i < _includepaths->listWidget()->count(); i++) {
		_project->includePaths().append(_includepaths->listWidget()->item(i)->text());
	}

	_project->compilerFlags().clear();
	for (int i = 0; i < _compilerflags->listWidget()->count(); i++) {
		_project->compilerFlags().append(_compilerflags->listWidget()->item(i)->text());
	}

	_project->setModified();
}

void ProjectSettings::btnStartupCodeAddClicked()
{
	CodeDialog d(_configuration, "", this);
	if (d.exec()) {
		QListWidgetItem *li = new QListWidgetItem(firstLine(d.text()), _startupcodes->listWidget());
		li->setData(FILEDATA_ROLE, d.text());
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(Qt::Checked);
		_startupcodes->listWidget()->addItem(li);
	}
}

void ProjectSettings::btnStartupCodeEditClicked()
{
	if (_startupcodes->listWidget()->currentRow() == -1) return;

	CodeDialog d(_configuration, _startupcodes->listWidget()->item(_startupcodes->listWidget()->currentRow())->data(FILEDATA_ROLE).toString(), this);
	if (d.exec()) {
		_startupcodes->listWidget()->item(_startupcodes->listWidget()->currentRow())->setText(firstLine(d.text()));
		_startupcodes->listWidget()->item(_startupcodes->listWidget()->currentRow())->setData(FILEDATA_ROLE, d.text());
	}
}

void ProjectSettings::btnStartupCodeRemoveClicked()
{
	if (_startupcodes->listWidget()->currentRow() == -1) return;

	QListWidgetItem *item = _startupcodes->listWidget()->takeItem(_startupcodes->listWidget()->currentRow());
	delete item;
}

void ProjectSettings::btnIncludeAddClicked()
{
	QString directory = QFileDialog::getExistingDirectory(this, tr("Select include directory"), QDir::currentPath());
	if (!directory.isEmpty()) {
		_includepaths->listWidget()->addItem(directory);
	}
}

void ProjectSettings::btnIncludeRemoveClicked()
{
	if (_includepaths->listWidget()->currentRow() == -1) return;

	QListWidgetItem *item = _includepaths->listWidget()->takeItem(_includepaths->listWidget()->currentRow());
	delete item;
}

void ProjectSettings::btnCompilerFlagsAddClicked()
{
	QString text = QInputDialog::getText(this, "Add compilar flag", "Enter compiler (Clang) flag");
	if (!text.isEmpty()) {
		_compilerflags->listWidget()->addItem(text);
	}
}

void ProjectSettings::btnCompilerFlagsRemoveClicked()
{
	if (_compilerflags->listWidget()->currentRow() == -1) return;

	QListWidgetItem *item = _compilerflags->listWidget()->takeItem(_compilerflags->listWidget()->currentRow());
	delete item;
}

void ProjectSettings::onBtnOk()
{
	writeSettings();
	accept();
}

void ProjectSettings::onBtnCancel()
{
	reject();
}

}