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

	_btnAdd = new QPushButton(QIcon(":/list-add.png"), "", this);
	_btnEdit = new QPushButton(QIcon(":/mail-mark-task.png"), "", this);
	_btnRemove = new QPushButton(QIcon(":/list-remove.png"), "", this);
	_btnAdd->setVisible(false);
	_btnAdd->setToolTip(tr("Add"));
	_btnEdit->setVisible(false);
	_btnEdit->setToolTip(tr("Edit"));
	_btnRemove->setVisible(false);
	_btnRemove->setToolTip(tr("Remove"));

	QPushButton *btnTop = new QPushButton(QIcon(":/arrow-up-double.png"), "", this);
	QPushButton *btnUp = new QPushButton(QIcon(":/arrow-up.png"), "", this);
	QPushButton *btnDown = new QPushButton(QIcon(":/arrow-down.png"), "", this);
	QPushButton *btnBottom = new QPushButton(QIcon(":/arrow-down-double.png"), "", this);
	btnTop->setToolTip(tr("Top"));
	btnUp->setToolTip(tr("Up"));
	btnDown->setToolTip(tr("Down"));
	btnBottom->setToolTip(tr("Bottom"));

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

	emit itemPositionChanged();
}

void UpDownListWidget::moveDown()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(currentIndex + 1, currentItem);
	_listwidget->setCurrentRow(currentIndex + 1);

	emit itemPositionChanged();
}

void UpDownListWidget::moveTop()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(0, currentItem);
	_listwidget->setCurrentRow(0);

	emit itemPositionChanged();
}

void UpDownListWidget::moveBottom()
{
	if (_listwidget->currentRow() == -1) return;

	int currentIndex = _listwidget->currentRow();
	QListWidgetItem *currentItem = _listwidget->takeItem(currentIndex);
	_listwidget->insertItem(_listwidget->count(), currentItem);
	_listwidget->setCurrentRow(_listwidget->count() - 1);

	emit itemPositionChanged();
}

}