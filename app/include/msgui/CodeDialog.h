#pragma once

#include "msgui/interfaces.h"

#include <mredit/Editor.h>

#include <QDialog>
#include <QKeyEvent>

namespace msgui {

class CodeDialog : public QDialog
{
public:
	CodeDialog(itf::Configuration *configuration, const QString &code, QWidget *parent);

	QString text() const;
private slots:
	void onBtnOk();
	void onBtnCancel();
protected:
	void keyPressEvent(QKeyEvent *);
private:
	itf::Configuration *_configuration;
	mredit::Editor *_editor;
};

}