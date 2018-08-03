#include "msgui/CodeHighlightEditDialog.h"

#include <msgwidget/highlighter/HL_CPP.h>
#include <mredit/margin/MarginStacker.h>

#include <QBoxLayout>
#include <QFormLayout>
#include <QPalette>
#include <QColorDialog>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

namespace msgui {

CodeHighlightEditDialog::CodeHighlightEditDialog(tl::optional<QColor> fgcolor, tl::optional<QColor> bgcolor, QWidget *parent) :
	QDialog(parent), _fgcolor(fgcolor), _bgcolor(bgcolor)
{
	setWindowTitle("Code highlight");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	QFormLayout *form = new QFormLayout;
	layout->addLayout(form);

	_text = new QLineEdit(this);
	form->addRow(tr("Text"), _text);

	_example = new QLabel(this);
	_example->setAutoFillBackground(true);
	_example->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	_example->setText("This is an example");
	form->addRow("Example", _example);

	// fgcolor
	QHBoxLayout *fglayout = new QHBoxLayout;

	_fgSelect = new QPushButton(tr("Select"), this);
	fglayout->addWidget(_fgSelect);
	connect(_fgSelect, &QPushButton::clicked, this, &CodeHighlightEditDialog::fgColorClicked);
	_fgDefault = new QCheckBox(tr("Default"), this);
	_fgDefault->setChecked(!_fgcolor);
	fglayout->addWidget(_fgDefault);
	connect(_fgDefault, &QCheckBox::clicked, this, &CodeHighlightEditDialog::fgColorDefaultClicked);

	form->addRow(tr("Foreground color"), fglayout);

	// bgcolor
	QHBoxLayout *bglayout = new QHBoxLayout;

	_bgSelect = new QPushButton(tr("Select"), this);
	bglayout->addWidget(_bgSelect);
	connect(_bgSelect, &QPushButton::clicked, this, &CodeHighlightEditDialog::bgColorClicked);
	_bgDefault = new QCheckBox(tr("Default"), this);
	_bgDefault->setChecked(!_fgcolor);
	bglayout->addWidget(_bgDefault);
	connect(_bgDefault, &QCheckBox::clicked, this, &CodeHighlightEditDialog::bgColorDefaultClicked);

	form->addRow(tr("Background color"), bglayout);

	// buttons
	QHBoxLayout *btnlayout = new QHBoxLayout;
	layout->addLayout(btnlayout);

	QPushButton *btnOk = new QPushButton(tr("Ok"), this);
	connect(btnOk, &QPushButton::clicked, this, &CodeHighlightEditDialog::onBtnOk);
	QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
	connect(btnCancel, &QPushButton::clicked, this, &CodeHighlightEditDialog::onBtnCancel);

	btnlayout->addWidget(btnOk);
	btnlayout->addWidget(btnCancel);

	exampleChanged();
	updateSelect();

	//resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

void CodeHighlightEditDialog::fgColorClicked()
{
	QColor c(QApplication::palette().color(QPalette::WindowText));
	if (_fgcolor)
		c = _fgcolor.value();

	QColorDialog d(c, this);
	if (d.exec()) {
		_fgcolor = d.currentColor();
		exampleChanged();
	}
}

void CodeHighlightEditDialog::fgColorDefaultClicked()
{
	if (_fgDefault->isChecked())
		_fgcolor = nullptr;
	exampleChanged();

	updateSelect();
}

void CodeHighlightEditDialog::bgColorClicked()
{
	QColor c(QApplication::palette().color(QPalette::Window));
	if (_bgcolor)
		c = _bgcolor.value();

	QColorDialog d(c, this);
	if (d.exec()) {
		_bgcolor = d.currentColor();
		exampleChanged();
	}
}

void CodeHighlightEditDialog::bgColorDefaultClicked()
{
	if (_bgDefault->isChecked())
		_fgcolor = nullptr;
	exampleChanged();

	updateSelect();
}

void CodeHighlightEditDialog::updateSelect()
{
	_fgSelect->setEnabled(!_fgDefault->isChecked());
	_bgSelect->setEnabled(!_bgDefault->isChecked());
}

void CodeHighlightEditDialog::exampleChanged()
{
	QPalette pal(_example->style()->standardPalette());
	if (_bgcolor)
		pal.setColor(_example->backgroundRole(), _bgcolor.value());
	if (_fgcolor)
		pal.setColor(_example->foregroundRole(), _fgcolor.value());
	_example->setPalette(pal);

	_example->update();
}

void CodeHighlightEditDialog::onBtnOk()
{
	accept();
}

void CodeHighlightEditDialog::onBtnCancel()
{
	reject();
}

}