#pragma once

#include <mredit/Editor.h>

#include <QDialog>

namespace msgui {

class CodeDialog : public QDialog
{
public:
	CodeDialog(const QString &code, QWidget *parent);

	QString text() const;
private slots:
	void onBtnOk();
	void onBtnCancel();
private:
	mredit::Editor *_editor;
};

}