#include "msgui/StartupCode.h"
#include "msgui/CodeDialog.h"

#include <mredit/Label.h>

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>
#include <QFontDatabase>

#define FILEDATA_ROLE 9112

namespace msgui {

StartupCode::StartupCode(itf::Configuration *configuration, QWidget *parent) :
	msgwidget::UpDownListWidget(parent), _configuration(configuration)
{
	showAddButton();
	showEditButton();
	showRemoveButton();

	connect(this, &StartupCode::addClicked, this, &StartupCode::btnStartupCodeAddClicked);
	connect(this, &StartupCode::UpDownListWidget::editClicked, this, &StartupCode::btnStartupCodeEditClicked);
	connect(this, &StartupCode::UpDownListWidget::removeClicked, this, &StartupCode::btnStartupCodeRemoveClicked);
	connect(listWidget(), &QListWidget::itemClicked, this, &StartupCode::onItemClicked);
	connect(listWidget(), &QListWidget::itemDoubleClicked, this, &StartupCode::onItemDoubleClicked);
	connect(this, &StartupCode::itemPositionChanged, this, &StartupCode::onItemPositionChanged);
}

void StartupCode::loadProject(Project *project)
{
	_project = project;

	listWidget()->clear();

	if (_project)
	{
		for (auto &i : _project->startupCodes()) {
			QListWidgetItem *li = new QListWidgetItem(firstLine(i->str), listWidget());
			li->setData(FILEDATA_ROLE, i->str);
			li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
			li->setCheckState(i->checked ? Qt::Checked : Qt::Unchecked);
			listWidget()->addItem(li);
		}
	}
}

void StartupCode::writeSettings()
{
	_project->startupCodes().clear();
	for (int i = 0; i < listWidget()->count(); i++) {
		_project->startupCodes().append(std::make_shared<msglib::CheckStringListData>(listWidget()->item(i)->data(FILEDATA_ROLE).toString(), listWidget()->item(i)->checkState() != Qt::Unchecked));
	}
	_project->setModified();

	emit changed();
}

void StartupCode::btnStartupCodeAddClicked()
{
	CodeDialog d(_configuration, "", this);
	if (d.exec()) {
		QListWidgetItem *li = new QListWidgetItem(firstLine(d.text()), listWidget());
		li->setData(FILEDATA_ROLE, d.text());
		li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
		li->setCheckState(Qt::Checked);
		listWidget()->addItem(li);

		writeSettings();
	}
}

void StartupCode::btnStartupCodeEditClicked()
{
	if (listWidget()->currentRow() == -1) return;

	CodeDialog d(_configuration, listWidget()->item(listWidget()->currentRow())->data(FILEDATA_ROLE).toString(), this);
	if (d.exec()) {
		listWidget()->item(listWidget()->currentRow())->setText(firstLine(d.text()));
		listWidget()->item(listWidget()->currentRow())->setData(FILEDATA_ROLE, d.text());

		writeSettings();
	}
}

void StartupCode::btnStartupCodeRemoveClicked()
{
	if (listWidget()->currentRow() == -1) return;

	QListWidgetItem *item = listWidget()->takeItem(listWidget()->currentRow());
	delete item;

	writeSettings();
}

void StartupCode::onItemClicked(QListWidgetItem *item)
{
	if (item == nullptr)
		return;
	
	int row = listWidget()->row(item);
	if (row >= 0 && row < _project->startupCodes().size()) {
		bool is_check = item->checkState() == Qt::Checked;
		if (_project->startupCodes()[row]->checked != is_check) {
			_project->startupCodes()[row]->checked = is_check;
			_project->setModified();

			emit changed();
		}
	}
}

void StartupCode::onItemDoubleClicked(QListWidgetItem *item)
{
	if (item == nullptr)
		return;

	int row = listWidget()->row(item);
	if (row >= 0 && row < _project->startupCodes().size()) {
		emit showCode(item->data(FILEDATA_ROLE).toString());
	}
}


void StartupCode::onItemPositionChanged()
{
	writeSettings();
}

QString StartupCode::firstLine(const QString &data)
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


}
