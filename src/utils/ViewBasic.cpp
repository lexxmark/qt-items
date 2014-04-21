#include "ViewBasic.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewText::ViewText()
{
}

void ViewText::drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const
{
    QString text = tr("Cell [%1, %2]").arg(cell.row).arg(cell.column);
    painter->drawText(rect, Qt::AlignLeft | Qt::TextSingleLine | Qt::AlignVCenter, text);
}

QSize ViewText::sizeHintImpl(const QWidget* widget, const CellID& cell) const
{
    QString text = tr("Cell [%1, %2]").arg(cell.row).arg(cell.column);
    return widget->fontMetrics().size(Qt::TextSingleLine, text);
}

bool ViewText::textImpl(const CellID& cell, QString& text) const
{
    text = tr("Cell [%1, %2]").arg(cell.row).arg(cell.column);
    return true;
}

ViewCheck::ViewCheck()
{
}

void ViewCheck::drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const
{
    bool check = cell.row % 2;

    QStyleOptionButton styleOption;
    styleOption.initFrom(widget);
    styleOption.rect = rect;
    styleOption.state = QStyle::State_Enabled | (check ? QStyle::State_On : QStyle::State_Off);
    
    widget->style()->drawControl(QStyle::CE_CheckBox, &styleOption, painter, widget);
}

QSize ViewCheck::sizeHintImpl(const QWidget* widget, const CellID& cell) const
{
    bool check = cell.row % 2;

    QStyleOptionButton styleOption;
    styleOption.initFrom(widget);
    styleOption.state = QStyle::State_Enabled | (check ? QStyle::State_On : QStyle::State_Off);
    return widget->style()->sizeFromContents(QStyle::CT_CheckBox, &styleOption, QSize(0, 0), widget);
}

bool ViewCheck::textImpl(const CellID& cell, QString& text) const
{
    return false;
}

ViewRadio::ViewRadio()
{
}

void ViewRadio::drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const
{
    bool check = (cell.row == 1);

    QStyleOptionButton styleOption;
    styleOption.initFrom(widget);
    styleOption.rect = rect;
    styleOption.state = QStyle::State_Enabled | (check ? QStyle::State_On : QStyle::State_Off);

    widget->style()->drawControl(QStyle::CE_RadioButton, &styleOption, painter, widget);
}

QSize ViewRadio::sizeHintImpl(const QWidget* widget, const CellID& cell) const
{
    bool check = (cell.row == 1);

    QStyleOptionButton styleOption;
    styleOption.initFrom(widget);
    styleOption.state = QStyle::State_Enabled | (check ? QStyle::State_On : QStyle::State_Off);
    return widget->style()->sizeFromContents(QStyle::CT_RadioButton, &styleOption, QSize(0, 0), widget);
}

bool ViewRadio::textImpl(const CellID& cell, QString& text) const
{
    return false;
}

} // end namespace Qi
