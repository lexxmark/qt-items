#include "View.h"
#include "Layout.h"
#include "core/ext/ControllerMouseMultiple.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

View::View()
    : m_excludeApplicationMask(ViewApplicationNone)
{
}

View::~View()
{
}

void View::setController(QSharedPointer<ControllerMouse> controller)
{
    if (m_controller == controller)
        return;

    m_controller = controller;
    emit viewChanged(this, ChangeReasonViewController);
}

void View::addController(QSharedPointer<ControllerMouse> controller)
{
    if (!m_controller)
    {
        setController(controller);
    }
    else
    {
        auto controllerMultiple = qSharedPointerObjectCast<ControllerMouseMultiple>(m_controller);
        if (!controllerMultiple)
        {
            controllerMultiple.reset(new ControllerMouseMultiple());
            controllerMultiple->addController(m_controller);
        }

        controllerMultiple->addController(controller);
        setController(controllerMultiple);
    }
}

void View::draw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    Q_ASSERT(cache.cacheView.view() == this);

    drawImpl(painter, ctx, cache, showTooltip);

    if (showTooltip && tooltipTextCallback)
        *showTooltip = true;
}

bool View::tooltipText(const ItemID& item, QString& text) const
{
    if (tooltipTextCallback)
        return tooltipTextCallback(item, text);

    return tooltipTextImpl(item, text);
}

void View::addViewImpl(const ItemID& /*item*/, QVector<const View*>& views) const
{
    views.append(this);
}

CacheView* View::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    QRect viewRect(0, 0, 0, 0);
    if (!layout.doLayout(*this, ctx, item, ViewSizeModeExact, viewRect, itemRect, visibleItemRect))
        return nullptr;

    CacheView cacheView(&layout, this, viewRect);
    cacheViews.append(cacheView);
    return &cacheViews.back();
}

void View::setTooltipText(const QString& text)
{
    tooltipTextCallback = [text] (const ItemID& /*item*/, QString& itemText) {
        itemText = text;
        return true;
    };
}
} // end namespace Qi
