/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
