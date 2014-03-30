#ifndef QI_RANGE_H
#define QI_RANGE_H

#include <QObject>
#include "CellID.h"

namespace Qi
{

class QI_EXPORT Range: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Range)

public:
    Range();
    virtual ~Range();
    
    bool hasCell(const CellID& cell) const { return hasCellImpl(cell); }
    bool hasCell(quint32 row, quint32 column) const { return hasCell(CellID(row, column)); }
    
Q_SIGNALS:
    void rangeChanged(const Range&);

protected:
    virtual bool hasCellImpl(const CellID &cell) const = 0;
};

} // end namespace Qi

#endif // QI_RANGE_H
