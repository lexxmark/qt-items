#ifndef QI_FILTER_H
#define QI_FILTER_H

#include "core/ItemID.h"
#include <QObject>

namespace Qi
{

class Model;

class QI_EXPORT ItemsFilter: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemsFilter)

public:
    virtual ~ItemsFilter();

    const QSharedPointer<Model>& modelToFilter() const { return m_modelToFilter; }
    bool isItemFiltered(const ItemID& item) const { return isItemFilteredImpl(item); }

signals:
    void filterChanged(const ItemsFilter*);

protected:
    ItemsFilter(const QSharedPointer<Model>& modelToFilter);

    virtual bool isItemFilteredImpl(const ItemID& item) const = 0;

private:
    void onModelToFilterChanged(const Model*);

    QSharedPointer<Model> m_modelToFilter;
};

} // end namespace Qi

#endif // QI_FILTER_H
