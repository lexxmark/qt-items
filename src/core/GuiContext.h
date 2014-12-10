#ifndef QI_GUI_CONTEXT_H
#define QI_GUI_CONTEXT_H

#include "QiAPI.h"
#include <QWidget>
#include <QStyleOption>

class QStyle;

namespace Qi
{

class QI_EXPORT GuiContext
{
public:
    const QWidget *widget;

    GuiContext(const QWidget *widget)
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

#endif // QI_GUI_CONTEXT_H
