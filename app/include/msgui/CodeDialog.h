#pragma once

#include <mredit/Editor.h>

#include <QDialog>
#include <QKeyEvent>

namespace msgui {

class CodeDialog : public QDialog
{
public:
	CodeDialog(const QString &code, QWidget *parent);

	QString text() const;
private slots:
	void onBtnOk();
	void onBtnCancel();
protected:
	void keyPressEvent(QKeyEvent *);
private:
	mredit::Editor *_editor;
};

}