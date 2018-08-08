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

#ifndef QI_CACHE_SPACE_H
#define QI_CACHE_SPACE_H

#include "Space.h"
#include <core/ext/ViewModeled.h>

namespace Qi
{

class ControllerContext;
class CacheItem;
class CacheItemFactory;
class CacheSpaceAnimationAbstract;

class QI_EXPORT CacheSpace: public QObject
{
    friend class CacheControllersMouse;

    Q_OBJECT
    Q_DISABLE_COPY(CacheSpace)

public:
    ~CacheSpace();

    const Space2& space() const { return *m_space; }
    Space2& rSpace() const { return *m_space; }

    const CacheItemFactory& cacheItemFactory() const { return *m_cacheItemsFactory; }

    const QRect& window() const { return m_window; }
    void setWindow(const QRect& window);

    const QPoint& scrollOffset() const { return m_scrollOffset; }
    void setScrollOffset(const QPoint& scrollOffset);

    QPoint originPos() const { return m_window.topLeft() - m_scrollOffset; }

    void set(const QRect& window, const QPoint& scrollOffset);

    QPoint window2Space(const QPoint& windowPoint) const;
    QPoint space2Window(const QPoint& spacePoint) const;

    void clear();
    const CacheItem* cacheItem(ID visibleId) const;
    const CacheItem* cacheItemByPosition(QPoint point) const;

    std::function<void(const CacheSpace*, QPainter* painter, const GuiContext& ctx)> drawProxy;

    void validate(const GuiContext& ctx) const;
    void draw(QPainter* painter, const GuiContext& ctx) const;
    void drawRaw(QPainter* painter, const GuiContext& ctx) const;

    CacheSpaceAnimationAbstract* animation() const;
    void setAnimation(CacheSpaceAnimationAbstract* animation);

    void tryActivateControllers(const ControllerContext& context, QVector<ControllerMouse*>& controllers) const;
    bool tooltipByPoint(const QPoint& point, TooltipInfo& tooltipInfo) const;

    struct QI_EXPORT IterateInfo
    {
        SharedPtr<CacheItem> cacheItem;
        int cacheItemIndex;
        CacheView2* cacheView;
        int cacheViewIndex;

        IterateInfo(): cacheItemIndex(0), cacheView(nullptr), cacheViewIndex(0) {}
    };

    bool forEachCacheItem(const std::function<bool(const SharedPtr<CacheItem> &)> &visitor) const;
    bool forEachCacheView(const std::function<bool(const IterateInfo&)>& visitor) const;
    //bool forEachCacheView(const std::function<bool(const makeShared<CacheItem>&, CacheView2*)>& visitor);


signals:
    void cacheChanged(const CacheSpace* cache, ChangeReason reason);

protected:
    explicit CacheSpace(SharedPtr<Space2> space);

    void validateItemsCache() const;
    void clearItemsCache() const;
    SharedPtr<CacheItem> createCacheItem(ID visibleId) const;

    virtual void clearItemsCacheImpl() const = 0;
    virtual void validateItemsCacheImpl() const = 0;
    virtual bool forEachCacheItemImpl(const std::function<bool(const SharedPtr<CacheItem>&)>& visitor) const = 0;
    virtual const CacheItem* cacheItemImpl(ID visibleId) const = 0;
    virtual const CacheItem* cacheItemByPositionImpl(QPoint point) const = 0;

    // space
    SharedPtr<Space2> m_space;

    // cache items factory
    SharedPtr<CacheItemFactory> m_cacheItemsFactory;

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

    QPointer<CacheSpaceAnimationAbstract> m_animation;

private:
    void invalidateItemsCache(ChangeReason reason);

    void onSpaceChanged(const Space2* space, ChangeReason reason);
    void updateCacheItemsFactory();
};

class View2Space : public ViewModeled<ModelSpace>
{
public:
    using ViewModeled<ModelSpace>::ViewModeled;

protected:
    // creates new cache view
    SharedPtr<View> createCacheViewImpl(const View* parent, ID id, const GuiContext& ctx) const final;
};

class ViewSpace : public View
{
public:
    ViewSpace(const View* parent, ID id, SharedPtr<Space> space);

protected:
    QSize contentSizeImpl(ViewSizeMode sizeMode) const final;
    void drawImpl(QPainter* painter) const final;
    bool contentAsTextImpl(QString& txt) const final;
    bool tooltipImpl(QPoint point, TooltipInfo& tooltip) const final;

private:
    void UpdateItems();

    SharedPtr<Space> m_space;
    QHash<ID, SharedPtr<View>> m_items;
};

} // end namespace Qi 

#endif // QI_CACHE_SPACE_H
