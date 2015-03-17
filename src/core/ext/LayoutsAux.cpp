#include "LayoutsAux.h"

namespace Qi
{

void LayoutHor::expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const
{
    QSize viewSize = viewInfo.size();
    size.rwidth() += (viewSize.width()+1);
    size.rheight() = qMax(size.height(), viewSize.height());
}

void LayoutVer::expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const
{
    QSize viewSize = viewInfo.size();
    size.rwidth() = qMax(size.width(), viewSize.width());
    size.rheight() += (viewSize.height()+1);
}

void LayoutSquareHor::expandSizeImpl(const ViewInfo& /*viewInfo*/, QSize& size) const
{
    QSize viewSize = QSize(size.height(), size.height());
    size.rwidth() += (viewSize.width()+1);
    size.rheight() = qMax(size.height(), viewSize.height());
}

void LayoutSquareVer::expandSizeImpl(const ViewInfo& /*viewInfo*/, QSize& size) const
{
    QSize viewSize = QSize(size.width(), size.width());
    size.rwidth() = qMax(size.width(), viewSize.width());
    size.rheight() += (viewSize.height()+1);
}

void LayoutFixedHor::expandSizeImpl(const ViewInfo& /*viewInfo*/, QSize& size) const
{
    size.rwidth() += m_width+1;
}

void LayoutFixedVer::expandSizeImpl(const ViewInfo& /*viewInfo*/, QSize& size) const
{
    size.rheight() += m_height+1;
}

} // end namespace Qi
