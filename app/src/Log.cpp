#include "msgui/Log.h"

#include <mredit/Label.h>

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>
#include <QFontDatabase>

#include <log4qt/helpers/datetime.h>

namespace msgui {

Log::Log(QWidget *parent) :
	QTreeWidget(parent)
{
	setColumnCount(3);
	setHeaderLabels(QStringList() << "Time" << "Category" << "Message");
	header()->resizeSection(0, 100);
	header()->resizeSection(1, 110);

	connect(header(), &QHeaderView::sectionResized, this, &Log::columnResized);
}

void Log::addLog(const Log4Qt::LoggingEvent &event)
{
	QTreeWidgetItem *logitem = new QTreeWidgetItem(this);

	logitem->setText(0, Log4Qt::DateTime::fromMSecsSinceEpoch(event.timeStamp()).toString("hh:mm:ss.zzz"));
	logitem->setText(1, event.loggename());
	mredit::Label *msglbl = new mredit::Label(this);
	msglbl->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	msglbl->setPlainText(event.message());
	setItemWidget(logitem, 2, msglbl);

	addTopLevelItem(logitem);

	scrollToBottom();
}

void Log::columnResized(int logicalIndex, int oldSize, int newSize)
{
	scheduleDelayedItemsLayout();
}

}
