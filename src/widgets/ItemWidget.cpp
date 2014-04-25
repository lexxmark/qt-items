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

void ItemWidget::addViewSchema(Layout* layout, View* view, ControllerMouse* controller)
{
    d->addViewSchema(layout, view, controller);
}

void ItemWidget::clearViewSchemas()
{
    d->clearViewSchemas();
}

void ItemWidget::setCell(const CellID& cell)
{
    d->setCell(cell);
}

QSize ItemWidget::sizeHint() const
{
    return d->sizeHint();
}

QSize ItemWidget::minimumSizeHint() const
{
    return d->sizeHint();
}

bool ItemWidget::event(QEvent* event)
{
    bool result = QWidget::event(event);
    d->event(event);
    return result;
}

/*
void ItemWidget::paintEvent(QPaintEvent* event)
{
    d->doPaintEvent(event);
}

void ItemWidget::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    d->doResizeEvent(event);
}
*/
} // end namespace Qi
