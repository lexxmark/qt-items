#ifndef QI_CACHE_ITEM_FACTORY_H
#define QI_CACHE_ITEM_FACTORY_H

#include "space/Space.h"
#include "CacheItem.h"

namespace Qi
{

class QI_EXPORT CacheItemFactory
{
    Q_DISABLE_COPY(CacheItemFactory)

public:
    CacheItemFactory(const Space& space, ViewApplicationMask viewApplicationMask);

    CacheItemInfo create(const ItemID& visibleItem) const;
    void updateSchema(CacheItemInfo& info) const;

    const Space& space() const { return m_space; }

protected:
    virtual void initSchemaImpl(CacheItemInfo& info) const;

    ViewSchema createViewSchema(const ItemID& absItem) const;

private:
    const Space& m_space;
    ViewApplicationMask m_viewApplicationMask;
};

QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactoryDefault(const Space& space, ViewApplicationMask viewApplicationMask);
QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactoryItem(const Space& space, ViewApplicationMask viewApplicationMask);
QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactorySameSchemaByColumn(const Space& space, ViewApplicationMask viewApplicationMask);
QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactorySameSchemaByRow(const Space& space, ViewApplicationMask viewApplicationMask);

} // end namespace Qi

#endif // QI_CACHE_ITEM_FACTORY_H
