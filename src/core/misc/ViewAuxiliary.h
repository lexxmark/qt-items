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

#ifndef QI_VIEW_AUXILIARY_H
#define QI_VIEW_AUXILIARY_H

#include "QiAPI.h"
#include <QWidget>
#include <QStyleOption>

namespace Qi
{

// struct to hold tooltip information
struct QI_EXPORT TooltipInfo
{
    QString text;
    QRect rect;
};

// constants for intended view application
enum ViewApplicationFlag
{
    ViewApplicationNone = 0x0000,
    // view intended to be drawn on the widget
    ViewApplicationDraw = 0x0001,
    // view intended to be drawn for copy
    ViewApplicationCopyDraw = 0x0002,
    // view intended to make text copy
    ViewApplicationCopyText = 0x0004,

    // view intended for user defined operations
    ViewApplicationUser = 0x0100,

    ViewApplicationCopy = ViewApplicationCopyDraw | ViewApplicationCopyText,
    ViewApplicationAll = 0xFFFF,
};

Q_DECLARE_FLAGS(ViewApplicationMask, ViewApplicationFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ViewApplicationMask)

enum ViewDefaultController
{
    ViewDefaultControllerNone = 0x0,
    ViewDefaultControllerCreate = 0x1
};

class QI_EXPORT GuiContext
{
public:
    const QWidget* widget;

    GuiContext(const QWidget* widget)
        : widget(widget)
    {
        Q_ASSERT(widget);
    }

    QStyle* style() const { return widget->style(); }
    void initStyleOption(QStyleOption& option) const
    {
        option.initFrom(widget);
        // State_MouseOver should be set explicitly
        option.state &= ~QStyle::State_MouseOver;
    }

    const QPalette& palette() const { return widget->palette(); }
    QPalette::ColorGroup colorGroup() const
    {
        if (!widget->isEnabled())
            return QPalette::Disabled;
        else if (!widget->hasFocus())
            return QPalette::Inactive;
        else
            return QPalette::Active;
    }
};

} // end namespace Qi

#endif // QI_VIEW_AUXILIARY_H
