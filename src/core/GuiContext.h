#ifndef QI_GUI_CONTEXT_H
#define QI_GUI_CONTEXT_H

#include "QiAPI.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT GuiContext
{
public:
    const QWidget *widget;

    GuiContext(const QWidget *widget)
        : widget(widget)
    {
    }
};

} // end namespace Qi

#endif // QI_GUI_CONTEXT_H
