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

#ifndef QI_CONTROLLER_MOUSE_PUSHABLE_H
#define QI_CONTROLLER_MOUSE_PUSHABLE_H

#include "ControllerMouseCaptured.h"
#include <QStyle>

namespace Qi
{

class View;

enum MousePushState
{
    MousePushStateNone = 0,
    MousePushStateHot = 1,
    MousePushStatePushed = 2
};

class QI_EXPORT ControllerMousePushable: public ControllerMouseCaptured
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMousePushable)

public:
    MousePushState pushState() const { return m_pushState; }
    MousePushState pushStateByItem(const ItemID& item) const { return (item == activeItem()) ? pushState() : MousePushStateNone; }
    void setPushState(MousePushState pushState);

    bool processMouseMove(QMouseEvent* event) override;

signals:
    void pushStateChanged(const ControllerMousePushable*);

protected:
    ControllerMousePushable(ControllerMousePriority priority = ControllerMousePriorityNormal, bool processDblClick = true);

    void startCapturingImpl() override;
    void stopCapturingImpl() override;
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

private:
    void updatePushState();

    MousePushState m_pushState;
};

class QI_EXPORT PushableTracker
{
    Q_DISABLE_COPY(PushableTracker)

public:
    PushableTracker(View* owner);
    ~PushableTracker();

    MousePushState pushStateByItem(const ItemID& item) const;
    QStyle::State styleStateByItem(const ItemID& item) const;

private:
    void onViewChanged(const View* view, ChangeReason changeReason);
    void onPushStateChanged(const ControllerMousePushable*controllerPushable);

    QPointer<View> m_owner;
    QMetaObject::Connection m_viewConnection;

    QPointer<ControllerMousePushable> m_controller;
    QMetaObject::Connection m_controllerConnection;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_PUSHABLE_H
