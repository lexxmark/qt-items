#include "Check.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewCheck::ViewCheck(const QSharedPointer<ModelCheck>& model, bool useDefaultController)
    : ViewModeled<ModelCheck>(model),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        setController(createControllerMouseCheck(model));
    }
}

QSize ViewCheck::sizeImpl(const GuiContext& ctx, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    auto style = ctx.style();
    return QSize(style->pixelMetric(QStyle::PM_IndicatorWidth),
                 style->pixelMetric(QStyle::PM_IndicatorHeight));
}

void ViewCheck::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    auto style = ctx.style();

    QStyleOptionButton option;
    ctx.initStyleOption(option);

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.state |= styleState(cache.item);
    option.rect = cache.cacheView.rect();
    // correct rect
    option.rect = style->subElementRect(QStyle::SE_CheckBoxIndicator, &option, ctx.widget);

    // draw check box image
    style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, ctx.widget);
}

QStyle::State ViewCheck::styleState(const ItemID& item) const
{
    Qt::CheckState check = theModel()->value(item);

    QStyle::State state = QStyle::State_Enabled | m_pushableTracker.styleStateByItem(item);

    switch (check)
    {
    case Qt::Checked:
        state |= QStyle::State_On;
        break;
    case Qt::PartiallyChecked:
        state |= QStyle::State_NoChange;
        break;
    case Qt::Unchecked:
        state |= QStyle::State_Off;
        break;
    }

    return state;
}

QSharedPointer<ControllerMousePushable> createControllerMouseCheck(const QSharedPointer<ModelCheck>& model)
{
    auto controller = QSharedPointer<ControllerMousePushableCallback>::create();
    controller->onApply = [model] (const ItemID& item, const ControllerContext& /*context*/) {
        Q_ASSERT(item.isValid());
        Qt::CheckState check = model->value(item);
        model->setValue(item, (check != Qt::Unchecked) ? Qt::Unchecked : Qt::Checked);
    };
    return controller;
}

} // end namespace Qi
