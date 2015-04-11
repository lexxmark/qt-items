#include "Button.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewButton::ViewButton(const QSharedPointer<View>& bttnContent, ViewDefaultController createDefaultController)
    : ViewComposite(bttnContent, QMargins(1, 1, 1, 1)),
      m_pushableTracker(this)
{
    if (createDefaultController)
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

    // shift sub-view's origin if button has pressed
    if (option.state & QStyle::State_Sunken)
        painter->translate(QPoint(1, 1));

    ViewComposite::drawImpl(painter, ctx, cache, showTooltip);

    // restore sub-view's origin if button has pressed
    if (option.state & QStyle::State_Sunken)
        painter->translate(QPoint(-1, -1));

    painterState.restore(painter);
}

} // end namespace Qi
