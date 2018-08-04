#include "msgui/CodeHighlightEditDialog.h"

#include <mredit/margin/MarginStacker.h>

#include <QBoxLayout>
#include <QFormLayout>
#include <QPalette>
#include <QColorDialog>
#include <QStyle>
#include <QApplication>
#include <QMessageBox>
#include <QFontDatabase>
#include <QDesktopWidget>

namespace msgui {

CodeHighlightEditDialog::CodeHighlightEditDialog(const QString &regexp, tl::optional<QColor> fgcolor, tl::optional<QColor> bgcolor, bool bold, QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint), _regexp(regexp), _fgcolor(fgcolor), _bgcolor(bgcolor), _bold(bold)
{
	setWindowTitle("Code highlight");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	QFormLayout *form = new QFormLayout;
	layout->addLayout(form);

	_regexpedit = new QLineEdit(this);
	_regexpedit->setText(regexp);
	form->addRow(tr("Text/Regexp"), _regexpedit);

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

	// bold
	_fBold = new QCheckBox(this);
	_fBold->setChecked(bold);
	connect(_fBold, &QCheckBox::clicked, this, &CodeHighlightEditDialog::boldClicked);
	form->addRow(tr("Bold"), _fBold);

	// bgcolor
	QHBoxLayout *bglayout = new QHBoxLayout;

	_bgSelect = new QPushButton(tr("Select"), this);
	bglayout->addWidget(_bgSelect);
	connect(_bgSelect, &QPushButton::clicked, this, &CodeHighlightEditDialog::bgColorClicked);
	_bgDefault = new QCheckBox(tr("Default"), this);
	_bgDefault->setChecked(!_bgcolor);
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

CodeHighlightEditDialog::CodeHighlightEditDialog(QWidget *parent) :
	CodeHighlightEditDialog("", {}, {}, false, parent)
{

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
		_fgcolor.reset();
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
		_bgcolor.reset();
	exampleChanged();

	updateSelect();
}

void CodeHighlightEditDialog::boldClicked()
{
	exampleChanged();
}

void CodeHighlightEditDialog::updateSelect()
{
	_fgSelect->setEnabled(!_fgDefault->isChecked());
	_bgSelect->setEnabled(!_bgDefault->isChecked());
}

void CodeHighlightEditDialog::exampleChanged()
{
	QPalette pal(style()->standardPalette());
	if (_bgcolor)
		pal.setColor(_example->backgroundRole(), _bgcolor.value());
	if (_fgcolor)
		pal.setColor(_example->foregroundRole(), _fgcolor.value());
	_example->setPalette(pal);
	QFont f(_example->font());
	f.setBold(_fBold->isChecked());
	_example->setFont(f);

	_example->update();
}

void CodeHighlightEditDialog::onBtnOk()
{
	if (_regexpedit->text().trimmed().isEmpty()) {
		QMessageBox::warning(this, tr("Code highlight"), "Text/regexp is required", QMessageBox::Ok);
		return;
	}

	_regexp = _regexpedit->text();
	_bold = _fBold->isChecked();

	accept();
}

void CodeHighlightEditDialog::onBtnCancel()
{
	reject();
}

}