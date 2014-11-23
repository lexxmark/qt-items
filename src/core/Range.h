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
    
signals:
    void rangeChanged(const Range*, ChangeReason);

protected:
    virtual bool hasItemImpl(const ItemID &item) const = 0;
};

} // end namespace Qi

#endif // QI_RANGE_H
