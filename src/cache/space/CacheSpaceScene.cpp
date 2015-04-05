#include "CacheSpaceScene.h"
#include "cache/CacheItem.h"
#include "utils/auto_value.h"

namespace Qi
{

CacheSpaceScene::CacheSpaceScene(const QSharedPointer<SpaceScene>& scene, ViewApplicationMask viewApplicationMask)
    : CacheSpace(scene, viewApplicationMask),
      m_scene(scene)
{
}

CacheSpaceScene::~CacheSpaceScene()
{
}

void CacheSpaceScene::clearItemsCacheImpl() const
{
    Q_ASSERT(!m_cacheIsInUse);

    m_items.clear();
    m_scrollDelta = QPoint(0, 0);
    m_sizeDelta = QSize(0, 0);
}

void CacheSpaceScene::validateItemsCacheImpl() const
{
    Q_ASSERT(m_itemsCacheInvalid);

    if (m_scene->count() == 0)
    {
        clearItemsCache();
        m_itemsCacheInvalid = false;
        return;
    }

    auto_value<bool> inUse(m_cacheIsInUse, true);

    QRect cacheRect(scrollOffset(), window().size());
    QPoint origin = originPos();

    int count = m_scene->count();
    auto it = m_items.begin();

    QVector<QSharedPointer<CacheItem>> newItems;

    for (ItemID item(0, 0); item.column < count; ++item.column)
    {
        if (!cacheRect.intersects(m_scene->itemRect(item)))
            continue;

        QSharedPointer<CacheItem> newItem;
        while ((*it)->item.column <= item.column)
        {
            if ((*it)->item.column == item.column)
            {
                (*it)->correctRectangles(m_scrollDelta);
                newItem = *it;
                break;
            }

            ++it;
        }

        if (newItem.isNull())
        {
            newItem = createCacheItem(item);
            // correct rectangle
            newItem->rect.translate(origin);
        }

        newItems.append(newItem);
    }

    m_items.swap(newItems);

    // clear offset
    m_scrollDelta = QPoint(0, 0);
    m_sizeDelta = QSize(0, 0);
    // mark items as valid
    m_itemsCacheInvalid = false;
}

bool CacheSpaceScene::forEachCacheItemImpl(const std::function<bool(const QSharedPointer<CacheItem>&)>& visitor) const
{
    for (const auto& cacheItem : m_items)
    {
        if (!visitor(cacheItem))
            return false;
    }
    return true;
}

const CacheItem* CacheSpaceScene::cacheItemImpl(const ItemID& visibleItem) const
{
    for (const auto& cacheItem : m_items)
    {
        if (cacheItem->item == visibleItem)
            return cacheItem.data();
    }

    return nullptr;
}

const CacheItem* CacheSpaceScene::cacheItemByPositionImpl(const QPoint& point) const
{
    validateItemsCache();

    if (isEmpty())
        return nullptr;

    for (const auto& cacheItem : m_items)
    {
        if (cacheItem->rect.contains(point))
            return cacheItem.data();
    }

    return nullptr;
}

} // end namespace Qi
