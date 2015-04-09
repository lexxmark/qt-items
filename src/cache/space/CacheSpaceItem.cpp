#include "CacheSpaceItem.h"
#include "core/ControllerMouse.h"
#include "cache/CacheItem.h"
#include "utils/auto_value.h"

namespace Qi
{


CacheSpaceItem::CacheSpaceItem(const QSharedPointer<SpaceItem>& spaceItem, bool syncSpaceSizeWithWindow, ViewApplicationMask viewApplicationMask)
    : CacheSpace(spaceItem, viewApplicationMask),
      m_spaceItem(spaceItem)
{
    if (syncSpaceSizeWithWindow)
        connect(this, &CacheSpace::cacheChanged, this, &CacheSpaceItem::onCacheChanged);
}

CacheSpaceItem::~CacheSpaceItem()
{
}

void CacheSpaceItem::onCacheChanged(const CacheSpace* cache, ChangeReason reason)
{
    Q_ASSERT(cache == this);
    if (reason & ChangeReasonCacheFrame)
        m_spaceItem->setSize(window().size());
}

void CacheSpaceItem::clearItemsCacheImpl() const
{
    Q_ASSERT(!m_cacheIsInUse);
    m_item.reset();
}

void CacheSpaceItem::validateItemsCacheImpl() const
{
    Q_ASSERT(m_itemsCacheInvalid);
    Q_ASSERT(!m_cacheIsInUse);

    auto_value<bool> inUse(m_cacheIsInUse, true);

    m_item = createCacheItem(m_spaceItem->item());
    m_item->rect.translate(originPos());

    // mark item as valid
    m_itemsCacheInvalid = false;
}

bool CacheSpaceItem::forEachCacheItemImpl(const std::function<bool(const QSharedPointer<CacheItem>&)>& visitor) const
{
    if (m_item)
        return visitor(m_item);
    else
        return true;
}

/*
void CacheSpaceItem::invalidateItemsCacheStructureImpl() const
{
    if (m_item)
        m_item->invalidateCacheView();
}

void CacheSpaceItem::updateItemsCacheSchemaImpl() const
{
    if (!m_item)
        return;

    m_item->invalidateCacheView();
    m_cacheItemsFactory->updateSchema(*m_item);
}

void CacheSpaceItem::drawImpl(QPainter* painter, const GuiContext& ctx) const
{
    Q_ASSERT(!m_itemsCacheInvalid);
    m_item->draw(painter, ctx, &m_window);
}
*/
const CacheItem* CacheSpaceItem::cacheItemImpl(const ItemID& visibleItem) const
{
    if (m_item->item == visibleItem)
        return m_item.data();
    else
        return nullptr;
}

const CacheItem* CacheSpaceItem::cacheItemByPositionImpl(const QPoint& point) const
{
    if (m_item && m_item->rect.contains(point))
        return m_item.data();
    else
        return nullptr;
}

} // end namespace Qi
