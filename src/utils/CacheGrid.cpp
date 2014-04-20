#include "CacheGrid.h"

namespace Qi
{

CacheGrid::CacheGrid(const Grid& grid)
    : m_grid(grid),
      m_frame(0, 0, 0, 0),
      m_isFrameValid(false)
{
    connect(&m_grid, SIGNAL(gridChanged(const Grid*, ChangeReason)), this, SLOT(onGridChanged(const Grid*, ChangeReason)));
}

CacheGrid::~CacheGrid()
{
    disconnect(&m_grid, SIGNAL(gridChanged(const Grid*, ChangeReason)), this, SLOT(onGridChanged(const Grid*, ChangeReason)));
}

const QRect& CacheGrid::frame() const
{
    return m_frame;
}

bool CacheGrid::setFrame(const QRect& frame)
{
    if (frame == m_frame)
        return false;
    
    m_frame = frame;
    m_isFrameValid = false;

    return true;
}

const CacheCell* CacheGrid::cacheCell(const CellID& visibleCell) const
{
    validateFrame();

    if (visibleCell.row < m_startVisibleCell.row || visibleCell.row > m_endVisibleCell.row)
        return nullptr;

    if (visibleCell.column < m_startVisibleCell.column || visibleCell.column > m_endVisibleCell.column)
        return nullptr;

    quint32 visibleColumns = m_endVisibleCell.column - m_startVisibleCell.column + 1;
    quint32 index = (visibleCell.row - m_startVisibleCell.row) * visibleColumns + visibleCell.column - m_startVisibleCell.column;
    Q_ASSERT(index < m_visibleCells.size());
    return &m_visibleCells[index];
}

void CacheGrid::draw(QPainter* painter, const QWidget* widget) const
{
    validateFrame();

    if (m_visibleCells.empty())
        return;

    QRect oldWindow = painter->window();
    painter->setWindow(m_frame);

    for (const auto& cacheCell: m_visibleCells)
    {
        cacheCell.draw(painter, widget);
    }

    painter->setWindow(oldWindow);
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

    const Lines& rows = m_grid.rows();
    const Lines& columns = m_grid.columns();

    std::vector<CacheCell> cells;
    cells.reserve((newEndVisibleCell.row - newStartVisibleCell.row + 1)*(newEndVisibleCell.column - newStartVisibleCell.column + 1));
    for (CellID visibleCell = newStartVisibleCell; visibleCell.row <= newEndVisibleCell.row; ++visibleCell.row)
    {
        for (visibleCell.column = newStartVisibleCell.column; visibleCell.column <= newEndVisibleCell.column; ++visibleCell.column)
        {
            CellID absCell = m_grid.toAbsolute(visibleCell);
            cells.push_back(CacheCell(absCell));

            CacheCell& cacheCell = cells.back();
            std::vector<ViewSchema> views;
            QRect cellRect(0, 0, 0, 0);
            if (visibleCell.row > 0)
                cellRect.setTop(rows.sizeAtVisLine(visibleCell.row - 1));
            if (visibleCell.column > 0)
                cellRect.setLeft(columns.sizeAtVisLine(visibleCell.column - 1));
            cellRect.setBottom(rows.sizeAtVisLine(visibleCell.row));
            cellRect.setRight(columns.sizeAtVisLine(visibleCell.column));

            for (const auto& cellsSchema: m_grid.cellsSchemas())
            {
                if (!cellsSchema.range->hasCell(absCell))
                    continue;

                views.push_back(cellsSchema.vSchema);
            }

            cacheCell.reinit(views, cellRect);
        }
    }

    std::swap(m_visibleCells, cells);
}

void CacheGrid::onGridChanged(const Grid* grid, ChangeReason reason)
{
    Q_ASSERT(&m_grid == grid);
    m_isFrameValid = false;
}

} // end namespace Qi
