#include "ControllerMousePushableCallback.h"

namespace Qi
{

ControllerMousePushableCallback::ControllerMousePushableCallback(ControllerMousePriority priority, bool processDblClick)
    : ControllerMousePushable(priority, processDblClick)
{
}

void ControllerMousePushableCallback::applyImpl()
{
    emitOnApply();
}

bool ControllerMousePushableCallback::acceptInplaceEditImpl(const ItemID& /*item*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* keyEvent) const
{
    return keyEvent && (keyEvent->type() == QEvent::KeyPress) && (keyEvent->key() == Qt::Key_Space);
}

void ControllerMousePushableCallback::doInplaceEditImpl(const QKeyEvent* keyEvent)
{
    Q_UNUSED(keyEvent);
    Q_ASSERT(keyEvent);
    Q_ASSERT(keyEvent->type() == QEvent::KeyPress);
    Q_ASSERT(keyEvent->key() == Qt::Key_Space);

    emitOnApply();
}

void ControllerMousePushableCallback::emitOnApply()
{
    Q_ASSERT(isActive());
    if (!onApply)
        return;

    onApply(activeItem(), activationState().context);
}

} // end namespace Qi
