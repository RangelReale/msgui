#include "msgui/ProjectSettingsCodeHighlight.h"
#include "msgui/CodeHighlightEditDialog.h"

#include <QSettings>
#include <QDir>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>

#define FILEDATA_ROLE 9588

namespace msgui {

ProjectSettingsCodeHighlight::ProjectSettingsCodeHighlight(Project *project, QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint), _project(project)
{
	setWindowTitle("Project code highlight");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	// settings
	QFormLayout *form = new QFormLayout;
	layout->addLayout(form);

	// code highlight
	_codehighlight = new msgwidget::UpDownListWidget(this);
	_codehighlight->showAddButton();
	_codehighlight->showEditButton();
	_codehighlight->showRemoveButton();
	connect(_codehighlight, &msgwidget::UpDownListWidget::addClicked, this, &ProjectSettingsCodeHighlight::btnCodeHighlightAddClicked);
	connect(_codehighlight, &msgwidget::UpDownListWidget::editClicked, this, &ProjectSettingsCodeHighlight::btnCodeHighlightEditClicked);
	connect(_codehighlight, &msgwidget::UpDownListWidget::removeClicked, this, &ProjectSettingsCodeHighlight::btnCodeHighlightRemoveClicked);
	form->addRow(tr("Code highlight"), _codehighlight);

	// buttons
	QHBoxLayout *btnlayout = new QHBoxLayout;
	layout->addLayout(btnlayout);

	QPushButton *btnOk = new QPushButton(tr("Ok"), this);
	connect(btnOk, &QPushButton::clicked, this, &ProjectSettingsCodeHighlight::onBtnOk);
	QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
	connect(btnCancel, &QPushButton::clicked, this, &ProjectSettingsCodeHighlight::onBtnCancel);

	btnlayout->addWidget(btnOk);
	btnlayout->addWidget(btnCancel);

	// read settings
	readSettings();
}

void ProjectSettingsCodeHighlight::readSettings()
{
	for (auto &i : _project->startupCodes()) {
		/*
		QListWidgetItem *li = new QListWidgetItem(firstLine(i->str), _codehighlight->listWidget());
		li->setData(FILEDATA_ROLE, i->str);
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(i->checked ? Qt::Checked : Qt::Unchecked);
		_codehighlight->listWidget()->addItem(li);
		*/
	}
}

void ProjectSettingsCodeHighlight::writeSettings()
{
	/*
	_project->startupCodes().clear();
	for (int i = 0; i < _codehighlight->listWidget()->count(); i++) {
		_project->startupCodes().append(std::make_shared<msglib::CheckStringListData>(_codehighlight->listWidget()->item(i)->data(FILEDATA_ROLE).toString(), _codehighlight->listWidget()->item(i)->checkState() != Qt::Unchecked));
	}
	*/

	_project->setModified();
}

void ProjectSettingsCodeHighlight::btnCodeHighlightAddClicked()
{
	CodeHighlightEditDialog d({}, {}, this);
	if (d.exec()) {
		/*
		QListWidgetItem *li = new QListWidgetItem(firstLine(d.text()), _codehighlight->listWidget());
		li->setData(FILEDATA_ROLE, d.text());
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(Qt::Checked);
		_codehighlight->listWidget()->addItem(li);
		*/
	}
}

void ProjectSettingsCodeHighlight::btnCodeHighlightEditClicked()
{
	/*
	if (_codehighlight->listWidget()->currentRow() == -1) return;

	CodeDialog d(_codehighlight->listWidget()->item(_codehighlight->listWidget()->currentRow())->data(FILEDATA_ROLE).toString(), this);
	if (d.exec()) {
		_codehighlight->listWidget()->item(_codehighlight->listWidget()->currentRow())->setText(firstLine(d.text()));
		_codehighlight->listWidget()->item(_codehighlight->listWidget()->currentRow())->setData(FILEDATA_ROLE, d.text());
	}
	*/
}

void ProjectSettingsCodeHighlight::btnCodeHighlightRemoveClicked()
{
	if (_codehighlight->listWidget()->currentRow() == -1) return;

	QListWidgetItem *item = _codehighlight->listWidget()->takeItem(_codehighlight->listWidget()->currentRow());
	delete item;
}

void ProjectSettingsCodeHighlight::onBtnOk()
{
	writeSettings();
	accept();
}

void ProjectSettingsCodeHighlight::onBtnCancel()
{
	reject();
}

}