#ifndef QI_CELLS_SCHEMA_H
#define QI_CELLS_SCHEMA_H

#include "Layout.h"
#include "Range.h"
#include <QSharedPointer>

namespace Qi
{

struct QI_EXPORT ViewSchema
{
    QSharedPointer<View> view;
    QSharedPointer<Layout> layout;
};

struct QI_EXPORT CellsSchema
{
    QSharedPointer<Range> range;
    ViewSchema vSchema;
};

} // end namespace Qi

#endif // QI_CELLS_SCHEMA_H
