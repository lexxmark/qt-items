#ifndef QI_CONTROLLER_MOUSE_CAPTURED_H
#define QI_CONTROLLER_MOUSE_CAPTURED_H

#include "core/ControllerMouse.h"

namespace Qi
{

class QI_EXPORT ControllerMouseCaptured: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseCaptured)

public:
    void processKillFocus(QFocusEvent* event) override;
    bool processLButtonDown(QMouseEvent* event) override;
    bool processLButtonUp(QMouseEvent* event) override;
    bool processLButtonDblClick(QMouseEvent* event) override;

protected:
    ControllerMouseCaptured(ControllerMousePriority priority = ControllerMousePriorityNormal, bool processDblClick = true);

    void startCapturingImpl() override;
    void stopCapturingImpl() override;

    virtual void applyImpl() {}
    virtual bool canApplyImpl() const;

private:
    const bool m_processDblClick;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_CAPTURED_H
