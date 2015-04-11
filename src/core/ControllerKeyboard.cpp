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

#include "ControllerKeyboard.h"
#include "utils/auto_value.h"
#include <QDebug>

namespace Qi
{

bool ControllerKeyboardCallback::processKeyPress(QKeyEvent* event)
{
    if (onKeyPress)
        return onKeyPress(event);

    return false;
}

bool ControllerKeyboardCallback::processKeyRelease(QKeyEvent* event)
{
    if (onKeyRelease)
        return onKeyRelease(event);

    return false;
}

ControllerKeyboardChain::ControllerKeyboardChain()
    : m_isInProcess(false)
{
}

void ControllerKeyboardChain::clear()
{
    Q_ASSERT(!m_isInProcess);
    m_controllers.clear();
}

void ControllerKeyboardChain::append(const QSharedPointer<ControllerKeyboard>& controller)
{
    Q_ASSERT(!m_isInProcess);
    m_controllers.append(controller);
}

void ControllerKeyboardChain::prepend(const QSharedPointer<ControllerKeyboard>& controller)
{
    Q_ASSERT(!m_isInProcess);
    m_controllers.prepend(controller);
}

bool ControllerKeyboardChain::processKeyPress(QKeyEvent* event)
{
    auto_value<bool> guard(m_isInProcess, true);

    for (const auto& c: m_controllers)
    {
        if (c->processKeyPress(event))
            return true;
    }

    return false;
}

bool ControllerKeyboardChain::processKeyRelease(QKeyEvent* event)
{
    auto_value<bool> guard(m_isInProcess, true);

    for (const auto& c: m_controllers)
    {
        if (c->processKeyRelease(event))
            return true;
    }

    return false;
}

void ControllerKeyboardChain::startCapturing()
{
    auto_value<bool> guard(m_isInProcess, true);

    for (const auto& c: m_controllers)
        c->startCapturing();
}

void ControllerKeyboardChain::stopCapturing()
{
    auto_value<bool> guard(m_isInProcess, true);

    for (const auto& c: m_controllers)
        c->stopCapturing();
}

} // end namespace Qi
