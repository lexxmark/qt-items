#ifndef QI_CACHE_SPACE_SCENE_H
#define QI_CACHE_SPACE_SCENE_H

#include "CacheSpace.h"
#include "space/SpaceScene.h"

namespace Qi
{

class QI_EXPORT CacheSpaceScene: public CacheSpace
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceScene)

public:
    explicit CacheSpaceScene(const QSharedPointer<SpaceScene>& scene, ViewApplicationMask viewApplicationMask = ViewApplicationDraw);
    ~CacheSpaceScene();

    const QSharedPointer<SpaceScene>& spaceScene() const { return m_scene; }
    bool isEmpty() const { return m_scene->count() == 0; }

private:
    void clearItemsCacheImpl() const override;
    void validateItemsCacheImpl() const override;
    bool forEachCacheItemImpl(const std::function<bool(const QSharedPointer<CacheItem>&)>& visitor) const override;
    const CacheItem* cacheItemImpl(const ItemID& visibleItem) const override;
    const CacheItem* cacheItemByPositionImpl(const QPoint& point) const override;

    // source scene space
    QSharedPointer<SpaceScene> m_scene;

    // cache items
    mutable QVector<QSharedPointer<CacheItem>> m_items;
};

} // end namespace Qi 

#endif // QI_CACHE_SPACE_SCENE_H
