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

    void addController(const SharedPtr<ControllerMouse>& controller);

    const QVector<SharedPtr<ControllerMouse>>& controllers() const { return m_controllers; }
    bool isEmpty() const { return m_controllers.isEmpty(); }
    void clear();

protected:
    void tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo) override;
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

private:
    QVector<SharedPtr<ControllerMouse>> m_controllers;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_MULTIPLE_H
