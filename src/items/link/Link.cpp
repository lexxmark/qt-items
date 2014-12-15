#include "Link.h"

namespace Qi
{

ViewLink::ViewLink(const QSharedPointer<ModelText>& model, bool useDefaultController, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewText(model, false, alignment, textElideMode),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        auto controller = QSharedPointer<ControllerMouseLink>::create();
        controller->onApply = [this] (const ItemID& item, const ControllerContext& context) {
            Q_ASSERT(item.isValid());
            if (action)
                action(item, context, this);
        };
        setController(controller);
    }
}

void ViewLink::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    PainterState pState;
    pState.save(painter);

    QColor linkColor = ctx.palette().color(ctx.colorGroup(), QPalette::Link);
    MousePushState state = m_pushableTracker.pushStateByItem(cache.item);
    switch (state)
    {
    case MousePushStateHot:
        linkColor = linkColor.lighter();
        break;

    case MousePushStatePushed:
        linkColor = linkColor.darker();
        break;

    default:;
    }

    painter->setPen(linkColor);

    QRect rect = cache.cacheView.rect();
    QString text = theModel()->value(cache.item);
    Qt::TextElideMode elideMode = textElideMode(cache.item);
    if (elideMode != Qt::ElideNone)
    {
        QString elidedText = painter->fontMetrics().elidedText(text, elideMode, rect.width());
        if (showTooltip)
            *showTooltip = (elidedText != text);

        text = elidedText;
    }
    else
    {
        if (showTooltip)
            *showTooltip = (painter->fontMetrics().width(text) > rect.width());
    }

    painter->drawText(rect, alignment(cache.item), text);

    pState.restore(painter);
}

ControllerMouseLink::ControllerMouseLink(ControllerMousePriority priority, bool processDblClick)
    : ControllerMousePushableCallback(priority, processDblClick)
{
}

void ControllerMouseLink::activateImpl(const ActivationInfo& activationInfo)
{
    ControllerMousePushableCallback::activateImpl(activationInfo);

    m_cursor = activationInfo.context.widget->cursor();
    activationInfo.context.widget->setCursor(Qt::PointingHandCursor);
}

void ControllerMouseLink::deactivateImpl()
{
    activationState().context.widget->setCursor(m_cursor);

    ControllerMousePushableCallback::deactivateImpl();
}


} // end namespace Qi
