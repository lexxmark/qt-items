#include "Check.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewCheck::ViewCheck(const QSharedPointer<ModelCheck>& model, bool autoController)
    : ViewModeled<ModelCheck>(model),
      m_pushableTracker(this)
{
    if (autoController)
    {
        setController(QSharedPointer<ControllerMouseCheck>::create(model));
    }
}

QSize ViewCheck::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    QStyleOptionButton styleOption;
    styleOption.initFrom(ctx.widget);
    styleOption.state = styleState(item);
    return ctx.widget->style()->sizeFromContents(QStyle::CT_CheckBox, &styleOption, QSize(0, 0), ctx.widget);
}

void ViewCheck::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    QStyleOptionButton styleOption;
    styleOption.initFrom(ctx.widget);
    styleOption.rect = cache.cacheView.rect();
    styleOption.state = styleState(cache.item);
    ctx.widget->style()->drawControl(QStyle::CE_CheckBox, &styleOption, painter, ctx.widget);
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

bool ControllerMouseCheck::acceptEditImpl(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent) const
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
