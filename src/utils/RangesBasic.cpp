#include "RangesBasic.h"

namespace Qi
{

RangeNone::RangeNone()
{
}

bool RangeNone::hasCellImpl(const CellID &cell) const
{
    return false;
}

QSharedPointer<RangeNone> makeRangeNone()
{
    return QSharedPointer<RangeNone>::create();
}

RangeAll::RangeAll()
{
}

bool RangeAll::hasCellImpl(const CellID &cell) const
{
    return true;
}

QSharedPointer<RangeAll> makeRangeAll()
{
    return QSharedPointer<RangeAll>::create();
}

RangeColumn::RangeColumn(quint32 column)
    : m_column(column)
{
}

void RangeColumn::setColumn(quint32 column)
{
    if (column != m_column)
    {
        m_column = column;
        emit rangeChanged(*this);
    }
}

bool RangeColumn::hasCellImpl(const CellID &cell) const
{
    return cell.column == m_column;
}

QSharedPointer<RangeColumn> makeRangeColumn(quint32 column)
{
    return QSharedPointer<RangeColumn>::create(column);
}

RangeColumns::RangeColumns(const QSet<quint32>& columns)
    : m_columns(columns)
{
}

RangeColumns::RangeColumns(quint32 columnBegin, quint32 columnEnd)
{
    Q_ASSERT(columnBegin <= columnEnd);
    for (quint32 column = columnBegin; column != columnEnd; ++column)
        m_columns.insert(column);
}

void RangeColumns::setColumns(const QSet<quint32>& columns)
{
    if (m_columns != columns)
    {
        m_columns = columns;
        emit rangeChanged(*this);
    }
}

bool RangeColumns::hasCellImpl(const CellID &cell) const
{
    return m_columns.contains(cell.column);
}

QSharedPointer<RangeColumns> makeRangeColumns(const QSet<quint32>& columns)
{
    return QSharedPointer<RangeColumns>::create(columns);
}

QSharedPointer<RangeColumns> makeRangeColumns(quint32 columnBegin, quint32 columnEnd)
{
    return QSharedPointer<RangeColumns>::create(columnBegin, columnEnd);
}

RangeRow::RangeRow(quint32 row)
    : m_row(row)
{
}

void RangeRow::setRow(quint32 row)
{
    if (row != m_row)
    {
        m_row = row;
        emit rangeChanged(*this);
    }
}

bool RangeRow::hasCellImpl(const CellID &cell) const
{
    return cell.row == m_row;
}

QSharedPointer<RangeRow> makeRangeRow(quint32 row)
{
    return QSharedPointer<RangeRow>::create(row);
}

RangeRows::RangeRows(const QSet<quint32>& rows)
    : m_rows(rows)
{
}

RangeRows::RangeRows(quint32 rowBegin, quint32 rowEnd)
{
    Q_ASSERT(rowBegin <= rowEnd);
    for (quint32 row = rowBegin; row != rowEnd; ++row)
        m_rows.insert(row);
}

void RangeRows::setRows(const QSet<quint32>& rows)
{
    if (m_rows != rows)
    {
        m_rows = rows;
        emit rangeChanged(*this);
    }
}

bool RangeRows::hasCellImpl(const CellID &cell) const
{
    return m_rows.contains(cell.row);
}

QSharedPointer<RangeRows> makeRangeRows(const QSet<quint32>& rows)
{
    return QSharedPointer<RangeRows>::create(rows);
}

QSharedPointer<RangeRows> makeRangeRows(quint32 rowBegin, quint32 rowEnd)
{
    return QSharedPointer<RangeRows>::create(rowBegin, rowEnd);
}

} // end namespace Qi
