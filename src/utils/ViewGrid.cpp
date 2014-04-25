#include "ViewGrid.h"
#include "CacheGrid.h"

namespace Qi
{

ViewGrid::ViewGrid(Grid* grid)
    : m_grid(grid)
{
    Q_ASSERT(m_grid);
    m_cache.reset(new CacheGrid(m_grid));
}

ViewGrid::~ViewGrid()
{
}

void ViewGrid::setGrid(Grid* grid)
{
    Q_ASSERT(m_grid);

    m_grid = grid;
    m_cache.reset(new CacheGrid(m_grid));

    emit viewChanged(this);
}

const QRect& ViewGrid::frame() const
{
    return m_cache->frame();
}

void ViewGrid::setFrame(const QRect& frame)
{
    if (m_cache->setFrame(frame))
    {
        emit viewChanged(this);
    }
}

void ViewGrid::drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const
{
    m_cache->draw(painter, widget);
}

QSize ViewGrid::sizeHintImpl(const QWidget* widget, const CellID& cell) const
{
    // returns size of the grid
    return m_grid->visibleSize();
}

bool ViewGrid::textImpl(const CellID& cell, QString& text) const
{
    // no text representation
    return false;
}

} // end namespace Qi
