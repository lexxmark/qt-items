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

#ifndef QI_RANGES_H
#define QI_RANGES_H

#include "core/Range.h"
#include <QSet>
#include <QVector>
#include <QSharedPointer>
#include <functional>

namespace Qi
{

class QI_EXPORT RangeCallback: public Range
{
    Q_OBJECT

public:
    RangeCallback(const std::function<bool(const ItemID&)>& hasItemCallback = nullptr)
        : hasItemCallback(hasItemCallback)
    {
    }

    std::function<bool(const ItemID&)> hasItemCallback;
    std::function<bool(int)> hasRowCallback;
    std::function<bool(int)> hasColumnCallback;

protected:
    bool hasItemImpl(const ItemID &item) const override { return hasItemCallback ? hasItemCallback(item) : Range::hasItemImpl(item); }
    bool hasRowImpl(int row) const override { return hasRowCallback ? hasRowCallback(row) : false; }
    bool hasColumnImpl(int column) const override { return hasColumnCallback ? hasColumnCallback(column) : false; }
};

class QI_EXPORT RangeSelection: public Range
{
    Q_OBJECT

public:
    RangeSelection() {}
    RangeSelection(const RangeSelection& other): Range() { *this = other; }

    RangeSelection& operator=(const RangeSelection& other);

    bool isEmpty() const { return m_ranges.isEmpty(); }
    void clear();
    void addRange(const QSharedPointer<Range>& range, bool exclude);

protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    struct RangeInfo
    {
        QSharedPointer<Range> range;
        bool exclude;
    };

    QVector<RangeInfo> m_ranges;
};

class QI_EXPORT RangeNone: public Range
{
    Q_OBJECT
    
public:
    RangeNone();
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;
};
QI_EXPORT QSharedPointer<RangeNone> makeRangeNone();

class QI_EXPORT RangeAll: public Range
{
    Q_OBJECT
    
public:
    RangeAll();
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;
};
QI_EXPORT QSharedPointer<RangeAll> makeRangeAll();

class QI_EXPORT RangeColumn: public Range
{
    Q_OBJECT
    
public:
    explicit RangeColumn(int column);
    
    int column() const { return m_column; }
    void setColumn(int column);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    int m_column;
};
QI_EXPORT QSharedPointer<RangeColumn> makeRangeColumn(int column);

class QI_EXPORT RangeColumns: public Range
{
    Q_OBJECT
    
public:
    explicit RangeColumns(const QSet<int>& columns);
    RangeColumns(int columnBegin, int columnEnd);
    
    const QSet<int>& columns() const { return m_columns; }
    void setColumns(const QSet<int>& columns);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    QSet<int> m_columns;
};
QI_EXPORT QSharedPointer<RangeColumns> makeRangeColumns(const QSet<int>& columns);
QI_EXPORT QSharedPointer<RangeColumns> makeRangeColumns(int columnBegin, int columnEnd);

class QI_EXPORT RangeRow: public Range
{
    Q_OBJECT
    
public:
    explicit RangeRow(int row);
    
    int row() const { return m_row; }
    void setRow(int row);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    int m_row;
};
QI_EXPORT QSharedPointer<RangeRow> makeRangeRow(int row);

class QI_EXPORT RangeRows: public Range
{
    Q_OBJECT
    
public:
    explicit RangeRows(const QSet<int>& rows);
    RangeRows(int rowBegin, int rowEnd);
    
    const QSet<int>& rows() const { return m_rows; }
    void setRows(const QSet<int>& rows);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    QSet<int> m_rows;
};
QI_EXPORT QSharedPointer<RangeRows> makeRangeRows(const QSet<int>& rows);
QI_EXPORT QSharedPointer<RangeRows> makeRangeRows(int rowBegin, int rowEnd);

class QI_EXPORT RangeRect: public Range
{
    Q_OBJECT

public:
    RangeRect(const QSet<int>& rows, const QSet<int>& columns);
    RangeRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd);

    const QSet<int>& rows() const { return m_rows; }
    void setRows(const QSet<int>& rows);

    const QSet<int>& columns() const { return m_columns; }
    void setColumns(const QSet<int>& columns);

protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    QSet<int> m_rows;
    QSet<int> m_columns;
};
QI_EXPORT QSharedPointer<RangeRect> makeRangeRect(const QSet<int>& rows, const QSet<int>& columns);
QI_EXPORT QSharedPointer<RangeRect> makeRangeRect(int rowBegin, int rowEnd, int columnBegin, int columnEnd);

class QI_EXPORT RangeItem: public Range
{
    Q_OBJECT

public:
    explicit RangeItem(const ItemID& item);

    const ItemID& item() const { return m_item; }
    void setItem(const ItemID& item);

protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    ItemID m_item;
};
QI_EXPORT QSharedPointer<RangeItem> makeRangeItem(const ItemID& item);

} // end namespace Qi

#endif // QI_RANGES_H
