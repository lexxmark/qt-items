#ifndef QI_ITEMS_SCHEMA_H
#define QI_ITEMS_SCHEMA_H

#include "Layout.h"
#include "Range.h"

namespace Qi
{

struct QI_EXPORT ViewSchema
{
    QSharedPointer<Layout> layout;
    QSharedPointer<View> view;

    ViewSchema() {}
    ViewSchema(QSharedPointer<Layout> layout, QSharedPointer<View> view)
        : layout(layout),
          view(view)
    {}

    bool isValid() const { return layout && view; }
};

struct QI_EXPORT ItemSchema
{
    QSharedPointer<Range> range;
    QSharedPointer<Layout> layout;
    QSharedPointer<View> view;

    ItemSchema() {}
    ItemSchema(QSharedPointer<Range> range, QSharedPointer<Layout> layout, QSharedPointer<View> view)
        : range(range),
          layout(layout),
          view(view)
    {}
};

} // end namespace Qi

#endif // QI_ITEMS_SCHEMA_H
