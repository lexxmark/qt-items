#ifndef QI_WIDGET_DRIVER_H
#define QI_WIDGET_DRIVER_H

#include "core/ItemID.h"

class QKeyEvent;

namespace Qi
{

class CacheSpace;

// this stuff should support each items widget
class WidgetDriver
{
    Q_DISABLE_COPY(WidgetDriver)

public:
    // scrolls widget to make visibleItem fully visible
    virtual void ensureVisible(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) = 0;
    // performs inplace editing for visibleItem
    virtual bool doEdit(const ItemID& visibleItem, const CacheSpace *cacheSpace, const QKeyEvent* event) = 0;

protected:
    // make it protected so client code
    // is not able to create/delete such objects
    WidgetDriver() {}
    ~WidgetDriver() {}
};

} // end namespace Qi

#endif // QI_WIDGET_DRIVER_H
