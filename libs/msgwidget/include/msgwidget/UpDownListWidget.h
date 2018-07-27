#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

namespace msgwidget {

class UpDownListWidget : public QWidget
{
	Q_OBJECT
public:
	UpDownListWidget(QWidget *parent);

	void showAddButton();
	void showEditButton();
	void showRemoveButton();

	QListWidget *listWidget() const;
signals:
	void addClicked();
	void editClicked();
	void removeClicked();
public slots:
	void moveUp();
	void moveDown();
	void moveTop();
	void moveBottom();
private:
	QListWidget * _listwidget;
	QPushButton *_btnAdd, *_btnEdit, *_btnRemove;
};

}