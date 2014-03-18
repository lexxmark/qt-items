#ifndef QI_CELL_ID_H
#define QI_CELL_ID_H

#include "QiAPI.h"

//#include <QPair>
#include <QHash>

namespace Qi
{

class QI_EXPORT CellID
{
public:
    quint32 row;
    quint32 column;

    CellID()
        : row(Invalid), column(Invalid)
    {}

    CellID(quint32 row, quint32 column)
        : row(row), column(column)
    {}

    bool isValid() const { return (row != Invalid) && (column != Invalid); }
};

inline bool operator==(CellID left, CellID right)
{
    return (left.row == right.row) && (left.column == right.column);
}

inline bool operator!=(CellID left, CellID right)
{
    return !(left == right);
}

// std::set/std::map support
inline bool operator<(CellID left, CellID right)
{
    if (left.row == right.row)
        return left.column < right.column;
    else
        return left.row < right.row;
}

// QSet/QMap support
inline uint qHash(CellID key)
{
    return qHash(QPair<quint32, quint32>(key.row, key.column));
}

} // end namespace Qi

#endif // QI_CELL_ID_H
