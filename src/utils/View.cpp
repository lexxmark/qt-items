#include "View.h"

namespace Qi
{

View::View()
{
}

View::~View()
{
}

void View::draw(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const
{
    drawImpl(painter, widget, cell, rect);
}

QSize View::sizeHint(const QWidget* widget, const CellID& cell) const
{
    return sizeHintImpl(widget, cell);
}

bool View::text(const CellID& cell, QString& text) const
{
    return textImpl(cell, text);
}

bool View::tooltipText(const CellID& cell, QString& text) const
{
    if (tooltipTextCallback)
        return tooltipTextCallback(cell, text);
    else
        return false;
}

void View::drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const
{
    // do nothing
}

QSize View::sizeHintImpl(const QWidget* widget, const CellID& cell) const
{
    // returns an empty size hint
    return QSize(0, 0);
}

bool View::textImpl(const CellID& cell, QString& text) const
{
    // no text representation
    return false;
}

} // end namespace Qi
