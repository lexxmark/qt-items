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
    Range(QObject* parent = nullptr);
    virtual ~Range();
    
    bool HasCell(const CellID& cell) const { return HasCellImpl(cell); }
    bool HasCell(quint32 row, quint32 column) const { return HasCell(CellID(row, column)); }
    
Q_SIGNALS:
    void rangeChanged(const Range&);

protected:
    virtual bool HasCellImpl(const CellID &cell) const = 0;
};

} // end namespace Qi

#endif // QI_RANGE_H
