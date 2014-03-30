#ifndef QI_RANGESBASIC_H
#define QI_RANGESBASIC_H

#include "Range.h"
#include <QSet>
#include <QSharedPointer>

namespace Qi
{

class QI_EXPORT RangeNone: public Range
{
    Q_OBJECT
    
public:
    RangeNone();
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
};
QI_EXPORT QSharedPointer<RangeNone> makeRangeNone();

class QI_EXPORT RangeAll: public Range
{
    Q_OBJECT
    
public:
    RangeAll();
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
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
    bool hasCellImpl(const CellID &cell) const override;
    
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
    bool hasCellImpl(const CellID &cell) const override;
    
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
    bool hasCellImpl(const CellID &cell) const override;
    
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
    bool hasCellImpl(const CellID &cell) const override;
    
private:
    QSet<quint32> m_rows;
};
QI_EXPORT QSharedPointer<RangeRows> makeRangeRows(const QSet<quint32>& rows);
QI_EXPORT QSharedPointer<RangeRows> makeRangeRows(quint32 rowBegin, quint32 rowEnd);

} // end namespace Qi

#endif // QI_RANGESBASIC_H
