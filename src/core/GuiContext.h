#ifndef QI_GUI_CONTEXT_H
#define QI_GUI_CONTEXT_H

#include "QiAPI.h"
#include <QWidget>

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
};

} // end namespace Qi

#endif // QI_GUI_CONTEXT_H
