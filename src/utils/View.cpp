#include "View.h"

namespace Qi
{

View::View(QObject* parent)
    : QObject(parent)
{
}

View::~View()
{
}

void View::draw(QPainter& painter, const CellID& cell, const QRect& rect) const
{
    drawImpl(painter, cell, rect);
}

QSize View::sizeHint(QPainter& painter, const CellID& cell) const
{
    return sizeHintImpl(painter, cell);
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

void View::drawImpl(QPainter& painter, const CellID& cell, const QRect& rect) const
{
    // do nothing
}

QSize View::sizeHintImpl(QPainter& painter, const CellID& cell) const
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
