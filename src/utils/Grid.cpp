#include "Grid.h"

namespace Qi
{

Grid::Grid()
    : m_rows(new Lines()),
      m_columns(new Lines())
{
    connectLinesSignal();
}

Grid::Grid(const QSharedPointer<Lines>& rows, const QSharedPointer<Lines>& columns)
    : m_rows(rows),
      m_columns(columns)
{
    connectLinesSignal();
}

Grid::~Grid()
{
    disconnectLinesSignal();
}

const Lines& Grid::rows() const
{
    return *m_rows;
}

const Lines& Grid::columns() const
{
    return *m_columns;
}

Lines& Grid::rows()
{
    return *m_rows;
}

Lines& Grid::columns()
{
    return *m_columns;
}

const QSharedPointer<Lines>& Grid::rowsPtr() const
{
    return m_rows;
}

const QSharedPointer<Lines>& Grid::columnsPtr() const
{
    return m_columns;
}

QSize Grid::dim() const
{
    return QSize(columns().count(), rows().count());
}

QSize Grid::visibleDim() const
{
    return QSize(columns().visibleCount(), rows().visibleCount());
}

QSize Grid::visibleSize() const
{
    return QSize(columns().visibleSize(), rows().visibleSize());
}

CellID Grid::toVisible(CellID absoluteCell) const
{
    return CellID(rows().toVisible(absoluteCell.row), columns().toVisible(absoluteCell.column));
}

CellID Grid::toVisibleSafe(CellID absoluteCell) const
{
    return CellID(rows().toVisibleSafe(absoluteCell.row), columns().toVisibleSafe(absoluteCell.column));
}

CellID Grid::toAbsolute(CellID visibleCell) const
{
    return CellID(rows().toAbsolute(visibleCell.row), columns().toAbsolute(visibleCell.column));
}

CellID Grid::toAbsoluteSafe(CellID visibleCell) const
{
    return CellID(rows().toAbsoluteSafe(visibleCell.row), columns().toAbsoluteSafe(visibleCell.column));
}

CellID Grid::findVisCell(QPoint point) const
{
    CellID cell;
    if (visibleDim().isEmpty())
        return cell;
    
    if (point.y() <= 0)
        cell.row = 0;
    else
        cell.row = rows().findVisLine(point.y());
    
    if (point.x() <= 0)
        cell.column = 0;
    else
        cell.column = columns().findVisLine(point.x());

    return cell;
}

CellID Grid::findVisCellExact(QPoint point) const
{
    CellID cell;
    if (visibleDim().isEmpty())
        return cell;

    QSize size = visibleSize();

    if (point.y() > 0 && point.y() < size.height())
        cell.row = rows().findVisLine(point.y());
    
    if (point.x() > 0 && point.x() < size.width())
        cell.column = columns().findVisLine(point.x());
    
    return cell;
}

void Grid::addCellsInfo(const QSharedPointer<Range>& range, const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout)
{
    CellsInfo info;
    info.range = range;
    info.view = view;
    info.layout = layout;
    m_cellsInfo.push_back(info);
    
    emit gridChanged(*this, ChangeReasonGridCellsInfo);    
}

void Grid::connectLinesSignal()
{
    connect(m_rows.data(), SIGNAL(linesChanged(const Lines&, ChangeReason)), this, SLOT(onLinesChanged(const Lines&, ChangeReason)));
    connect(m_columns.data(), SIGNAL(linesChanged(const Lines&, ChangeReason)), this, SLOT(onLinesChanged(const Lines&, ChangeReason)));
}

void Grid::disconnectLinesSignal()
{
    disconnect(m_rows.data(), SIGNAL(linesChanged(const Lines&, ChangeReason)), this, SLOT(onLinesChanged(const Lines&, ChangeReason)));
    disconnect(m_columns.data(), SIGNAL(linesChanged(const Lines&, ChangeReason)), this, SLOT(onLinesChanged(const Lines&, ChangeReason)));
}

void Grid::onLinesChanged(const Lines& lines, ChangeReason reason)
{
    emit gridChanged(*this, reason);
}

} // end namespace Qi
