/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "CacheSpaceScene.h"
#include "cache/CacheItem.h"
#include "utils/auto_value.h"

namespace Qi
{

CacheSpaceScene::CacheSpaceScene(SharedPtr<SpaceScene> scene, ViewApplicationMask viewApplicationMask)
    : CacheSpace(scene, viewApplicationMask),
      m_scene(std::move(scene))
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

    QVector<SharedPtr<CacheItem>> newItems;

    for (int id(0); id < count; ++id)
    {
        if (!cacheRect.intersects(m_scene->itemRect(ID(id))))
            continue;

        SharedPtr<CacheItem> newItem;
        while ((it != m_items.end()) && (index((*it)->id) <= id))
        {
            if (index((*it)->id) == id)
            {
                (*it)->correctRectangles(m_scrollDelta);
                newItem = *it;
                break;
            }

            ++it;
        }

        if (newItem.isNull())
        {
            newItem = createCacheItem(ID(id));
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

bool CacheSpaceScene::forEachCacheItemImpl(const std::function<bool(const SharedPtr<CacheItem>&)>& visitor) const
{
    for (const auto& cacheItem : m_items)
    {
        if (!visitor(cacheItem))
            return false;
    }
    return true;
}

const CacheItem* CacheSpaceScene::cacheItemImpl(ID visibleId) const
{
    for (const auto& cacheItem : m_items)
    {
        if (cacheItem->id == visibleId)
            return cacheItem.data();
    }

    return nullptr;
}

const CacheItem* CacheSpaceScene::cacheItemByPositionImpl(QPoint point) const
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
