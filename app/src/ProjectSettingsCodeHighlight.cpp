#include "msgui/ProjectSettingsCodeHighlight.h"

#include <QSettings>
#include <QDir>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>

#define CHINFO_ROLE 8123

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
	for (auto &i : _project->codeHighlight()) {
		QListWidgetItem *li = new QListWidgetItem(i->regexp, _codehighlight->listWidget());
		chrole_t chinfo;
		chinfo["bold"] = i->bold;
		chinfo["fgcolor"] = i->fgcolor ? i->fgcolor.value() : QVariant();
		chinfo["bgcolor"] = i->bgcolor ? i->bgcolor.value() : QVariant();
		li->setData(CHINFO_ROLE, chinfo);
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(i->enabled ? Qt::Checked : Qt::Unchecked);
		_codehighlight->listWidget()->addItem(li);
	}
}

void ProjectSettingsCodeHighlight::writeSettings()
{
	_project->codeHighlight().clear();
	for (int i = 0; i < _codehighlight->listWidget()->count(); i++) {
		QListWidgetItem *li = _codehighlight->listWidget()->item(i);
		chrole_t chinfo = li->data(CHINFO_ROLE).toMap();
		_project->codeHighlight().append(std::make_shared<ProjectCodeHighlight>(li->checkState() != Qt::Unchecked, li->text(),
			chinfo["fgcolor"].isValid() ? chinfo["fgcolor"].value<QColor>() : tl::optional<QColor>{},
			chinfo["bgcolor"].isValid() ? chinfo["bgcolor"].value<QColor>() : tl::optional<QColor>{},
			chinfo["bold"].toBool()));
	}

	_project->setModified();
}

void ProjectSettingsCodeHighlight::btnCodeHighlightAddClicked()
{
	CodeHighlightEditDialog d(this);
	if (d.exec()) {
		QListWidgetItem *li = new QListWidgetItem(d.regexp(), _codehighlight->listWidget());
		chrole_t chinfo;
		chinfo["fgcolor"] = d.fgcolor()?d.fgcolor().value():QVariant();
		chinfo["bgcolor"] = d.bgcolor()?d.bgcolor().value():QVariant();
		chinfo["bold"] = d.bold();
		li->setData(CHINFO_ROLE, chinfo);
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(Qt::Checked);
		_codehighlight->listWidget()->addItem(li);
	}
}

void ProjectSettingsCodeHighlight::btnCodeHighlightEditClicked()
{
	if (_codehighlight->listWidget()->currentRow() == -1) return;

	QListWidgetItem *li = _codehighlight->listWidget()->item(_codehighlight->listWidget()->currentRow());
	chrole_t chinfo = li->data(CHINFO_ROLE).toMap();
	CodeHighlightEditDialog d(li->text(), 
		chinfo["fgcolor"].isValid() ? chinfo["fgcolor"].value<QColor>() : tl::optional<QColor>{},
		chinfo["bgcolor"].isValid() ? chinfo["bgcolor"].value<QColor>() : tl::optional<QColor>{},
		chinfo["bold"].toBool(),
	this);
	if (d.exec()) {
		li->setText(d.regexp());
		chrole_t chinfo;
		chinfo["fgcolor"] = d.fgcolor() ? d.fgcolor().value() : QVariant();
		chinfo["bgcolor"] = d.bgcolor() ? d.bgcolor().value() : QVariant();
		chinfo["bold"] = d.bold();
		li->setData(CHINFO_ROLE, chinfo);
	}
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