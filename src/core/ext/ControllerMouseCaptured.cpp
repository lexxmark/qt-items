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

void ControllerMouseCaptured::processKillFocus(QFocusEvent* /*event*/)
{
    if (isCapturing())
        stopCapturing();
}

bool ControllerMouseCaptured::processLButtonDown(QMouseEvent* /*event*/)
{
    Q_ASSERT(!isCapturing());
    if (!isCapturing())
        startCapturing();

    return true;
}

bool ControllerMouseCaptured::processLButtonUp(QMouseEvent* /*event*/)
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

bool ControllerMouseCaptured::processLButtonDblClick(QMouseEvent* /*event*/)
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
