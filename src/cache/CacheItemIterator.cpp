#include "CacheItemIterator.h"

namespace Qi
{

CacheItemIterator::CacheItemIterator(const Space& space, ViewApplicationMask viewApplicationMask)
    : m_space(space), m_viewApplicationMask(viewApplicationMask)
{
}

CacheItem& CacheItemIterator::moveTo(const ItemID& visibleItem, bool initItemRect)
{
    CacheItem& cacheItem = m_cacheItems[visibleItem.column];
    cacheItem.item = m_space.toAbsolute(visibleItem);

    // mark view cache as invalid
    cacheItem.invalidateCacheView();

    // update view schema
    if (!(m_space.hints() & SpaceHintSimilarViewsByColumns) || !cacheItem.schema.isValid())
    {
        cacheItem.schema = viewSchemaForItem(cacheItem.item, m_viewApplicationMask | m_space.viewApplicationMask(), m_space.schemasOrdered());
    }

    // initialize item rect
    if (initItemRect)
    {
        cacheItem.rect = m_space.itemRect(visibleItem);
    }

    return cacheItem;
}

} // end namespace Qi
