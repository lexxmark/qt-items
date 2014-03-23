#include "GridWidget.h"
#include <QPainter>

namespace Qi
{

GridWidget::GridWidget(QWidget* parent)
    : QWidget(parent)
{
}

GridWidget::~GridWidget()
{
}

void GridWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
 
    painter.drawRect(rect());
    painter.drawText(rect(), Qt::AlignCenter, tr("GridWidget"));
}

}
