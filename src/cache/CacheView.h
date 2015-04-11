#ifndef QI_CACHE_VIEW_H
#define QI_CACHE_VIEW_H

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

#include "core/ItemID.h"
#include <QVector>
#include <QPainter>
#include <functional>

namespace Qi
{

class Layout;
class View;
class GuiContext;

class QI_EXPORT CacheView
{
public:
    CacheView();
    CacheView(const Layout* layout, const View* view, const QRect& rect);
    CacheView(const CacheView& other);
    ~CacheView();

    CacheView& operator=(const CacheView& other);

    const Layout* layout() const { return m_layout; }
    const View* view() const { return m_view; }
    const QRect& rect() const { return m_rect; }
    const QVector<CacheView>& subViews() const { return m_subViews; }

    QVector<CacheView>& rSubViews() { return m_subViews; }
    QRect& rRect() { return m_rect; }

    std::function<void(const CacheView*, QPainter*, const GuiContext&, const ItemID&, const QRect&, const QRect*)> drawProxy;

    // draws view within m_rect
    void draw(QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect* visibleRect = nullptr) const;
    void drawRaw(QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect* visibleRect = nullptr) const;

    void cleanupDraw(QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect* visibleRect = nullptr) const;

    // retruns tooltip text
    bool tooltipText(const ItemID& item, QString& tooltipText) const;

    template <typename Pred>
    bool forEachCacheView(Pred pred)
    {
        if (!pred(this))
            return false;

        for (auto& cacheSubView: m_subViews)
        {
            if (!cacheSubView.forEachCacheView(pred))
                return false;
        }

        return true;
    }

    template <typename Pred>
    bool forEachCacheView(Pred pred) const
    {
        if (!pred(this))
            return false;

        for (const auto& cacheSubView: m_subViews)
        {
            if (!cacheSubView.forEachCacheView(pred))
                return false;
        }

        return true;
    }

protected:
    const Layout* m_layout;
    const View* m_view;
    QRect m_rect;
    mutable bool m_showTooltip;

    QVector<CacheView> m_subViews;
};

class QI_EXPORT CacheContext
{
public:
    const ItemID& item;
    const QRect& itemRect;
    const CacheView& cacheView;
    const QRect* visibleRect;

    CacheContext(const ItemID& item, const QRect& itemRect, const CacheView& cacheView, const QRect* visibleRect)
        : item(item),
          itemRect(itemRect),
          cacheView(cacheView),
          visibleRect(visibleRect)
    {
    }
};

} // end namespace Qi

#endif // QI_CACHE_VIEW_H
