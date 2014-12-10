#include "Button.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewButton::ViewButton(const QSharedPointer<View>& bttnContent, bool useDefaultController, quintptr id)
    : ViewComposite(bttnContent, QMargins(1, 1, 1, 1)),
      m_id(id),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        auto controller = QSharedPointer<ControllerMousePushableCallback>::create();
        controller->onApply = [this] (const ItemID& item, const ControllerContext& context) {
            Q_ASSERT(item.isValid());
            if (bttnAction)
                bttnAction(item, context, this);
        };
        setController(controller);
    }
}

void ViewButton::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    m_painterState.save(painter);

    auto style = ctx.style();

    QStyleOptionButton option;
    ctx.initStyleOption(option);

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.state |= m_pushableTracker.styleStateByItem(cache.item);
    option.rect = cache.cacheView.rect();
    // draw button
    style->drawControl(QStyle::CE_PushButtonBevel, &option, painter, ctx.widget);

    auto cg = ctx.colorGroup();
    painter->setPen(ctx.widget->palette().color(cg, QPalette::ButtonText));
    painter->setBackground(ctx.widget->palette().brush(cg, QPalette::Button));

    // setup sub-view rect
    // dirty code => should be rewritten
    Q_ASSERT(cache.cacheView.subViews().size() == 1);
    if (cache.cacheView.subViews().size() == 1)
    {
        auto& subView = const_cast<CacheView&>(cache.cacheView.subViews().back());
        subView.rRect() = option.rect.marginsRemoved(margins());
        if (option.state & QStyle::State_Sunken)
        {
            subView.rRect().adjust(1, 1, 0, 0);
        }
    }

    ViewComposite::drawImpl(painter, ctx, cache, showTooltip);
}

void ViewButton::cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const
{
    ViewComposite::cleanupDrawImpl(painter, ctx, cache);
    m_painterState.restore(painter);
}

} // end namespace Qi
