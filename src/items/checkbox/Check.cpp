#include "Check.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewCheck::ViewCheck(const QSharedPointer<ModelCheck>& model, bool useDefaultController)
    : ViewModeled<ModelCheck>(model),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        setController(QSharedPointer<ControllerMouseCheck>::create(model));
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

    QStyleOptionButton styleOption;

    // dont initialize style from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (!style->inherits("QWindowsVistaStyle"))
        styleOption.initFrom(ctx.widget);

    styleOption.state = styleState(cache.item);
    styleOption.rect = cache.cacheView.rect();
    // correct rect
    styleOption.rect = style->subElementRect(QStyle::SE_RadioButtonIndicator, &styleOption, ctx.widget);

    // draw check box image
    style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &styleOption, painter, ctx.widget);
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

ControllerMouseCheck::ControllerMouseCheck(const QSharedPointer<ModelCheck>& model)
    : m_model(model)
{
    Q_ASSERT(m_model);
}

void ControllerMouseCheck::applyImpl()
{
    toggleCheck();
}

bool ControllerMouseCheck::acceptEditImpl(const ItemID& /*item*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* keyEvent) const
{
    return keyEvent && (keyEvent->type() == QEvent::KeyPress) && (keyEvent->key() == Qt::Key_Space);
}

void ControllerMouseCheck::doEditImpl(const QKeyEvent* keyEvent)
{
    Q_ASSERT(keyEvent);
    Q_ASSERT(keyEvent->type() == QEvent::KeyPress);
    Q_ASSERT(keyEvent->key() == Qt::Key_Space);

    toggleCheck();
}

void ControllerMouseCheck::toggleCheck()
{
    ItemID item = activeItem();
    Q_ASSERT(item.isValid());
    Qt::CheckState check = m_model->value(item);
    m_model->setValue(item, (check != Qt::Unchecked) ? Qt::Unchecked : Qt::Checked);
}

} // end namespace Qi
