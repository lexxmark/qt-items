#ifndef QI_CELLS_SCHEMA_H
#define QI_CELLS_SCHEMA_H

#include "Layout.h"
#include "ControllerMouse.h"
#include "Range.h"

#ifndef QT_NO_DEBUG
#include <QPointer>
#endif

namespace Qi
{

struct QI_EXPORT ViewSchema
{
    Layout* layout;
    View* view;
    ControllerMouse* controller;

    void initOwner(QObject* owner);
    void deleteIfOwnedBy(QObject* owner);

    ViewSchema();
    ~ViewSchema();

#ifndef QT_NO_DEBUG
    QPointer<Layout> layoutTracker;
    QPointer<View> viewTracker;
    QPointer<ControllerMouse> controllerTracker;
#endif
};

struct QI_EXPORT CellsSchema
{
    Range* range;
    ViewSchema viewSchema;

    void initOwner(QObject* owner);
    void deleteIfOwnedBy(QObject* owner);

    CellsSchema();
    ~CellsSchema();

#ifndef QT_NO_DEBUG
    QPointer<Range> rangeTracker;
#endif
};

} // end namespace Qi

#endif // QI_CELLS_SCHEMA_H
