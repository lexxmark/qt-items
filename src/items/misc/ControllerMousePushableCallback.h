#ifndef QI_CONTROLLER_MOUSE_PUSHABLE_CALLBACK_H
#define QI_CONTROLLER_MOUSE_PUSHABLE_CALLBACK_H

#include "core/ext/ControllerMousePushable.h"
#include <functional>

namespace Qi
{

// emits onApply when user clicks or presses Space key
class QI_EXPORT ControllerMousePushableCallback: public ControllerMousePushable
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMousePushableCallback)

public:
    ControllerMousePushableCallback(ControllerMousePriority priority = ControllerMousePriorityNormal, bool processDblClick = true);

    std::function<void (const ItemID& item, const ControllerContext& context)> onApply;

protected:
    void applyImpl() override;
    bool acceptInplaceEditImpl(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent) const override;
    void doInplaceEditImpl(const QKeyEvent* keyEvent) override;

private:
    void emitOnApply();
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_PUSHABLE_CALLBACK_H
