/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
    m_owner->emitViewChanged(ChangeReasonViewContent);
}


} // end namespace Qi
