#include "msgui/TemplateKind.h"
#include "msgui/Util.h"

#include <mredit/Label.h>

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>
#include <QFontDatabase>

#define FILENAME_ROLE 9551

namespace msgui {

TemplateKind::TemplateKind(itf::Configuration *configuration, QWidget *parent) :
	QTreeWidget(parent), _configuration(configuration)
{
	setColumnCount(1);
	setHeaderLabels(QStringList() << "Name" << "Source"); 
	header()->resizeSection(0, 300);
	//header()->resizeSection(1, 330);

	connect(this, &QTreeWidget::itemClicked, this, &TemplateKind::btItemClicked);
	connect(header(), &QHeaderView::sectionResized, this, &TemplateKind::columnResized);
}

void TemplateKind::addTemplateKind(const QString &name, const QString &kind, const QString &sourceLocation)
{
	if (kind != "TemplateInstantiation")
		return;

	// check if already exists
	auto find = findItems(name, Qt::MatchCaseSensitive);
	if (find.size() > 0)
		return;

	QTreeWidgetItem *item = new QTreeWidgetItem(this);

	item->setData(1, FILENAME_ROLE, sourceLocation);

	mredit::Label *msglbl = new mredit::Label(this);
	msglbl->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	_configuration->createCPPHighligher(msglbl->document());
	msglbl->setPlainText(_configuration->indentCPPType(name));
	setItemWidget(item, 0, msglbl);
	QFileInfo fi(sourceLocation);
	item->setText(1, fi.fileName());

	item->setToolTip(0, QString("<pre>%1</pre>").arg(Util::indentCPPType(name).toHtmlEscaped()));

	addTopLevelItem(item);

	scrollToBottom();

	scheduleDelayedItemsLayout();
}

void TemplateKind::btItemClicked(QTreeWidgetItem *item, int column)
{
	if (item)
	{
		emit showFileAndLine(item->data(1, FILENAME_ROLE).toString());
	}
}

void TemplateKind::columnResized(int logicalIndex, int oldSize, int newSize)
{
	scheduleDelayedItemsLayout();
}

void TemplateKind::onProjectChanged()
{
	// TODO
}

}
