#include "RangesBasic.h"

namespace Qi
{

RangeNone::RangeNone(QObject *parent)
    : Range(parent)
{
}

bool RangeNone::HasCellImpl(const CellID &cell) const
{
    return false;
}

RangeNone* makeRangeNone(QObject* parent)
{
    return new RangeNone(parent);
}

RangeAll::RangeAll(QObject *parent)
    : Range(parent)
{
}

bool RangeAll::HasCellImpl(const CellID &cell) const
{
    return true;
}

RangeAll* makeRangeAll(QObject* parent)
{
    return new RangeAll(parent);
}

RangeColumn::RangeColumn(quint32 column, QObject* parent)
    : Range(parent),
      m_column(column)
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

bool RangeColumn::HasCellImpl(const CellID &cell) const
{
    return cell.column == m_column;
}

RangeColumn* makeRangeColumn(quint32 column, QObject* parent)
{
    return new RangeColumn(column, parent);
}

RangeColumns::RangeColumns(const QSet<quint32>& columns, QObject* parent)
    : Range(parent),
      m_columns(columns)
{
}

RangeColumns::RangeColumns(quint32 columnBegin, quint32 columnEnd, QObject* parent)
    : Range(parent)
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

bool RangeColumns::HasCellImpl(const CellID &cell) const
{
    return m_columns.contains(cell.column);
}

RangeColumns* makeRangeColumns(const QSet<quint32>& columns, QObject* parent)
{
    return new RangeColumns(columns, parent);
}

RangeColumns* makeRangeColumns(quint32 columnBegin, quint32 columnEnd, QObject* parent)
{
    return new RangeColumns(columnBegin, columnEnd, parent);
}

RangeRow::RangeRow(quint32 row, QObject* parent)
    : Range(parent),
      m_row(row)
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

bool RangeRow::HasCellImpl(const CellID &cell) const
{
    return cell.row == m_row;
}

RangeRow* makeRangeRow(quint32 row, QObject* parent)
{
    return new RangeRow(row, parent);
}

RangeRows::RangeRows(const QSet<quint32>& rows, QObject* parent)
    : Range(parent),
      m_rows(rows)
{
}

RangeRows::RangeRows(quint32 rowBegin, quint32 rowEnd, QObject* parent)
    : Range(parent)
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

bool RangeRows::HasCellImpl(const CellID &cell) const
{
    return m_rows.contains(cell.row);
}

RangeRows* makeRangeRows(const QSet<quint32>& rows, QObject* parent)
{
    return new RangeRows(rows, parent);
}

RangeRows* makeRangeRows(quint32 rowBegin, quint32 rowEnd, QObject* parent)
{
    return new RangeRows(rowBegin, rowEnd, parent);
}

} // end namespace Qi
