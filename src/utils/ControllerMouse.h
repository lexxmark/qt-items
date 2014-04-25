#ifndef QI_CONTROLLER_MOUSE_H
#define QI_CONTROLLER_MOUSE_H

#include <QObject>
#include "CellID.h"

class QMouseEvent;
class QContextMenuEvent;

namespace Qi
{

class QI_EXPORT ControllerMouse: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouse)

public:
    ControllerMouse();
    virtual ~ControllerMouse();

    bool activate();
    bool deactivate();

    bool mouseMoveEvent(QMouseEvent* event);
    bool mousePressEvent(QMouseEvent* event);
    bool mouseReleaseEvent(QMouseEvent* event);
    bool contextMenuEvent(QContextMenuEvent* event);
    
//Q_SIGNALS:
//    void rangeChanged(const Range*);

protected:
    virtual bool activateImpl();
    virtual bool deactivateImpl();

    virtual bool mouseMoveEventImpl(QMouseEvent* event);
    virtual bool mousePressEventImpl(QMouseEvent* event);
    virtual bool mouseReleaseEventImpl(QMouseEvent* event);
    virtual bool contextMenuEventImpl(QContextMenuEvent* event);
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_H
