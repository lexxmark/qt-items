#include "ViewBasic.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewText::ViewText()
{
}

void ViewText::drawImpl(DrawContext& dc, const CellID& cell, const QRect& rect) const
{
    QString text = tr("Cell [%1, %2]").arg(cell.row).arg(cell.column);
    dc.painter->drawText(rect, Qt::AlignLeft | Qt::TextSingleLine | Qt::AlignVCenter, text);
}

QSize ViewText::sizeHintImpl(DrawContext& dc, const CellID& cell) const
{
    QString text = tr("Cell [%1, %2]").arg(cell.row).arg(cell.column);
    return dc.painter->fontMetrics().size(Qt::TextSingleLine, text);
}

bool ViewText::textImpl(const CellID& cell, QString& text) const
{
    text = tr("Cell [%1, %2]").arg(cell.row).arg(cell.column);
    return true;
}

ViewCheck::ViewCheck()
{
}

void ViewCheck::drawImpl(DrawContext& dc, const CellID& cell, const QRect& rect) const
{
    bool check = cell.row % 2;

    QStyleOptionButton styleOption;
    styleOption.initFrom(dc.widget);
    styleOption.state = QStyle::State_Enabled | check ? QStyle::State_On : QStyle::State_Off;
    
    dc.style->drawControl(QStyle::CE_CheckBox, &styleOption, dc.painter, dc.widget);
}

QSize ViewCheck::sizeHintImpl(DrawContext& dc, const CellID& cell) const
{
    bool check = cell.row % 2;

    QStyleOptionButton styleOption;
    styleOption.initFrom(dc.widget);
    styleOption.state = QStyle::State_Enabled | check ? QStyle::State_On : QStyle::State_Off;
    return dc.style->sizeFromContents(QStyle::CT_CheckBox, &styleOption, QSize(0, 0), dc.widget);
}

bool ViewCheck::textImpl(const CellID& cell, QString& text) const
{
    return false;
}

}
