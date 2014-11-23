#ifndef QI_CACHE_ITEM_ITERATOR_H
#define QI_CACHE_ITEM_ITERATOR_H

#include "space/Space.h"
#include "CacheItem.h"
#include <QMap>

namespace Qi
{

class QI_EXPORT CacheItemIterator
{
public:
    CacheItemIterator(const Space& space, ViewApplicationMask viewApplicationMask);

    const Space& space() const { return m_space; }

    CacheItem& moveTo(const ItemID& visibleItem, bool initItemRect = false);

private:
    CacheItemIterator(const CacheItemIterator&);
    CacheItemIterator& operator=(const CacheItemIterator&);

    const Space& m_space;
    ViewApplicationMask m_viewApplicationMask;

    // cache items by columns
    QMap<int, CacheItem> m_cacheItems;
};

} // end namespace Qi

#endif // QI_CACHE_ITEM_ITERATOR_H
