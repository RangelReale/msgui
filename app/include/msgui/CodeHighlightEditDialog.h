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
	CodeHighlightEditDialog(const QString &regexp, tl::optional<QColor> fgcolor, tl::optional<QColor> bgcolor, bool bold, QWidget *parent);
	CodeHighlightEditDialog(QWidget *parent);

	const QString &regexp() const { return _regexp; }
	tl::optional<QColor> fgcolor() const { return _fgcolor; }
	tl::optional<QColor> bgcolor() const { return _bgcolor; }
	bool bold() const { return _bold; }
private slots:
	void fgColorClicked();
	void fgColorDefaultClicked();
	void bgColorClicked();
	void bgColorDefaultClicked();
	void boldClicked();

	void onBtnOk();
	void onBtnCancel();
private:
	void updateSelect();
	void exampleChanged();

	QLineEdit * _regexpedit;
	QLabel *_example;
	QPushButton *_fgSelect;
	QCheckBox *_fgDefault;
	QPushButton *_bgSelect;
	QCheckBox *_bgDefault;
	QCheckBox *_fBold;

	QString _regexp;
	bool _bold;
	tl::optional<QColor> _fgcolor;
	tl::optional<QColor> _bgcolor;
};

}