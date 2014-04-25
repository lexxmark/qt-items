#ifndef QI_CACHE_CONTROLLER_H
#define QI_CACHE_CONTROLLER_H

#include "CellID.h"
#include <QVector>

class QMouseEvent;
class QContextMenuEvent;

namespace Qi
{

class ControllerMouse;

class QI_EXPORT CacheController
{
    Q_DISABLE_COPY(CacheController)

public:
    CacheController();
    ~CacheController();

protected:

private:
    QVector<ControllerMouse*> m_activeControllers;
};

} // end namespace Qi

#endif // QI_CACHE_CONTROLLER_H
