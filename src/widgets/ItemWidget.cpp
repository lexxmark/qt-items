#include "ItemWidget.h"
#include <QPainter>

namespace Qi
{

ItemWidget::ItemWidget(QWidget* parent)
    : QWidget(parent)
{
}

ItemWidget::~ItemWidget()
{
}

void ItemWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
 
    painter.drawRect(rect());
    painter.drawText(rect(), Qt::AlignCenter, tr("ItemWidget"));
}

}
