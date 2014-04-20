#include "ItemWidget.h"
#include "ItemWidget_p.h"

namespace Qi
{

ItemWidget::ItemWidget(QWidget* parent)
    : QWidget(parent)
{
    d.reset(new ItemWidgetPrivate(this));
    setMouseTracking(true);
}

ItemWidget::~ItemWidget()
{
}

void ItemWidget::addViewSchema(QSharedPointer<View> view, QSharedPointer<Layout> layout)
{
    d->addViewSchema(view, layout);
}

QSize ItemWidget::sizeHint() const
{
    return d->doSizeHint();
}

QSize ItemWidget::minimumSizeHint() const
{
    return d->doSizeHint();
}

void ItemWidget::paintEvent(QPaintEvent* event)
{
    d->doPaintEvent(event);
}

void ItemWidget::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    d->doResizeEvent(event);
}

} // end namespace Qi
