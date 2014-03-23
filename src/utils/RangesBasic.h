#ifndef QI_RANGESBASIC_H
#define QI_RANGESBASIC_H

#include "Range.h"
#include <QSet>

namespace Qi
{

class QI_EXPORT RangeNone: public Range
{
    Q_OBJECT
    
public:
    RangeNone(QObject* parent = nullptr);
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
};
QI_EXPORT RangeNone* makeRangeNone(QObject* parent = nullptr);

class QI_EXPORT RangeAll: public Range
{
    Q_OBJECT
    
public:
    RangeAll(QObject* parent = nullptr);
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
};
QI_EXPORT RangeAll* makeRangeAll(QObject* parent = nullptr);

class QI_EXPORT RangeColumn: public Range
{
    Q_OBJECT
    
public:
    RangeColumn(quint32 column, QObject* parent = nullptr);
    
    quint32 column() const { return m_column; }
    void setColumn(quint32 column);
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
    
private:
    quint32 m_column;
};
QI_EXPORT RangeColumn* makeRangeColumn(quint32 column, QObject* parent = nullptr);

class QI_EXPORT RangeColumns: public Range
{
    Q_OBJECT
    
public:
    RangeColumns(const QSet<quint32>& columns, QObject* parent = nullptr);
    RangeColumns(quint32 columnBegin, quint32 columnEnd, QObject* parent = nullptr);
    
    const QSet<quint32>& columns() const { return m_columns; }
    void setColumns(const QSet<quint32>& columns);
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
    
private:
    QSet<quint32> m_columns;
};
QI_EXPORT RangeColumns* makeRangeColumns(const QSet<quint32>& columns, QObject* parent = nullptr);
QI_EXPORT RangeColumns* makeRangeColumns(quint32 columnBegin, quint32 columnEnd, QObject* parent = nullptr);

class QI_EXPORT RangeRow: public Range
{
    Q_OBJECT
    
public:
    RangeRow(quint32 row, QObject* parent = nullptr);
    
    quint32 row() const { return m_row; }
    void setRow(quint32 row);
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
    
private:
    quint32 m_row;
};
QI_EXPORT RangeRow* makeRangeRow(quint32 row, QObject* parent = nullptr);

class QI_EXPORT RangeRows: public Range
{
    Q_OBJECT
    
public:
    RangeRows(const QSet<quint32>& rows, QObject* parent = nullptr);
    RangeRows(quint32 rowBegin, quint32 rowEnd, QObject* parent = nullptr);
    
    const QSet<quint32>& rows() const { return m_rows; }
    void setRows(const QSet<quint32>& rows);
    
protected:
    bool hasCellImpl(const CellID &cell) const override;
    
private:
    QSet<quint32> m_rows;
};
QI_EXPORT RangeRows* makeRangeRows(const QSet<quint32>& rows, QObject* parent = nullptr);
QI_EXPORT RangeRows* makeRangeRows(quint32 rowBegin, quint32 rowEnd, QObject* parent = nullptr);

} // end namespace Qi

#endif // QI_RANGESBASIC_H
