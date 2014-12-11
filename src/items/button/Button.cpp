#include "Button.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewButton::ViewButton(const QSharedPointer<View>& bttnContent, bool useDefaultController)
    : ViewComposite(bttnContent, QMargins(1, 1, 1, 1)),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        auto controller = QSharedPointer<ControllerMousePushableCallback>::create();
        controller->onApply = [this] (const ItemID& item, const ControllerContext& context) {
            Q_ASSERT(item.isValid());
            if (action)
                action(item, context, this);
        };
        setController(controller);
    }
}

void ViewButton::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    PainterState painterState;
    painterState.save(painter);

    auto style = ctx.style();

    QStyleOptionButton option;
    ctx.initStyleOption(option);

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.state |= m_pushableTracker.styleStateByItem(cache.item);
    option.rect = cache.cacheView.rect();

    if (tuneBttnState)
        tuneBttnState(cache.item, option.state);

    // draw button
    style->drawControl(QStyle::CE_PushButtonBevel, &option, painter, ctx.widget);

    // setup standard palette
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

    painterState.restore(painter);
}

} // end namespace Qi
