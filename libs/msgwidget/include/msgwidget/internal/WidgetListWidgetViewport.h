#ifndef H__WIDGETLISTWIDGETVIEWPORT__H
#define H__WIDGETLISTWIDGETVIEWPORT__H

#include <QWidget>
#include <QSpacerItem>

class QVBoxLayout;

namespace msgwidget {
namespace internal {

class WidgetListWidgetViewport : public QWidget {
	Q_OBJECT
public:
	WidgetListWidgetViewport(QWidget *parent = 0);
	virtual ~WidgetListWidgetViewport();

	void layoutSetSpacing(int spacing);

	void clear();
	void addWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = 0);
	void insertWidget(int index, QWidget* widget, int stretch = 0,
		Qt::Alignment alignment = 0);
	void removeWidget(QWidget* widget);

	QList<QWidget*> getWidgetList() const;
	QWidget* getWidget(int index) const;
	int widgetCount() const;

	void hasScrollChanged(bool hasScroll);
private:
	QVBoxLayout* layout_;
	QSpacerItem* _bottomspacer;
};

} }

#endif // H__WIDGETLISTWIDGETVIEWPORT__H
