#include "View.h"

namespace Qi
{

View::View()
{
}

View::~View()
{
}

void View::draw(DrawContext &dc, const CellID& cell, const QRect& rect) const
{
    drawImpl(dc, cell, rect);
}

QSize View::sizeHint(DrawContext &dc, const CellID& cell) const
{
    return sizeHintImpl(dc, cell);
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

void View::drawImpl(DrawContext& dc, const CellID& cell, const QRect& rect) const
{
    // do nothing
}

QSize View::sizeHintImpl(DrawContext& dc, const CellID& cell) const
{
    // returns an empty size hint
    return QSize(0, 0);
}

bool View::textImpl(const CellID& cell, QString& text) const
{
    // no text representation
    return false;
}

}
