#ifndef QI_RANGE_H
#define QI_RANGE_H

#include <QObject>
#include "ItemID.h"

namespace Qi
{

class QI_EXPORT Range: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Range)

public:
    Range();
    virtual ~Range();

    bool hasItem(const ItemID& item) const { return hasItemImpl(item); }
    bool hasItem(int row, int column) const { return hasItem(ItemID(row, column)); }

    bool hasRow(int row) const { return hasRowImpl(row); }
    bool hasColumn(int column) const { return hasColumnImpl(column); }

signals:
    void rangeChanged(const Range*, ChangeReason);

protected:
    // should return true if item is included in the range and false otherwise
    virtual bool hasItemImpl(const ItemID &item) const { return hasRow(item.row) && hasColumn(item.column); }
    // should return true if row intersets with the range and false otherwise
    virtual bool hasRowImpl(int row) const = 0;
    // should return true if column intersets with the range and false otherwise
    virtual bool hasColumnImpl(int column) const = 0;
};

} // end namespace Qi

#endif // QI_RANGE_H
