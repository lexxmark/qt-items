#ifndef QI_VIEW_H
#define QI_VIEW_H

#include "cache/CacheView.h"
#include "GuiContext.h"
#include <QPainter>
#include <functional>

namespace Qi
{

class Model;
class ControllerMouse;
class Layout;

// struct to hold tooltip information
struct QI_EXPORT TooltipInfo
{
    QString text;
    QRect rect;
};

// constants for intended view application
enum ViewApplication
{
    ViewApplicationNone = 0x0000,
    // view intended to be drawn on the screen
    ViewApplicationDraw = 0x0001,
    // view intended to be drawn for copy
    ViewApplicationCopyDraw = 0x0002,
    // view intended to make text copy
    ViewApplicationCopyText = 0x0004,

    // view intended for user defined operations
    ViewApplicationUser = 0x0100,

    ViewApplicationCopy = ViewApplicationCopyDraw | ViewApplicationCopyText,
    ViewApplicationAll = 0xFFFF,
};

typedef quint16 ViewApplicationMask;


class QI_EXPORT View: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(View)

public:
    View();
    virtual ~View();

    ViewApplicationMask excludeApplicationMask() const { return m_excludeApplicationMask; }
    void setExcludeApplicationMask(ViewApplicationMask excludeApplicationMask) { m_excludeApplicationMask = excludeApplicationMask; }
    bool isApplicable(ViewApplicationMask applicationMask) const { return !(bool)(m_excludeApplicationMask & applicationMask); }

    QSharedPointer<ControllerMouse> controller() const { return m_controller; }
    void setController(QSharedPointer<ControllerMouse> controller);
    void addController(QSharedPointer<ControllerMouse> controller);

    std::function<bool(const ItemID& item, QString& text)> tooltipTextCallback;
    void setTooltipText(const QString& text);

    // adds self to views
    void addView(const ItemID& item, QVector<const View*>& views) const
    { addViewImpl(item, views); }

    // adds self CacheView to cacheViews
    CacheView* addCacheView(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
    { return addCacheViewImpl(layout, ctx, item, cacheViews, itemRect, visibleItemRect); }

    // returns size of the view
    QSize size(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
    { return sizeImpl(ctx, item, sizeMode); }

    // draws view content
    void draw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const;
    // restores painter state after draw
    void cleanupDraw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const
    { cleanupDrawImpl(painter, ctx, cache); }

    // returns text representation of the view
    bool text(const ItemID& item, QString& txt) const { return textImpl(item, txt); }
    // returns tooltip text of the view
    bool tooltipText(const ItemID& item, QString& text) const;
    // returns tooltip under the point
    bool tooltipByPoint(const QPoint& point, const ItemID& item, TooltipInfo& tooltipInfo) const
    { return tooltipByPointImpl(point, item, tooltipInfo); }

    // returns represented model
    Model* model() { return modelImpl(); }

signals:
    void viewChanged(const View*, ChangeReason);

protected:
    // adds View to views
    virtual void addViewImpl(const ItemID& item, QVector<const View*>& views) const;
    // adds CacheView
    virtual CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const;
    // returns size of the view
    virtual QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const { return QSize(0, 0); }
    // draws view content
    virtual void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const { }
    // cleanups drawing attributes
    virtual void cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const { }

    // returns text representation of the view
    virtual bool textImpl(const ItemID& item, QString& txt) const { return false; }
    // returns tooltip text of the view
    virtual bool tooltipTextImpl(const ItemID& item, QString& txt) const { return text(item, txt); }
    // returns tooltip under the point
    virtual bool tooltipByPointImpl(const QPoint& point, const ItemID& item, TooltipInfo& tooltipInfo) const { return false; }

    // returns represented model
    virtual Model* modelImpl() { return nullptr; }

private:
    QSharedPointer<ControllerMouse> m_controller;
    ViewApplicationMask m_excludeApplicationMask;
};

} // end namespace Qi

#endif // QI_VIEW_H
