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

#include "ControllerMouseAuxiliary.h"

namespace Qi
{

ControllerContext::ControllerContext(QWidget* widget, SpaceWidgetCore* widgetCore)
    : widget(widget),
      widgetCore(widgetCore),
      point(0, 0)
{
}

ControllerContext::~ControllerContext()
{
}

void ControllerContext::pushCursor(QCursor cursor, const ControllerMouse* controller) const
{
    Q_ASSERT(std::find_if(m_cursorInfos.begin(), m_cursorInfos.end(), [controller](const QPair<QCursor, const ControllerMouse*>& elem) {
        return elem.second == controller;
    }) == m_cursorInfos.end());

    if (m_cursorInfos.isEmpty())
        m_widgetCursor = widget->cursor();

    m_cursorInfos.append(qMakePair(cursor, controller));
    widget->setCursor(cursor);
}

void ControllerContext::popCursor(const ControllerMouse* controller) const
{
    for (auto it = m_cursorInfos.begin(); it != m_cursorInfos.end(); ++it)
    {
        if (it->second == controller)
        {
            m_cursorInfos.erase(it);
            break;
        }
    }

    if (m_cursorInfos.isEmpty())
        widget->setCursor(m_widgetCursor);
    else
        widget->setCursor(m_cursorInfos.back().first);
}

} // end namespace Qi
