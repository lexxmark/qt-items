#include "ControllerMousePushable.h"
#include "core/View.h"

namespace Qi
{

ControllerMousePushable::ControllerMousePushable(ControllerMousePriority priority, bool processDblClick)
    : ControllerMouseCaptured(priority, processDblClick)
{
}

void ControllerMousePushable::setPushState(MousePushState pushState)
{
    if (m_pushState != pushState)
    {
        m_pushState = pushState;
        emit pushStateChanged(this);
    }
}

bool ControllerMousePushable::processMouseMove(QMouseEvent* event)
{
    ControllerMouseCaptured::processMouseMove(event);
    updatePushState();
    return false;
}

void ControllerMousePushable::startCapturingImpl()
{
    ControllerMouseCaptured::startCapturingImpl();
    updatePushState();
}

void ControllerMousePushable::stopCapturingImpl()
{
    ControllerMouseCaptured::stopCapturingImpl();
    updatePushState();
}

void ControllerMousePushable::activateImpl(const ActivationInfo& activationInfo)
{
    ControllerMouseCaptured::activateImpl(activationInfo);
    updatePushState();
}

void ControllerMousePushable::deactivateImpl()
{
    ControllerMouseCaptured::deactivateImpl();
    updatePushState();
}

void ControllerMousePushable::updatePushState()
{
    if (state() == ControllerMouseStateInactive)
    {
        setPushState(MousePushStateNone);
        return;
    }

    MousePushState newState = MousePushStateNone;
    if (state() == ControllerMouseStateCapturing)
        newState = canApplyImpl() ? MousePushStatePushed : MousePushStateHot;
    else if (canApplyImpl())
        newState = MousePushStateHot;

    setPushState(newState);
}

PushableTracker::PushableTracker(View* owner)
    : m_owner(owner)
{
    Q_ASSERT(m_owner);
    m_viewConnection = QObject::connect(m_owner.data(), &View::viewChanged, [this](const View* view, ChangeReason changeReason){
        onViewChanged(view, changeReason);
    });
}

PushableTracker::~PushableTracker()
{
    QObject::disconnect(m_controllerConnection);
    QObject::disconnect(m_viewConnection);
}

MousePushState PushableTracker::pushStateByItem(const ItemID& item) const
{
    if (!m_controller)
        return MousePushStateNone;

    return m_controller->pushStateByItem(item);
}

QStyle::State PushableTracker::styleStateByItem(const ItemID& item) const
{
    switch (pushStateByItem(item))
    {
    case MousePushStateHot:
        return QStyle::State_MouseOver;
    case MousePushStatePushed:
        return QStyle::State_Sunken;
    default:
        return QStyle::State_None;
    }
}

void PushableTracker::onViewChanged(const View* view, ChangeReason changeReason)
{
    Q_ASSERT(view == m_owner.data());
    if (changeReason & ChangeReasonViewController)
    {
        QObject::disconnect(m_controllerConnection);
        m_controller = nullptr;

        auto controllerPushable = view->controller().objectCast<ControllerMousePushable>();
        if (controllerPushable)
        {
            m_controller = controllerPushable.data();
            m_controllerConnection = QObject::connect(controllerPushable.data(), &ControllerMousePushable::pushStateChanged, [this](const ControllerMousePushable* controllerPushable) {
                onPushStateChanged(controllerPushable);
            });
        }
    }
}

void PushableTracker::onPushStateChanged(const ControllerMousePushable* controllerPushable)
{
    Q_UNUSED(controllerPushable);
    Q_ASSERT(controllerPushable == m_controller.data());
    emit m_owner->viewChanged(m_owner.data(), ChangeReasonViewContent);
}


} // end namespace Qi
