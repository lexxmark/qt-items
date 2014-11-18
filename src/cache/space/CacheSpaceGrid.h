#ifndef QI_CACHE_SPACE_GRID_H
#define QI_CACHE_SPACE_GRID_H

#include "CacheSpace.h"
#include "space/SpaceGrid.h"

namespace Qi
{

class QI_EXPORT CacheSpaceGrid: public CacheSpace
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceGrid)

public:
    explicit CacheSpaceGrid(const QSharedPointer<SpaceGrid>& grid, ViewApplicationMask viewApplicationMask = ViewApplicationDraw);
    ~CacheSpaceGrid();

    const QSharedPointer<SpaceGrid>& spaceGrid() const { return m_grid; }
    bool isEmpty() const { return m_items.isEmpty(); }

    bool isItemInFrame(const ItemID& visibleItem) const;
    bool isItemInFrameStrict(const ItemID& visibleItem) const;
    bool isItemAbsInFrame(const ItemID& absItem) const;

    void visibleItemsRange(ItemID& itemStart, ItemID& itemEnd) const;
    ItemID visibleItemByPosition(const QPoint& point) const;

private:
    void clearItemsCacheImpl() const override;
    void validateItemsCacheImpl() const override;
    void invalidateItemsCacheStructureImpl() const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx) const override;
    const CacheItem* cacheItemImpl(const ItemID& visibleItem) const override;
    const CacheItem* cacheItemByPositionImpl(const QPoint& point) const override;

    QSize calculateItemSize(const ItemID& itemVisible, const GuiContext& ctx, ViewSizeMode sizeMode) const;

    QSharedPointer<CacheItem> createCacheItem(const ItemID& itemAbsolute, const QPoint& itemOrigin, const QSize& itemSize) const;

    // source grid space
    QSharedPointer<SpaceGrid> m_grid;

    // visible item ids
    mutable ItemID m_itemStart;
    mutable ItemID m_itemEnd;
    // caches items
    mutable QVector<QSharedPointer<CacheItem>> m_items;

    // hints optimizations
    mutable QScopedPointer<QMap<quint32, ViewSchema>> m_viewsByColumnOptimization;
};

} // end namespace Qi 

#endif // QI_CACHE_SPACE_GRID_H
