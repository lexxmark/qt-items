#ifndef QI_CACHE_SPACE_H
#define QI_CACHE_SPACE_H

#include "space/Space.h"

namespace Qi
{

class ControllerContext;
class CacheItem;

class QI_EXPORT CacheSpace: public QObject
{
    friend class CacheControllersMouse;

    Q_OBJECT
    Q_DISABLE_COPY(CacheSpace)

public:
    ~CacheSpace();

    const Space& space() const { return *m_space; }

    ViewApplicationMask viewApplicationMask() const { return m_viewApplicationMask; }
    void setViewApplicationMask(ViewApplicationMask viewApplicationMask);

    const QRect& window() const { return m_window; }
    void setWindow(const QRect& window);

    const QPoint& scrollOffset() const { return m_scrollOffset; }
    void setScrollOffset(const QPoint& scrollOffset);

    QPoint originPos() const { return m_window.topLeft() - m_scrollOffset; }

    void set(const QRect& window, const QPoint& scrollOffset);

    QPoint window2Space(const QPoint& windowPoint) const;
    QPoint space2Window(const QPoint& spacePoint) const;

    void clear();
    const CacheItem* cacheItem(const ItemID& visibleItem) const;
    const CacheItem* cacheItemByPosition(const QPoint& point) const;

    void draw(QPainter* painter, const GuiContext& ctx) const;

    void tryActivateControllers(const ControllerContext& context, QVector<ControllerMouse*>& controllers) const;
    bool tooltipByPoint(const QPoint& point, TooltipInfo& tooltipInfo) const;

signals:
    void cacheChanged(const CacheSpace* cache, ChangeReason reason);

private slots:
    void onSpaceChanged(const Space* space, ChangeReason reason);

protected:
    explicit CacheSpace(const QSharedPointer<Space>& space, ViewApplicationMask viewApplicationMask = ViewApplicationDraw);

    void invalidateItemsCache();
    void clearItemsCache() const;
    void validateItemsCache() const;

    virtual void clearItemsCacheImpl() const = 0;
    virtual void validateItemsCacheImpl() const = 0;
    virtual void invalidateItemsCacheStructureImpl() const = 0;
    virtual void drawImpl(QPainter* painter, const GuiContext& ctx) const = 0;
    virtual const CacheItem* cacheItemImpl(const ItemID& visibleItem) const = 0;
    virtual const CacheItem* cacheItemByPositionImpl(const QPoint& point) const = 0;

    // space
    QSharedPointer<Space> m_space;

    // filter for views
    ViewApplicationMask m_viewApplicationMask;

    // visible frame
    QRect m_window;
    // offset within frame
    QPoint m_scrollOffset;

    // offset delta between two ValidateItemsCache calls
    mutable QPoint m_scrollDelta;
    mutable QSize m_sizeDelta;
    // items cache validation flag
    mutable bool m_itemsCacheInvalid;

    // flag for debugging
    mutable bool m_cacheIsInUse;
};

} // end namespace Qi 

#endif // QI_CACHE_SPACE_H
