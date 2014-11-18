#include "Radio.h"
#include <QStyleOptionButton>

namespace Qi
{

int ModelRadio::compareImpl(const ItemID &left, const ItemID &right) const
{
    if (isRadioItem(left))
        return 1;
    else if (isRadioItem(right))
        return -1;
    else
        return 0;
}

bool ModelRadioCallback::isRadioItemImpl(const ItemID& item) const
{
    Q_ASSERT(isRadioItem);
    return isRadioItem(item);
}

bool ModelRadioCallback::setRadioItemImpl(const ItemID& item)
{
    Q_ASSERT(setRadioItem);

    if (setRadioItem(item))
    {
        emit modelChanged(this);
        return true;
    }

    return false;
}

ModelRadioStorage::ModelRadioStorage(const ItemID& radioItem)
    : m_radioItem(radioItem)
{
}

bool ModelRadioStorage::isRadioItemImpl(const ItemID& item) const
{
    return m_radioItem ==item;
}

bool ModelRadioStorage::setRadioItemImpl(const ItemID& item)
{
    if (m_radioItem == item)
        return false;

    m_radioItem = item;
    emit modelChanged(this);

    return true;
}

ViewRadio::ViewRadio(const QSharedPointer<ModelRadio>& model, bool autoController)
    : ViewModeled<ModelRadio>(model),
      m_pushableTracker(this)
{
    if (autoController)
    {
        setController(QSharedPointer<ControllerMouseRadio>::create(model));
    }
}

QSize ViewRadio::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    QStyleOptionButton styleOption;
    styleOption.initFrom(ctx.widget);
    styleOption.state = styleState(item);
    return ctx.widget->style()->sizeFromContents(QStyle::CT_RadioButton, &styleOption, QSize(0, 0), ctx.widget);
}

void ViewRadio::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    QStyleOptionButton styleOption;
    styleOption.initFrom(ctx.widget);
    styleOption.rect = cache.cacheView.rect();
    styleOption.state = styleState(cache.item);
    ctx.widget->style()->drawControl(QStyle::CE_RadioButton, &styleOption, painter, ctx.widget);
}

QStyle::State ViewRadio::styleState(const ItemID& item) const
{
    bool isRadioItem = theModel()->isRadioItem(item);

    QStyle::State state = QStyle::State_Enabled | m_pushableTracker.styleStateByItem(item);
    state |= isRadioItem ? QStyle::State_On : QStyle::State_Off;

    return state;
}

ControllerMouseRadio::ControllerMouseRadio(const QSharedPointer<ModelRadio>& model)
    : m_model(model)
{
    Q_ASSERT(m_model);
}

void ControllerMouseRadio::applyImpl()
{
    m_model->setRadioItem(activeItem());
}

bool ControllerMouseRadio::acceptEditImpl(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent) const
{
    return !m_model->isRadioItem(item) && keyEvent && (keyEvent->type() == QEvent::KeyPress) && (keyEvent->key() == Qt::Key_Space);
}

void ControllerMouseRadio::doEditImpl(const QKeyEvent* keyEvent)
{
    Q_ASSERT(!m_model->isRadioItem(activeItem()));
    Q_ASSERT(keyEvent);
    Q_ASSERT(keyEvent->type() == QEvent::KeyPress);
    Q_ASSERT(keyEvent->key() == Qt::Key_Space);

    m_model->setRadioItem(activeItem());
}

} // end namespace Qi
