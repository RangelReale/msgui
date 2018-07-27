#include "msgwidget/UpDownListWidget.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QStyle>

namespace msgwidget {

UpDownListWidget::UpDownListWidget(QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout;
	setLayout(layout);

	_listwidget = new QListWidget(this);
	layout->addWidget(_listwidget);

	QVBoxLayout *btnlayout = new QVBoxLayout;
	layout->addLayout(btnlayout);

	_btnAdd = new QPushButton(tr("Add"), this);
	_btnEdit = new QPushButton(tr("Edit"), this);
	_btnRemove = new QPushButton(tr("Remove"), this);
	_btnAdd->setVisible(false);
	_btnEdit->setVisible(false);
	_btnRemove->setVisible(false);

	QPushButton *btnTop = new QPushButton(style()->standardIcon(QStyle::SP_ArrowUp), tr("Top"), this);
	QPushButton *btnUp = new QPushButton(style()->standardIcon(QStyle::SP_ArrowUp), tr("Up"), this);
	QPushButton *btnDown = new QPushButton(style()->standardIcon(QStyle::SP_ArrowDown), tr("Down"), this);
	QPushButton *btnBottom = new QPushButton(style()->standardIcon(QStyle::SP_ArrowDown), tr("Bottom"), this);

	connect(_btnAdd, &QPushButton::clicked, this, &UpDownListWidget::addClicked);
	connect(_btnEdit, &QPushButton::clicked, this, &UpDownListWidget::editClicked);
	connect(_btnRemove, &QPushButton::clicked, this, &UpDownListWidget::removeClicked);

	connect(btnTop, &QPushButton::clicked, this, &UpDownListWidget::moveTop);
	connect(btnUp, &QPushButton::clicked, this, &UpDownListWidget::moveUp);
	connect(btnDown, &QPushButton::clicked, this, &UpDownListWidget::moveDown);
	connect(btnBottom, &QPushButton::clicked, this, &UpDownListWidget::moveBottom);

	btnlayout->addStretch();
	btnlayout->addWidget(_btnAdd);
	btnlayout->addWidget(_btnEdit);
	btnlayout->addWidget(_btnRemove);
	btnlayout->addWidget(btnTop);
	btnlayout->addWidget(btnUp);
	btnlayout->addWidget(btnDown);
	btnlayout->addWidget(btnBottom);
	btnlayout->addStretch();
}

QListWidget *UpDownListWidget::listWidget() const
{
	return _listwidget;
}

void UpDownListWidget::showAddButton()
{
	_btnAdd->setVisible(true);
}

void UpDownListWidget::showEditButton()
{
	_btnEdit->setVisible(true);
}

void UpDownListWidget::showRemoveButton()
{
	_btnRemove->setVisible(true);
}

void UpDownListWidget::moveUp()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(currentIndex - 1, currentItem);
	_listwidget->setCurrentRow(currentIndex - 1);
}

void UpDownListWidget::moveDown()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(currentIndex + 1, currentItem);
	_listwidget->setCurrentRow(currentIndex + 1);
}

void UpDownListWidget::moveTop()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(0, currentItem);
	_listwidget->setCurrentRow(0);
}

void UpDownListWidget::moveBottom()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(_listwidget->count(), currentItem);
	_listwidget->setCurrentRow(_listwidget->count() - 1);
}

}