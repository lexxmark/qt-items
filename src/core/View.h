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

#ifndef QI_VIEW_H
#define QI_VIEW_H

#include "core/misc/ViewAuxiliary.h"
#include "cache/CacheView.h"
#include <QPainter>
#include <functional>

namespace Qi
{

class Model;
class ControllerMouse;
class Layout;

class QI_EXPORT View: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(View)

public:
    View();
    virtual ~View();

    SharedPtr<ControllerMouse> controller() const { return m_controller; }
    void setController(SharedPtr<ControllerMouse> controller);
    void addController(SharedPtr<ControllerMouse> controller);

    std::function<bool(ID id, QString& text)> tooltipTextCallback;
    void setTooltipText(const QString& text);

    // adds self to views
    void addView(ID id, QVector<const View*>& views) const
    { addViewImpl(id, views); }

    // adds self CacheView2 to cacheViews
    CacheView2* addCacheView(const Layout& layout, const GuiContext& ctx, ID id, QVector<CacheView2>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
    { return addCacheViewImpl(layout, ctx, id, cacheViews, itemRect, visibleItemRect); }

    // returns size of the view
    QSize size(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const
    { return sizeImpl(ctx, id, sizeMode); }

    // draws view content
    void draw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const;
    // restores painter state after draw
    void cleanupDraw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const
    { cleanupDrawImpl(painter, ctx, cache); }

    // returns text representation of the view
    bool text(ID id, QString& txt) const { return textImpl(id, txt); }
    // returns tooltip text of the view
    bool tooltipText(ID id, QString& text) const;
    // returns tooltip under the point
    bool tooltipByPoint(QPoint point, ID id, TooltipInfo& tooltipInfo) const
    { return tooltipByPointImpl(point, id, tooltipInfo); }

    // returns represented model
    Model* model() { return modelImpl(); }

    SharedPtr<CacheView> createCacheView(const CacheView* parent, QRect rect, ID id, const GuiContext& ctx) const
    { return createCacheViewImpl(parent, rect, id, ctx); }

    // emits viewChanged signal
    void emitViewChanged(ChangeReason reason);

signals:
    void viewChanged(const View*, ChangeReason);

protected:
    // adds View to views
    virtual void addViewImpl(ID id, QVector<const View*>& views) const;
    // adds CacheView2
    virtual CacheView2* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, ID id, QVector<CacheView2>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const;
    // returns size of the view
    virtual QSize sizeImpl(const GuiContext& /*ctx*/, ID /*id*/, ViewSizeMode /*sizeMode*/) const;
    // draws view content
    virtual void drawImpl(QPainter* /*painter*/, const GuiContext& /*ctx*/, const CacheContext& /*cache*/, bool* /*showTooltip*/) const { }
    // cleanups drawing attributes
    virtual void cleanupDrawImpl(QPainter* /*painter*/, const GuiContext& /*ctx*/, const CacheContext& /*cache*/) const { }

    // returns text representation of the view
    virtual bool textImpl(ID /*id*/, QString& /*txt*/) const { return false; }
    // returns tooltip text of the view
    virtual bool tooltipTextImpl(ID id, QString& txt) const { return text(id, txt); }
    // returns tooltip under the point
    virtual bool tooltipByPointImpl(QPoint /*point*/, ID /*id*/, TooltipInfo& /*tooltipInfo*/) const { return false; }

    // returns represented model
    virtual Model* modelImpl() { return nullptr; }

    // creates new cache view
    virtual SharedPtr<CacheView> createCacheViewImpl(const CacheView* parent, QRect rect, ID id, const GuiContext& ctx) const;

    class PainterState
    {
    public:
        QPen pen;
        QBrush background;
        QBrush brush;

        void save(QPainter* painter)
        {
            pen = painter->pen();
            background = painter->background();
            brush = painter->brush();
        }

        void restore(QPainter* painter)
        {
            painter->setPen(pen);
            painter->setBackground(background);
            painter->setBrush(brush);
        }
    };

private:
    SharedPtr<ControllerMouse> m_controller;
};

class QI_EXPORT CacheViewProxy : public CacheView
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheViewProxy)

public:
    CacheViewProxy(const CacheView* parent, QRect rect, const View* view, ID id, const GuiContext& ctx);

protected:
    QSize contentSizeImpl(ViewSizeMode sizeMode) const final;
    void drawImpl(QPainter* painter) const final;
    bool contentAsTextImpl(QString& txt) const final;
    bool tooltipTextImpl(QString& tooltipText) const final;

private:
    const View* m_view;
    ID m_id;
    const GuiContext& m_ctx;
};

} // end namespace Qi

#endif // QI_VIEW_H
