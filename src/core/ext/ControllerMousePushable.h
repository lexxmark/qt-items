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
