#include "CacheSpaceItem.h"
#include "core/ControllerMouse.h"
#include "cache/CacheItem.h"
#include "utils/auto_value.h"

namespace Qi
{


CacheSpaceItem::CacheSpaceItem(const QSharedPointer<SpaceItem>& spaceItem, ViewApplicationMask viewApplicationMask)
    : CacheSpace(spaceItem, viewApplicationMask),
      m_spaceItem(spaceItem)
{
}

CacheSpaceItem::~CacheSpaceItem()
{
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

    QPoint origin = originPos();
    QSize size = m_spaceItem->size();
    QRect rect(origin.x(), origin.y(), origin.x() + size.width(), origin.y() + size.height());

    m_item = QSharedPointer<CacheItem>::create();
    m_item->item = m_spaceItem->item();
    m_item->rect = rect;
    m_item->schema = viewSchemaForItem(m_spaceItem->item(), m_viewApplicationMask | m_spaceItem->viewApplicationMask(), m_spaceItem->schemasOrdered());

    // mark item as valid
    m_itemsCacheInvalid = false;
}

void CacheSpaceItem::invalidateItemsCacheStructureImpl() const
{
    if (m_item)
        m_item->invalidateCacheView();
}

void CacheSpaceItem::drawImpl(QPainter* painter, const GuiContext& ctx) const
{
    Q_ASSERT(!m_itemsCacheInvalid);
    m_item->draw(painter, ctx, &m_window);
}

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
