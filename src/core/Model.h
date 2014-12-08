#ifndef QI_MODEL_H
#define QI_MODEL_H

#include "ItemID.h"

namespace Qi
{

class QI_EXPORT Model: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Model)

protected:
    Model();

public:
    virtual ~Model();
    
signals:
    void modelChanged(const Model*);
};

class QI_EXPORT ModelComparable: public Model
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelComparable)

protected:
    ModelComparable();

public:
    virtual ~ModelComparable();

    int compare(const ItemID& left, const ItemID& right) const { return compareImpl(left, right); }
    bool isAscendingDefault(const ItemID& item) const { return isAscendingDefaultImpl(item); }

protected:
    virtual int compareImpl(const ItemID& left, const ItemID& right) const = 0;
    virtual bool isAscendingDefaultImpl(const ItemID& /*item*/) const { return true; }
};

} // end namespace Qi

#endif // QI_MODEL_H
