#ifndef QI_CACHE_SPACE_ITEM_H
#define QI_CACHE_SPACE_ITEM_H

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

#include "space/CacheSpace.h"
#include "SpaceItem.h"

namespace Qi
{

class QI_EXPORT CacheSpaceItem : public CacheSpace
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceItem)

public:
    explicit CacheSpaceItem(const SharedPtr<SpaceItem>& spaceItem, bool syncSpaceSizeWithWindow = false);
    ~CacheSpaceItem();

    const SharedPtr<SpaceItem>& spaceItem() const { return m_spaceItem; }

private:
    void clearItemsCacheImpl() const override;
    void validateItemsCacheImpl() const override;
    bool forEachCacheItemImpl(const std::function<bool(const SharedPtr<CacheItem>&)>& visitor) const override;
    const CacheItem* cacheItemImpl(ID visibleId) const override;
    const CacheItem* cacheItemByPositionImpl(QPoint point) const override;

    void onCacheChanged(const CacheSpace* cache, ChangeReason reason);

    SharedPtr<SpaceItem> m_spaceItem;
    mutable SharedPtr<CacheItem> m_item;
};

} // end namespace Qi

#endif // QI_CACHE_SPACE_H
