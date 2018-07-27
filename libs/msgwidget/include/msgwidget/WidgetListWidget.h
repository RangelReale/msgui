
#ifndef H__WIDGETLISTWIDGET__H
#define H__WIDGETLISTWIDGET__H

#include <QScrollArea>

namespace msgwidget {

class WidgetListWidgetPrivate;

class WidgetListWidget : public QScrollArea
{
    Q_OBJECT
public:
	WidgetListWidget(QWidget* parent = 0);
	virtual ~WidgetListWidget();

	bool scrollToBottomOnAdd() const;
	void setScrollToBottomOnAdd(bool value);

	void layoutSetSpacing(int spacing);

	void clear();
	void addWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = 0);
	void insertWidget(int index, QWidget* widget, int stretch = 0,
		Qt::Alignment alignment = 0);
	void removeWidget(QWidget* widget);

	QList<QWidget*> getWidgetList() const;
	QWidget* getWidget(int index) const;
	int widgetCount() const;
private slots:
	void scrollToBottom(int, int);
protected:
	void resizeEvent(QResizeEvent *event) override;
private:
	WidgetListWidgetPrivate *d;
};

}

#endif // H__WIDGETLISTWIDGET__H
