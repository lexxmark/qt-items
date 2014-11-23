#include "Ranges.h"

namespace Qi
{

RangeSelection& RangeSelection::operator=(const RangeSelection& other)
{
    m_ranges = other.m_ranges;
    emit rangeChanged(this, ChangeReasonRange);

    return *this;
}

void RangeSelection::clear()
{
    m_ranges.clear();
    emit rangeChanged(this, ChangeReasonRange);
}

void RangeSelection::addRange(const QSharedPointer<Range>& range, bool exclude)
{
    RangeInfo info = { range, exclude };
    m_ranges.append(info);
    emit rangeChanged(this, ChangeReasonRange);
}

bool RangeSelection::hasItemImpl(const ItemID &item) const
{
    bool excluded = true;
    for (const auto& range: m_ranges)
    {
        if (range.range->hasItem(item))
        {
            excluded = range.exclude;
        }
    }

    return !excluded;
}

RangeNone::RangeNone()
{
}

bool RangeNone::hasItemImpl(const ItemID& item) const
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

bool RangeAll::hasItemImpl(const ItemID& item) const
{
    return true;
}

QSharedPointer<RangeAll> makeRangeAll()
{
    return QSharedPointer<RangeAll>::create();
}

RangeColumn::RangeColumn(int column)
    : m_column(column)
{
}

void RangeColumn::setColumn(int column)
{
    if (column != m_column)
    {
        m_column = column;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeColumn::hasItemImpl(const ItemID& item) const
{
    return item.column == m_column;
}

QSharedPointer<RangeColumn> makeRangeColumn(int column)
{
    return QSharedPointer<RangeColumn>::create(column);
}

RangeColumns::RangeColumns(const QSet<int>& columns)
    : m_columns(columns)
{
}

RangeColumns::RangeColumns(int columnBegin, int columnEnd)
{
    Q_ASSERT(columnBegin <= columnEnd);
    for (int column = columnBegin; column != columnEnd; ++column)
        m_columns.insert(column);
}

void RangeColumns::setColumns(const QSet<int>& columns)
{
    if (m_columns != columns)
    {
        m_columns = columns;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeColumns::hasItemImpl(const ItemID& item) const
{
    return m_columns.contains(item.column);
}

QSharedPointer<RangeColumns> makeRangeColumns(const QSet<int>& columns)
{
    return QSharedPointer<RangeColumns>::create(columns);
}

QSharedPointer<RangeColumns> makeRangeColumns(int columnBegin, int columnEnd)
{
    return QSharedPointer<RangeColumns>(new RangeColumns(columnBegin, columnEnd));
}

RangeRow::RangeRow(int row)
    : m_row(row)
{
}

void RangeRow::setRow(int row)
{
    if (row != m_row)
    {
        m_row = row;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeRow::hasItemImpl(const ItemID& item) const
{
    return item.row == m_row;
}

QSharedPointer<RangeRow> makeRangeRow(int row)
{
    return QSharedPointer<RangeRow>::create(row);
}

RangeRows::RangeRows(const QSet<int>& rows)
    : m_rows(rows)
{
}

RangeRows::RangeRows(int rowBegin, int rowEnd)
{
    Q_ASSERT(rowBegin <= rowEnd);
    for (int row = rowBegin; row != rowEnd; ++row)
        m_rows.insert(row);
}

void RangeRows::setRows(const QSet<int>& rows)
{
    if (m_rows != rows)
    {
        m_rows = rows;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeRows::hasItemImpl(const ItemID& item) const
{
    return m_rows.contains(item.row);
}

QSharedPointer<RangeRows> makeRangeRows(const QSet<int>& rows)
{
    return QSharedPointer<RangeRows>::create(rows);
}

QSharedPointer<RangeRows> makeRangeRows(int rowBegin, int rowEnd)
{
    return QSharedPointer<RangeRows>::create(rowBegin, rowEnd);
}

RangeRect::RangeRect(const QSet<int>& rows, const QSet<int>& columns)
    : m_rows(rows),
      m_columns(columns)
{
}

RangeRect::RangeRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd)
{
    Q_ASSERT(rowBegin <= rowEnd);
    Q_ASSERT(columnBegin <= columnEnd);

    for (; rowBegin < rowEnd; ++rowBegin)
        m_rows.insert(rowBegin);

    for (; columnBegin < columnEnd; ++columnBegin)
        m_columns.insert(columnBegin);
}

void RangeRect::setRows(const QSet<int>& rows)
{
    if (m_rows != rows)
    {
        m_rows = rows;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

void RangeRect::setColumns(const QSet<int>& columns)
{
    if (m_columns != columns)
    {
        m_columns = columns;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeRect::hasItemImpl(const ItemID& item) const
{
    return m_rows.contains(item.row) && m_columns.contains(item.column);
}

QSharedPointer<RangeRect> makeRangeRect(const QSet<int>& rows, const QSet<int>& columns)
{
    return QSharedPointer<RangeRect>::create(rows, columns);
}

QSharedPointer<RangeRect> makeRangeRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd)
{
    return QSharedPointer<RangeRect>::create(rowBegin, rowEnd, columnBegin, columnEnd);
}

} // end namespace Qi
