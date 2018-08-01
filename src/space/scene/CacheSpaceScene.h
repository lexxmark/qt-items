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

#ifndef QI_CACHE_SPACE_SCENE_H
#define QI_CACHE_SPACE_SCENE_H

#include "space/CacheSpace.h"
#include "SpaceScene.h"

namespace Qi
{

class QI_EXPORT CacheSpaceScene: public CacheSpace
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceScene)

public:
    explicit CacheSpaceScene(SharedPtr<SpaceScene> scene);
    ~CacheSpaceScene();

    const SharedPtr<SpaceScene>& spaceScene() const { return m_scene; }
    bool isEmpty() const { return m_scene->count() == 0; }

private:
    void clearItemsCacheImpl() const override;
    void validateItemsCacheImpl() const override;
    bool forEachCacheItemImpl(const std::function<bool(const SharedPtr<CacheItem>&)>& visitor) const override;
    const CacheItem* cacheItemImpl(ID visibleId) const override;
    const CacheItem* cacheItemByPositionImpl(QPoint point) const override;

    // source scene space
    SharedPtr<SpaceScene> m_scene;

    // cache items
    mutable QVector<SharedPtr<CacheItem>> m_items;
};

} // end namespace Qi 

#endif // QI_CACHE_SPACE_SCENE_H
