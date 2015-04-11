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
