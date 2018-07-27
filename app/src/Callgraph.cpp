#include "msgui/Callgraph.h"
#include "msgwidget/highlighter/HL_CPP.h"

#include <mredit/Label.h>

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>

#include <functional>

#define FILENAME_ROLE 9552

namespace msgui {

Callgraph::Callgraph(QWidget *parent) :
	QTreeWidget(parent), _callgraph()
{
	setColumnCount(3);
	setHeaderLabels(QStringList() << "Name" << "Kind" << "Source");
	header()->resizeSection(0, 230);
	header()->resizeSection(1, 180);
	header()->resizeSection(2, 70);

	connect(this, &QTreeWidget::itemClicked, this, &Callgraph::btItemClicked);
	connect(header(), &QHeaderView::sectionResized, this, &Callgraph::columnResized);
}

void Callgraph::setCallgraph(msglib::cmd::call_graph::ptr callgraph)
{
	if (callgraph)
	{
		clear();

		std::function<void(msglib::cmd::call_graph_tree_node::ptr, QTreeWidgetItem *item_parent)> write_node;
		write_node = [this, &write_node](msglib::cmd::call_graph_tree_node::ptr node, QTreeWidgetItem *item_parent) {
			QTreeWidgetItem *item = nullptr;

			if (node->node)
			{
				if (item_parent)
					item = new QTreeWidgetItem(item_parent);
				else
					item = new QTreeWidgetItem(this);

				item->setData(2, FILENAME_ROLE, node->node->source_location);

				mredit::Label *namelbl = new mredit::Label(node->node->name, this);
				new msgwidget::highlighter::HL_CPP(namelbl->document());
				setItemWidget(item, 0, namelbl);

				item->setText(1, node->node->kind);
				QFileInfo fi(node->node->source_location);
				if (fi.isAbsolute())
					item->setText(2, fi.fileName());
				else
					item->setText(2, node->node->source_location);

				item->setExpanded(true);
			}

			for (auto &n : node->nodes) {
				write_node(n, item);
			}
		};

		write_node(callgraph->nodes_root, nullptr);
	}
	_callgraph =  callgraph;
}

void Callgraph::btItemClicked(QTreeWidgetItem *item, int column)
{
	if (item)
	{
		emit showFileAndLine(item->data(2, FILENAME_ROLE).toString());
	}
}

void Callgraph::columnResized(int logicalIndex, int oldSize, int newSize)
{
	scheduleDelayedItemsLayout();
}


}