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

#include "ControllerMouseMultiple.h"

namespace Qi
{

void ControllerMouseMultiple::addController(const QSharedPointer<ControllerMouse>& controller)
{
    Q_ASSERT(controller);
    m_controllers.append(controller);
}

void ControllerMouseMultiple::clear()
{
    m_controllers.clear();
}

void ControllerMouseMultiple::tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo)
{
    for (const auto& controller: m_controllers)
    {
        controller->tryActivate(activatedControllers, activationInfo.context, activationInfo.cache, activationInfo.cacheSpace);
    }
}

void ControllerMouseMultiple::activateImpl(const ActivationInfo& activationInfo)
{
    // should never be happened
    Q_ASSERT(false);
    ControllerMouse::activateImpl(activationInfo);
}

void ControllerMouseMultiple::deactivateImpl()
{
    // should never be happened
    Q_ASSERT(false);
    ControllerMouse::deactivateImpl();
}

} // end namespace Qi
