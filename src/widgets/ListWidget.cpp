#include "ListWidget.h"
#include <QPainter>

namespace Qi
{

ListWidget::ListWidget(QWidget* parent)
    : QWidget(parent)
{
}

ListWidget::~ListWidget()
{
}

void ListWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
 
    painter.drawRect(rect());
    painter.drawText(rect(), Qt::AlignCenter, tr("ListWidget"));
}

} // end namespace Qi
