#include <QVBoxLayout>

#include "msgwidget/internal/WidgetListWidgetViewport.h"

namespace msgwidget {
namespace internal {

WidgetListWidgetViewport::WidgetListWidgetViewport(QWidget *parent) :
	QWidget(parent),
	layout_(new QVBoxLayout(this)) 
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setSpacing(0);
	// adds a bottom spacer so the widgets doesn't take the full space
	_bottomspacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout_->addItem(_bottomspacer);
	this->setLayout(layout_);
}

WidgetListWidgetViewport::~WidgetListWidgetViewport() 
{
}

void WidgetListWidgetViewport::layoutSetSpacing(int spacing)
{
	layout_->setSpacing(spacing);
}

void WidgetListWidgetViewport::clear()
{
	layout_->removeItem(_bottomspacer);

	QLayoutItem* child;
	while (layout_->count() != 0)
	{
		child = layout_->takeAt(0);
		if (child->layout() != 0)
		{
			child->layout()->removeItem(child);
		}
		else if (child->widget() != 0)
		{
			delete child->widget();
		}
		delete child;
	}

	layout_->addItem(_bottomspacer);
}

void WidgetListWidgetViewport::addWidget(QWidget* widget, int stretch,
	Qt::Alignment alignment) 
{
	layout_->removeItem(_bottomspacer);
	layout_->addWidget(widget, stretch, alignment);
	layout_->addItem(_bottomspacer);
}

void WidgetListWidgetViewport::insertWidget(int index, QWidget* widget, int stretch,
	Qt::Alignment alignment) 
{
	layout_->removeItem(_bottomspacer);
	layout_->insertWidget(index, widget, stretch, alignment);
	layout_->addItem(_bottomspacer);
}

void WidgetListWidgetViewport::removeWidget(QWidget* widget) 
{
	layout_->removeWidget(widget);
}

QList<QWidget*> WidgetListWidgetViewport::getWidgetList() const
{
	QWidgetList list;
	for (int i = 0; i < layout_->count()-1; ++i) {
		list.push_back(layout_->itemAt(i)->widget());
	}
	return list;
}

QWidget* WidgetListWidgetViewport::getWidget(int index) const 
{
	if (index < layout_->count() - 1) {
		return layout_->itemAt(index)->widget();
	}
	return nullptr;
}

int WidgetListWidgetViewport::widgetCount() const
{
	return layout_->count()-1;
}

void WidgetListWidgetViewport::hasScrollChanged(bool hasScroll)
{
	if (hasScroll) {
		// Fix sizer with 0 height
		_bottomspacer->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);
	}
	else {
		// Make spacer expading
		_bottomspacer->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	}
}

} }