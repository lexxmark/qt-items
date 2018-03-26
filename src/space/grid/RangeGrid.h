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

#ifndef QI_RANGE_GRID_H
#define QI_RANGE_GRID_H

#include "core/Range.h"
#include "GridID.h"
#include "Lines.h"

namespace Qi
{

class QI_EXPORT RangeGrid: public Range
{
    Q_OBJECT

public:
    bool hasItem(GridID id) const { return hasItemImpl(id); }
    bool hasItem(int row, int column) const { return hasItem(GridID(row, column)); }

protected:
    RangeGrid() = default;

    bool hasItemImpl(ID id) const final { return hasItem(id.as<GridID>()); }
    virtual bool hasItemImpl(GridID id) const = 0;
};

class QI_EXPORT RangeGridCallback: public RangeGrid
{
    Q_OBJECT

public:
    RangeGridCallback(std::function<bool(GridID)> hasItemCallback = nullptr)
        : hasItemCallback(std::move(hasItemCallback))
    {
    }

    std::function<bool(GridID)> hasItemCallback;

protected:
    bool hasItemImpl(GridID id) const final { return hasItemCallback ? hasItemCallback(id) : false; }
};

class QI_EXPORT RangeGridColumn: public RangeGrid
{
    Q_OBJECT

public:
    explicit RangeGridColumn(int column);

    int column() const { return m_column; }
    void setColumn(int column);

protected:
    bool hasItemImpl(GridID id) const override;

private:
    int m_column;
};
QI_EXPORT QSharedPointer<RangeGridColumn> makeRangeGridColumn(int column);

class QI_EXPORT RangeGridColumns: public RangeGrid
{
    Q_OBJECT

public:
    explicit RangeGridColumns(QSet<int> columns);
    RangeGridColumns(int columnBegin, int columnEnd);

    const QSet<int>& columns() const { return m_columns; }
    void setColumns(QSet<int> columns);

protected:
    bool hasItemImpl(GridID id) const override;

private:
    QSet<int> m_columns;
};
QI_EXPORT QSharedPointer<RangeGridColumns> makeRangeGridColumns(const QSet<int>& columns);
QI_EXPORT QSharedPointer<RangeGridColumns> makeRangeGridColumns(int columnBegin, int columnEnd);

class QI_EXPORT RangeGridRow: public RangeGrid
{
    Q_OBJECT

public:
    explicit RangeGridRow(int row);

    int row() const { return m_row; }
    void setRow(int row);

protected:
    bool hasItemImpl(GridID id) const override;

private:
    int m_row;
};
QI_EXPORT QSharedPointer<RangeGridRow> makeRangeGridRow(int row);

class QI_EXPORT RangeGridRows: public RangeGrid
{
    Q_OBJECT

public:
    explicit RangeGridRows(QSet<int> rows);
    RangeGridRows(int rowBegin, int rowEnd);

    const QSet<int>& rows() const { return m_rows; }
    void setRows(QSet<int> rows);

protected:
    bool hasItemImpl(GridID id) const override;
private:
    QSet<int> m_rows;
};
QI_EXPORT QSharedPointer<RangeGridRows> makeRangeGridRows(const QSet<int>& rows);
QI_EXPORT QSharedPointer<RangeGridRows> makeRangeGridRows(int rowBegin, int rowEnd);

class QI_EXPORT RangeGridRect: public RangeGrid
{
    Q_OBJECT

public:
    RangeGridRect(const QSet<int>& rows, const QSet<int>& columns);
    RangeGridRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd);

    const QSet<int>& rows() const { return m_rows; }
    void setRows(QSet<int> rows);

    const QSet<int>& columns() const { return m_columns; }
    void setColumns(QSet<int> columns);

protected:
    bool hasItemImpl(GridID id) const override;

private:
    QSet<int> m_rows;
    QSet<int> m_columns;
};
QI_EXPORT QSharedPointer<RangeGridRect> makeRangeGridRect(const QSet<int>& rows, const QSet<int>& columns);
QI_EXPORT QSharedPointer<RangeGridRect> makeRangeGridRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd);

} // end namespace Qi 

#endif // QI_RANGE_GRID_H
