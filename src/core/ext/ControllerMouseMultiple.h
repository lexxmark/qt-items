#ifndef QI_CONTROLLER_MOUSE_MULTIPLE_H
#define QI_CONTROLLER_MOUSE_MULTIPLE_H

#include "core/ControllerMouse.h"

namespace Qi
{

class QI_EXPORT ControllerMouseMultiple: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseMultiple)

public:
    ControllerMouseMultiple() {}
    virtual ~ControllerMouseMultiple() { clear(); }

    void addController(const QSharedPointer<ControllerMouse>& controller);

    const QVector<QSharedPointer<ControllerMouse>>& controllers() const { return m_controllers; }
    bool isEmpty() const { return m_controllers.isEmpty(); }
    void clear();

protected:
    void tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo) override;
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

private:
    QVector<QSharedPointer<ControllerMouse>> m_controllers;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_MULTIPLE_H
