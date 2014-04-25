#include "ControllerMouse.h"

namespace Qi
{

ControllerMouse::ControllerMouse()
{
}

ControllerMouse::~ControllerMouse()
{
}

bool ControllerMouse::activate()
{
    return activateImpl();
}

bool ControllerMouse::deactivate()
{
    return deactivateImpl();
}

bool ControllerMouse::mouseMoveEvent(QMouseEvent* event)
{
    return mouseMoveEventImpl(event);
}

bool ControllerMouse::mousePressEvent(QMouseEvent* event)
{
    return mousePressEventImpl(event);
}

bool ControllerMouse::mouseReleaseEvent(QMouseEvent* event)
{
    return mouseReleaseEventImpl(event);
}

bool ControllerMouse::contextMenuEvent(QContextMenuEvent* event)
{
    return contextMenuEventImpl(event);
}

bool ControllerMouse::activateImpl()
{
    return false;
}

bool ControllerMouse::deactivateImpl()
{
    return false;
}

bool ControllerMouse::mouseMoveEventImpl(QMouseEvent* event)
{
    return false;
}

bool ControllerMouse::mousePressEventImpl(QMouseEvent* event)
{
    return false;
}

bool ControllerMouse::mouseReleaseEventImpl(QMouseEvent* event)
{
    return false;
}

bool ControllerMouse::contextMenuEventImpl(QContextMenuEvent* event)
{
    return false;
}

} // end namespace Qi
