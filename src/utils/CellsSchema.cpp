#include "CellsSchema.h"

namespace Qi
{

ViewSchema::ViewSchema()
    : layout(nullptr),
      view(nullptr),
      controller(nullptr)
{
}

ViewSchema::~ViewSchema()
{
#ifndef QT_NO_DEBUG
    if (layout && layoutTracker.isNull())
    {
        // layout is dead
        Q_ASSERT(false);
    }
    if (view && viewTracker.isNull())
    {
        // view is dead
        Q_ASSERT(false);
    }
    if (controller && controllerTracker.isNull())
    {
        // controller is dead
        Q_ASSERT(false);
    }
#endif
}

void ViewSchema::initOwner(QObject* owner)
{
    Q_ASSERT(layout);
    if (!layout->parent())
    {
        layout->setParent(owner);
    }

    Q_ASSERT(view);
    if (!view->parent())
    {
        view->setParent(owner);
    }

    if (controller && !controller->parent())
    {
        controller->setParent(owner);
    }

#ifndef QT_NO_DEBUG
    // start track objects lifetime
    layoutTracker = layout;
    viewTracker = view;
    controllerTracker = controller;
#endif
}

void ViewSchema::deleteIfOwnedBy(QObject* owner)
{
    Q_ASSERT(layout);
    if (layout->parent() == owner)
    {
        delete layout;
        layout = nullptr;
    }

    Q_ASSERT(view);
    if (view->parent() == owner)
    {
        delete view;
        view = nullptr;
    }

    if (controller && (controller->parent() == owner))
    {
        delete controller;
        controller = nullptr;
    }
}

CellsSchema::CellsSchema()
    : range(nullptr)
{
}

CellsSchema::~CellsSchema()
{
#ifndef QT_NO_DEBUG
    if (range && rangeTracker.isNull())
    {
        // range is dead
        Q_ASSERT(false);
    }
#endif
}

void CellsSchema::initOwner(QObject* owner)
{
    Q_ASSERT(range);
    if (!range->parent())
    {
        range->setParent(owner);
    }

#ifndef QT_NO_DEBUG
    rangeTracker = range;
#endif

    viewSchema.initOwner(owner);
}

void CellsSchema::deleteIfOwnedBy(QObject* owner)
{
    Q_ASSERT(range);
    if (range->parent() == owner)
    {
        delete range;
        range = nullptr;
    }

    viewSchema.deleteIfOwnedBy(owner);
}


} // end namespace Qi
