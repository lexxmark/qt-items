#include "CacheGrid.h"

namespace Qi
{

CacheGrid::CacheGrid(const Grid& grid)
    : m_grid(grid),
      m_frame(0, 0, 0, 0),
      m_isFrameValid(false)
{
    connect(&m_grid, SIGNAL(gridChanged(const Grid&, ChangeReason)), this, SLOT(onGridChanged(const Grid&, ChangeReason)));
}

CacheGrid::~CacheGrid()
{
    disconnect(&m_grid, SIGNAL(gridChanged(const Grid&, ChangeReason)), this, SLOT(onGridChanged(const Grid&, ChangeReason)));
}

const QRect& CacheGrid::frame() const
{
    return m_frame;
}

void CacheGrid::setFrame(const QRect& frame)
{
    if (frame == m_frame)
        return;
    
    m_frame = frame;
    m_isFrameValid = false;
}

void CacheGrid::validateFrame() const
{
    if (m_isFrameValid)
        return;
    
    CellID newStartVisibleCell = m_grid.findVisCell(m_frame.topLeft());
    CellID newEndVisibleCell = m_grid.findVisCell(m_frame.bottomRight());
    
    if (newStartVisibleCell == m_startVisibleCell && newEndVisibleCell == m_endVisibleCell)
    {
        m_isFrameValid = true;
        return;
    }
    
    Q_ASSERT(newStartVisibleCell.row <= newEndVisibleCell.row);
    Q_ASSERT(newStartVisibleCell.column <= newEndVisibleCell.column);

    std::vector<CacheCell> cells;
    cells.reserve((newEndVisibleCell.row - newStartVisibleCell.row + 1)*(newEndVisibleCell.column - newStartVisibleCell.column + 1));
    for (CellID visibleCell = newStartVisibleCell; visibleCell.row <= newEndVisibleCell.row; ++visibleCell.row)
        for (visibleCell.row = newStartVisibleCell.row; visibleCell.column <= newEndVisibleCell.column; ++visibleCell.column)
        {
            CellID absCell = m_grid.toAbsolute(visibleCell);
            cells.push_back(CacheCell(absCell));
        }
    
    std::swap(m_visibleCells, cells);
}

void CacheGrid::onGridChanged(const Grid& grid, ChangeReason reason)
{
    Q_ASSERT(&m_grid == &grid);
    m_isFrameValid = false;
}

} // end namespace Qi
