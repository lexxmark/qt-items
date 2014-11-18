#include "ControllerMouseCaptured.h"
#include <QWidget>

namespace Qi
{

ControllerMouseCaptured::ControllerMouseCaptured(ControllerMousePriority priority, bool processDblClick)
    : ControllerMouse(priority),
      m_processDblClick(processDblClick)
{
}

void ControllerMouseCaptured::startCapturingImpl()
{
    ControllerMouse::startCapturingImpl();
    activationState().context.widget->grabMouse();
}

void ControllerMouseCaptured::stopCapturingImpl()
{
    activationState().context.widget->releaseMouse();
    ControllerMouse::stopCapturingImpl();
}

void ControllerMouseCaptured::processKillFocus(QFocusEvent* event)
{
    if (isCapturing())
        stopCapturing();
}

bool ControllerMouseCaptured::processLButtonDown(QMouseEvent* event)
{
    Q_ASSERT(!isCapturing());
    if (!isCapturing())
        startCapturing();

    return true;
}

bool ControllerMouseCaptured::processLButtonUp(QMouseEvent* event)
{
    if (isCapturing())
    {
        bool canApply = canApplyImpl();
        stopCapturing();
        if (canApply)
            applyImpl();

        return true;
    }

    return false;
}

bool ControllerMouseCaptured::processLButtonDblClick(QMouseEvent* event)
{
    if (!m_processDblClick)
        return false;

    Q_ASSERT(!isCapturing());

    if (!isCapturing())
        startCapturing();

    return true;
}

bool ControllerMouseCaptured::canApplyImpl() const
{
    // can apply click if mouse was released under view's rect
    return activationState().viewRect.contains(activationState().point());
}


} // end namespace Qi
