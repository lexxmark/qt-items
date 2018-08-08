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

#include "Layout.h"
#include "View.h"

namespace Qi
{

QSize Layout::ViewInfo::size() const
{
    return view.size(ctx, id, sizeMode);
}

bool Layout::doLayout(const View2& view, const GuiContext& ctx, ID id, ViewSizeMode sizeMode, QRect& viewRect, QRect& itemRect, QRect* visibleItemRect) const
{
    ViewInfo vi(view, ctx, id, sizeMode);
    LayoutInfo li(viewRect, itemRect);
    if (!doLayoutImpl(vi, li))
        return false;

    if (isFloat() && visibleItemRect)
    {
        LayoutInfo li(viewRect, *visibleItemRect);
        if (!doLayoutImpl(vi, li))
            return false;
    }

    return true;
}

void Layout::expandSize(const View2& view, const GuiContext& ctx, ID id, ViewSizeMode sizeMode, QSize& size) const
{
    if (isTransparent())
        return;

    expandSizeImpl(ViewInfo(view, ctx, id, sizeMode), size);
}

bool Layout::isFinal() const
{
    return isFinalImpl();
}

} // end namespace Qi
