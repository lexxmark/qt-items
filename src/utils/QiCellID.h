#ifndef QI_CELL_ID_H
#define QI_CELL_ID_H

#include "QiAPI.h"

//#include <QPair>
#include <QHash>

class QI_EXPORT QiCellID
{
public:
    quint32 row;
    quint32 column;

    QiCellID()
        : row(QiInvalid), column(QiInvalid)
    {}

    QiCellID(quint32 row, quint32 column)
        : row(row), column(column)
    {}

    bool isValid() const { return (row != QiInvalid) && (column != QiInvalid); }
};

inline bool operator==(QiCellID left, QiCellID right)
{
    return (left.row == right.row) && (left.column == right.column);
}

inline bool operator!=(QiCellID left, QiCellID right)
{
    return !(left == right);
}

// std::set/std::map support
inline bool operator<(QiCellID left, QiCellID right)
{
    if (left.row == right.row)
        return left.column < right.column;
    else
        return left.row < right.row;
}

// QSet/QMap support
inline uint qHash(QiCellID key)
{
    return qHash(QPair<quint32, quint32>(key.row, key.column));
}

#endif // QI_CELL_ID_H
