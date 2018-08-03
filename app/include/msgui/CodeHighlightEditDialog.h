#pragma once

#include <tl/optional.hpp>

#include <QDialog>
#include <QLabel>
#include <QFont>
#include <QColor>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>

namespace msgui {

class CodeHighlightEditDialog : public QDialog
{
	Q_OBJECT
public:
	CodeHighlightEditDialog(tl::optional<QColor> fgcolor, tl::optional<QColor> bgcolor, QWidget *parent);

	tl::optional<QColor> fgcolor() { return _fgcolor; }
	tl::optional<QColor> bgcolor() { return _bgcolor; }
private slots:
	void fgColorClicked();
	void fgColorDefaultClicked();
	void bgColorClicked();
	void bgColorDefaultClicked();

	void onBtnOk();
	void onBtnCancel();
private:
	void updateSelect();
	void exampleChanged();

	QLineEdit * _text;
	QLabel *_example;
	QPushButton *_fgSelect;
	QCheckBox *_fgDefault;
	QPushButton *_bgSelect;
	QCheckBox *_bgDefault;
	tl::optional<QColor> _fgcolor;
	tl::optional<QColor> _bgcolor;
};

}