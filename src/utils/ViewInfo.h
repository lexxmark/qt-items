#ifndef QI_VIEW_INFO_H
#define QI_VIEW_INFO_H

#include "Layout.h"

namespace Qi
{

struct QI_EXPORT ViewInfo
{
    QSharedPointer<View> view;
    QSharedPointer<Layout> layout;
};

} // end namespace Qi

#endif // QI_VIEW_INFO_H
