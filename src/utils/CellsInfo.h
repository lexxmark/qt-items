#ifndef QI_CELLS_INFO_H
#define QI_CELLS_INFO_H

#include "Layout.h"
#include "Range.h"
#include <QSharedPointer>

namespace Qi
{

struct QI_EXPORT CellsInfo
{
    QSharedPointer<Range> range;
    QSharedPointer<View> view;
    QSharedPointer<Layout> layout;
};

} // end namespace Qi

#endif // QI_CELLS_INFO_H
