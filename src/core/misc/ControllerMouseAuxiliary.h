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

#ifndef QI_CONTROLLER_MOUSE_AUXILIARY_H
#define QI_CONTROLLER_MOUSE_AUXILIARY_H

#include "QiAPI.h"
#include <QWidget>

namespace Qi
{

class SpaceWidgetCore;
class ControllerMouse;

enum ControllerMousePriority
{
    ControllerMousePriorityBackground = 0, // low priority
    ControllerMousePriorityNormal = 1, // normal priority
    ControllerMousePriorityOverlay = 2 // high priority
};

class QI_EXPORT ControllerContext
{
public:
    QWidget* widget;
    SpaceWidgetCore* widgetCore;
    QPoint point;

    void pushCursor(QCursor cursor, const ControllerMouse* controller) const;
    void popCursor(const ControllerMouse* controller) const;

    virtual void notifyStartCapturing(ControllerMouse& controller) = 0;
    virtual void notifyStopCapturing(ControllerMouse& controller) = 0;

protected:
    // constructor/destructor are accessible from
    // derived classes only
    ControllerContext(QWidget* widget, SpaceWidgetCore* widgetCore);
    virtual ~ControllerContext();

private:
    mutable QCursor m_widgetCursor;
    mutable QVector<QPair<QCursor, const ControllerMouse*>> m_cursorInfos;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_AUXILIARY_H
