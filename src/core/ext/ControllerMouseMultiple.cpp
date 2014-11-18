#include "ControllerMouseMultiple.h"

namespace Qi
{

void ControllerMouseMultiple::addController(const QSharedPointer<ControllerMouse>& controller)
{
    Q_ASSERT(controller);
    m_controllers.append(controller);
}

void ControllerMouseMultiple::clear()
{
    m_controllers.clear();
}

void ControllerMouseMultiple::tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo)
{
    for (const auto& controller: m_controllers)
    {
        controller->tryActivate(activatedControllers, activationInfo.context, activationInfo.cache, activationInfo.cacheSpace);
    }
}

void ControllerMouseMultiple::activateImpl(const ActivationInfo& activationInfo)
{
    // should never be happened
    Q_ASSERT(false);
    ControllerMouse::activateImpl(activationInfo);
}

void ControllerMouseMultiple::deactivateImpl()
{
    // should never be happened
    Q_ASSERT(false);
    ControllerMouse::deactivateImpl();
}

} // end namespace Qi
