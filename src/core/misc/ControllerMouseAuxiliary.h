#ifndef QI_CONTROLLER_MOUSE_AUXILIARY_H
#define QI_CONTROLLER_MOUSE_AUXILIARY_H

#include "QiAPI.h"
#include <QWidget>

namespace Qi
{

class WidgetDriver;
class ControllerMouse;

enum ControllerMousePriority
{
    ControllerMousePriorityNormal = 1, // normal priority
    ControllerMousePriorityBackground = 0, // low priority
    ControllerMousePriorityOverlay = 2 // high priority
};

class QI_EXPORT ControllerContext
{
public:
    QWidget* widget;
    WidgetDriver* widgetDriver;
    QPoint point;

    void pushCursor(QCursor cursor, const ControllerMouse* controller) const;
    void popCursor(const ControllerMouse* controller) const;

    virtual void notifyStartCapturing(ControllerMouse& controller) = 0;
    virtual void notifyStopCapturing(ControllerMouse& controller) = 0;

protected:
    // constructor/destructor are accessible from
    // derived classes only
    ControllerContext(QWidget* widget, WidgetDriver* widgetDriver);
    virtual ~ControllerContext();

private:
    mutable QCursor m_widgetCursor;
    mutable QVector<QPair<QCursor, const ControllerMouse*>> m_cursorInfos;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_AUXILIARY_H
