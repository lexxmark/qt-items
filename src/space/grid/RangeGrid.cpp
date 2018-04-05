/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "RangeGrid.h"

namespace Qi
{

RangeGridColumn::RangeGridColumn(int column)
    : m_column(column)
{
}

void RangeGridColumn::setColumn(int column)
{
    if (column != m_column)
    {
        m_column = column;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeGridColumn::hasItemImpl(GridID id) const
{
    return id.column == m_column;
}

SharedPtr<RangeGridColumn> makeRangeGridColumn(int column)
{
    return makeShared<RangeGridColumn>(column);
}

RangeGridColumns::RangeGridColumns(QSet<int> columns)
    : m_columns(std::move(columns))
{
}

RangeGridColumns::RangeGridColumns(int columnBegin, int columnEnd)
{
    Q_ASSERT(columnBegin <= columnEnd);
    for (int column = columnBegin; column != columnEnd; ++column)
        m_columns.insert(column);
}

void RangeGridColumns::setColumns(QSet<int> columns)
{
    if (m_columns != columns)
    {
        m_columns = std::move(columns);
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeGridColumns::hasItemImpl(GridID id) const
{
    return m_columns.contains(id.column);
}

SharedPtr<RangeGridColumns> makeRangeGridColumns(const QSet<int>& columns)
{
    return makeShared<RangeGridColumns>(columns);
}

SharedPtr<RangeGridColumns> makeRangeGridColumns(int columnBegin, int columnEnd)
{
    return makeShared<RangeGridColumns>(columnBegin, columnEnd);
}

RangeGridRow::RangeGridRow(int row)
    : m_row(row)
{
}

void RangeGridRow::setRow(int row)
{
    if (row != m_row)
    {
        m_row = row;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeGridRow::hasItemImpl(GridID id) const
{
    return id.row == m_row;
}

SharedPtr<RangeGridRow> makeRangeGridRow(int row)
{
    return makeShared<RangeGridRow>(row);
}

RangeGridRows::RangeGridRows(QSet<int> rows)
    : m_rows(std::move(rows))
{
}

RangeGridRows::RangeGridRows(int rowBegin, int rowEnd)
{
    Q_ASSERT(rowBegin <= rowEnd);
    for (int row = rowBegin; row != rowEnd; ++row)
        m_rows.insert(row);
}

void RangeGridRows::setRows(QSet<int> rows)
{
    if (m_rows != rows)
    {
        m_rows = std::move(rows);
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeGridRows::hasItemImpl(GridID id) const
{
    return m_rows.contains(id.row);
}

SharedPtr<RangeGridRows> makeRangeGridRows(const QSet<int>& rows)
{
    return makeShared<RangeGridRows>(rows);
}

SharedPtr<RangeGridRows> makeRangeGridRows(int rowBegin, int rowEnd)
{
    return makeShared<RangeGridRows>(rowBegin, rowEnd);
}

RangeGridRect::RangeGridRect(const QSet<int>& rows, const QSet<int>& columns)
    : m_rows(rows),
      m_columns(columns)
{
}

RangeGridRect::RangeGridRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd)
{
    Q_ASSERT(rowBegin <= rowEnd);
    Q_ASSERT(columnBegin <= columnEnd);

    for (; rowBegin < rowEnd; ++rowBegin)
        m_rows.insert(rowBegin);

    for (; columnBegin < columnEnd; ++columnBegin)
        m_columns.insert(columnBegin);
}

void RangeGridRect::setRows(QSet<int> rows)
{
    if (m_rows != rows)
    {
        m_rows = std::move(rows);
        emit rangeChanged(this, ChangeReasonRange);
    }
}

void RangeGridRect::setColumns(QSet<int> columns)
{
    if (m_columns != columns)
    {
        m_columns = std::move(columns);
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeGridRect::hasItemImpl(GridID id) const
{
     return m_rows.contains(id.row) && m_columns.contains(id.column);
}

SharedPtr<RangeGridRect> makeRangeGridRect(const QSet<int>& rows, const QSet<int>& columns)
{
    return makeShared<RangeGridRect>(rows, columns);
}

SharedPtr<RangeGridRect> makeRangeGridRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd)
{
    return makeShared<RangeGridRect>(rowBegin, rowEnd, columnBegin, columnEnd);
}

} // end namespace Qi
