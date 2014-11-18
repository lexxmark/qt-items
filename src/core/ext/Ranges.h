#ifndef QI_RANGES_H
#define QI_RANGES_H

#include "core/Range.h"
#include <QSet>
#include <QVector>
#include <QSharedPointer>

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

protected:
    bool hasItemImpl(const ItemID &item) const override { return hasItemCallback(item); }
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
};
QI_EXPORT QSharedPointer<RangeNone> makeRangeNone();

class QI_EXPORT RangeAll: public Range
{
    Q_OBJECT
    
public:
    RangeAll();
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
};
QI_EXPORT QSharedPointer<RangeAll> makeRangeAll();

class QI_EXPORT RangeColumn: public Range
{
    Q_OBJECT
    
public:
    explicit RangeColumn(quint32 column);
    
    quint32 column() const { return m_column; }
    void setColumn(quint32 column);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    
private:
    quint32 m_column;
};
QI_EXPORT QSharedPointer<RangeColumn> makeRangeColumn(quint32 column);

class QI_EXPORT RangeColumns: public Range
{
    Q_OBJECT
    
public:
    explicit RangeColumns(const QSet<quint32>& columns);
    RangeColumns(quint32 columnBegin, quint32 columnEnd);
    
    const QSet<quint32>& columns() const { return m_columns; }
    void setColumns(const QSet<quint32>& columns);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    
private:
    QSet<quint32> m_columns;
};
QI_EXPORT QSharedPointer<RangeColumns> makeRangeColumns(const QSet<quint32>& columns);
QI_EXPORT QSharedPointer<RangeColumns> makeRangeColumns(quint32 columnBegin, quint32 columnEnd);

class QI_EXPORT RangeRow: public Range
{
    Q_OBJECT
    
public:
    explicit RangeRow(quint32 row);
    
    quint32 row() const { return m_row; }
    void setRow(quint32 row);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    
private:
    quint32 m_row;
};
QI_EXPORT QSharedPointer<RangeRow> makeRangeRow(quint32 row);

class QI_EXPORT RangeRows: public Range
{
    Q_OBJECT
    
public:
    explicit RangeRows(const QSet<quint32>& rows);
    RangeRows(quint32 rowBegin, quint32 rowEnd);
    
    const QSet<quint32>& rows() const { return m_rows; }
    void setRows(const QSet<quint32>& rows);
    
protected:
    bool hasItemImpl(const ItemID &item) const override;
    
private:
    QSet<quint32> m_rows;
};
QI_EXPORT QSharedPointer<RangeRows> makeRangeRows(const QSet<quint32>& rows);
QI_EXPORT QSharedPointer<RangeRows> makeRangeRows(quint32 rowBegin, quint32 rowEnd);

class QI_EXPORT RangeRect: public Range
{
    Q_OBJECT

public:
    RangeRect(const QSet<quint32>& rows, const QSet<quint32>& columns);
    RangeRect(quint32 rowBegin, quint32 rowEnd, quint32 columnBegin, quint32 columnEnd);

    const QSet<quint32>& rows() const { return m_rows; }
    void setRows(const QSet<quint32>& rows);

    const QSet<quint32>& columns() const { return m_columns; }
    void setColumns(const QSet<quint32>& columns);

protected:
    bool hasItemImpl(const ItemID &item) const override;

private:
    QSet<quint32> m_rows;
    QSet<quint32> m_columns;
};
QI_EXPORT QSharedPointer<RangeRect> makeRangeRect(const QSet<quint32>& rows, const QSet<quint32>& columns);
QI_EXPORT QSharedPointer<RangeRect> makeRangeRect(quint32 rowBegin, quint32 rowEnd, quint32 columnBegin, quint32 columnEnd);

} // end namespace Qi

#endif // QI_RANGES_H
