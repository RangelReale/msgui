
#include "msgwidget/WidgetListWidget.h"
#include "msgwidget/internal/WidgetListWidgetViewport.h"

#include <QScrollBar>

namespace msgwidget {

// Private
class WidgetListWidgetPrivate
{
public:
	WidgetListWidget *widget;
	internal::WidgetListWidgetViewport* viewport;
	bool scroll_to_bottom_on_add;
	bool is_scroll_add;

	WidgetListWidgetPrivate(WidgetListWidget* widget) : 
		widget(widget), viewport(new internal::WidgetListWidgetViewport(widget)),
		is_scroll_add(false), scroll_to_bottom_on_add(false)
	{
		Q_ASSERT(widget);
	}
};

// Public

WidgetListWidget::WidgetListWidget(QWidget* parent) :
	QScrollArea(parent),
	d(new WidgetListWidgetPrivate(this))
{
	this->setWidget(d->viewport);
	this->setBackgroundRole(QPalette::Window);
	this->setFrameShadow(QFrame::Plain);
	this->setFrameShape(QFrame::NoFrame);
	this->setWidgetResizable(true);

	connect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(scrollToBottom(int, int)));
}

WidgetListWidget::~WidgetListWidget()
{
	delete d;
}

bool WidgetListWidget::scrollToBottomOnAdd() const
{
	return d->scroll_to_bottom_on_add;
}

void WidgetListWidget::setScrollToBottomOnAdd(bool value)
{
	d->scroll_to_bottom_on_add = value;
	if (value)
		d->is_scroll_add = false;
}


void WidgetListWidget::layoutSetSpacing(int spacing)
{
	d->viewport->layoutSetSpacing(spacing);
}

void WidgetListWidget::clear()
{
	d->viewport->clear();
}

void WidgetListWidget::addWidget(QWidget* widget, int stretch, Qt::Alignment alignment)
{
	d->is_scroll_add = true;
	d->viewport->addWidget(widget, stretch, alignment);
}

void WidgetListWidget::insertWidget(int index, QWidget* widget, int stretch,
	Qt::Alignment alignment)
{
	d->is_scroll_add = true;
	d->viewport->insertWidget(index, widget, stretch, alignment);
}

void WidgetListWidget::removeWidget(QWidget* widget)
{
	d->viewport->removeWidget(widget);
}

QList<QWidget*> WidgetListWidget::getWidgetList() const
{
	return d->viewport->getWidgetList();
}

QWidget* WidgetListWidget::getWidget(int index) const
{
	return d->viewport->getWidget(index);
}

int WidgetListWidget::widgetCount() const
{
	return d->viewport->widgetCount();
}

void WidgetListWidget::resizeEvent(QResizeEvent *event)
{
	QScrollArea::resizeEvent(event);
	d->viewport->hasScrollChanged(verticalScrollBar()->isVisible());
}

void WidgetListWidget::scrollToBottom(int min, int max)
{
	if (d->scroll_to_bottom_on_add && d->is_scroll_add && max > verticalScrollBar()->value()) {
		d->is_scroll_add = false;
		verticalScrollBar()->setValue(max);
	}
	d->viewport->hasScrollChanged(verticalScrollBar()->isVisible());
}

}

