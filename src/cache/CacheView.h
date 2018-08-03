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

#include "core/ID.h"
#include <QVector>
#include <QPainter>
#include <functional>

namespace Qi
{

class View;
class GuiContext;

class QI_EXPORT CacheView2
{
public:
    CacheView2();
    CacheView2(const View* view, const QRect& rect);
    CacheView2(const CacheView2& other);
    ~CacheView2();

    CacheView2& operator=(const CacheView2& other);

    const View* view() const { return m_view; }
    const QRect& rect() const { return m_rect; }
    const QVector<CacheView2>& subViews() const { return m_subViews; }

    QVector<CacheView2>& rSubViews() { return m_subViews; }
    QRect& rRect() { return m_rect; }

    std::function<void(const CacheView2*, QPainter*, const GuiContext&, ID, const QRect&, const QRect*)> drawProxy;

    // draws view within m_rect
    void draw(QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect* visibleRect = nullptr) const;
    void drawRaw(QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect* visibleRect = nullptr) const;

    void cleanupDraw(QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect* visibleRect = nullptr) const;

    // retruns tooltip text
    bool tooltipText(ID id, QString& tooltipText) const;

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
    const View* m_view;
    QRect m_rect;
    mutable bool m_showTooltip;

    QVector<CacheView2> m_subViews;
};

class QI_EXPORT CacheContext
{
public:
    const ID& id;
    const QRect& itemRect;
    const CacheView2& cacheView;
    const QRect* visibleRect;

    CacheContext(const ID& id, const QRect& itemRect, const CacheView2& cacheView, const QRect* visibleRect)
        : id(id),
          itemRect(itemRect),
          cacheView(cacheView),
          visibleRect(visibleRect)
    {
    }
};

class QI_EXPORT CacheView: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheView)

public:
    virtual ~CacheView() = default;

    const CacheView* parent() const { return m_parent; }
    const QRect& rect() const { return m_rect; }
    void setRect(QRect rect);

    void visitChildren(const std::function<bool(const CacheView&)>& visitor) const
    { visitChildrenImpl(visitor);}

    void visitChildren(const std::function<bool(CacheView&)>& visitor)
    { visitChildrenImpl(visitor);}

    QSize contentSize(ViewSizeMode sizeMode) const
    { return contentSizeImpl(sizeMode); }

    void draw(QPainter* painter) const
    { drawImpl(painter); }

    bool contentAsText(QString& txt) const
    { return contentAsTextImpl(txt); }

    bool tooltipText(QString& tooltipText) const
    { return tooltipTextImpl(tooltipText); }

    void emitCacheViewChanged(ChangeReason reason);

signals:
    void cacheViewChanged(const CacheView*, ChangeReason);

protected:
    CacheView(const CacheView* parent, QRect rect);

    virtual void visitChildrenImpl(const std::function<bool(const CacheView&)>& /*visitor*/) const {}
    virtual void visitChildrenImpl(const std::function<bool(CacheView&)>& /*visitor*/) {}

    virtual QSize contentSizeImpl(ViewSizeMode /*sizeMode*/) const { return QSize(0, 0); }
    virtual void drawImpl(QPainter* /*painter*/) const {}
    virtual bool contentAsTextImpl(QString& /*txt*/) const { return false; }
    virtual bool tooltipTextImpl(QString& /*tooltipText*/) const { return false; }

private:
    const CacheView* m_parent = nullptr;
    QRect m_rect;
};

} // end namespace Qi

#endif // QI_CACHE_VIEW_H
