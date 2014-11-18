#ifndef QI_CACHE_ITEM_H
#define QI_CACHE_ITEM_H

#include "CacheView.h"
#include "core/ItemSchema.h"

namespace Qi
{

class ControllerMouse;
class ControllerContext;
class CacheSpace;
struct TooltipInfo;

class QI_EXPORT CacheItem
{
public:
    CacheItem();
    CacheItem(const CacheItem& other);
    CacheItem& operator=(const CacheItem& other);

    ItemID item;
    QRect rect;
    ViewSchema schema;

    const CacheView* cacheView() const { return m_cacheView.data(); }

    bool isCacheViewValid() const { return m_isCacheViewValid; }
    const CacheView* findCacheViewByController(const ControllerMouse* controller) const;

    void invalidateCacheView();
    void correctRectangles(const QPoint& offset);
    void initializeCacheView(const GuiContext& ctx, const QRect* visibleRect = nullptr);

    QSize calculateItemSize(const GuiContext& ctx, ViewSizeMode sizeMode = ViewSizeModeExact) const;
    void draw(QPainter* painter, const GuiContext& ctx, const QRect* visibleRect = nullptr);
    QString text() const;
    void tryActivateControllers(const ControllerContext& context, const CacheSpace& cacheSpace, const QRect* visibleRect, QVector<ControllerMouse*>& controllers) const;
    bool tooltipByPoint(const QPoint& point, TooltipInfo& tooltipInfo) const;

    void addViews(QVector<const View*>& views) const;

private:
    QSharedPointer<CacheView> m_cacheView;
    bool m_isCacheViewValid;
    bool m_isAnyFloatView;
};

} // end namespace Qi

#endif // QI_CACHE_ITEM_H
