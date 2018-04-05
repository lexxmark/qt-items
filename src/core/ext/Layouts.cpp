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

#include "Layouts.h"

namespace Qi
{

SharedPtr<Qi::Layout> makeLayoutBackground()
{
    static SharedPtr<Layout> layout = makeShared<LayoutClient>(LayoutBehaviorTransparent);
    return layout;
}

SharedPtr<Layout> makeLayoutClient()
{
    static SharedPtr<Layout> layout = makeShared<LayoutClient>(LayoutBehaviorNone);
    return layout;
}

SharedPtr<Qi::Layout> makeLayoutCenter()
{
    static SharedPtr<Layout> layout = makeShared<LayoutCenter>(LayoutBehaviorNone);
    return layout;
}

SharedPtr<Layout> makeLayoutLeft(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutLeft>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutLeft>(behavior);
}

SharedPtr<Qi::Layout> makeLayoutRight(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutRight>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutRight>(behavior);
}

SharedPtr<Layout> makeLayoutTop(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutTop>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutTop>(behavior);
}

SharedPtr<Qi::Layout> makeLayoutBottom(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutBottom>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutBottom>(behavior);
}

SharedPtr<Layout> makeLayoutSquareLeft(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutSquareLeft>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutSquareLeft>(behavior);
}

SharedPtr<Qi::Layout> makeLayoutSquareRight(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutSquareRight>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutSquareRight>(behavior);
}

SharedPtr<Layout> makeLayoutSquareTop(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutSquareTop>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutSquareTop>(behavior);
}

SharedPtr<Qi::Layout> makeLayoutSquareBottom(LayoutBehaviorMask behavior)
{
    if (behavior == LayoutBehaviorNone)
    {
        static SharedPtr<Layout> layout = makeShared<LayoutSquareBottom>(LayoutBehaviorNone);
        return layout;
    }

    return makeShared<LayoutSquareBottom>(behavior);
}

SharedPtr<Layout> makeLayoutFixedLeft(int width, LayoutBehaviorMask behavior)
{
    return makeShared<LayoutFixedLeft>(width, behavior);
}

SharedPtr<Qi::Layout> makeLayoutFixedRight(int width, LayoutBehaviorMask behavior)
{
    return makeShared<LayoutFixedRight>(width, behavior);
}

SharedPtr<Layout> makeLayoutFixedTop(int height, LayoutBehaviorMask behavior)
{
    return makeShared<LayoutFixedTop>(height, behavior);
}

SharedPtr<Qi::Layout> makeLayoutFixedBottom(int height, LayoutBehaviorMask behavior)
{
    return makeShared<LayoutFixedBottom>(height, behavior);
}

bool LayoutCenter::doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const
{
    info.viewRect = info.itemRect;

    QSize viewSize = viewInfo.size();

    int deltaHeight = (info.itemRect.height() - viewSize.height()) / 2;
    if (deltaHeight > 0)
    {
        info.viewRect.setTop(info.viewRect.top() + deltaHeight);
        info.viewRect.setBottom(info.viewRect.bottom() - deltaHeight);
    }

    int deltaWidth = (info.itemRect.width() - viewSize.width()) / 2;
    if (deltaWidth > 0)
    {
        info.viewRect.setLeft(info.viewRect.left() + deltaWidth);
        info.viewRect.setRight(info.viewRect.right() - deltaWidth);
    }

    return true;
}

void LayoutCenter::expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const
{
    QSize viewSize = viewInfo.size();
    size.rwidth() = qMax(size.width(), viewSize.width());
    size.rheight() = qMax(size.height(), viewSize.height());
}

bool LayoutClient::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    info.viewRect = info.itemRect;
    if (!isTransparent())
        info.itemRect.setLeft(info.viewRect.right()+1);

    return true;
}

void LayoutClient::expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const
{
    QSize viewSize = viewInfo.size();
    size.rwidth() += (viewSize.width()+1);
    size.rheight() += (viewSize.height()+1);
}

bool LayoutLeft::doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const
{
    QSize viewSize = viewInfo.size();

    info.viewRect = info.itemRect;
    info.viewRect.setRight(qMin(info.viewRect.right(), info.viewRect.left() + viewSize.width()));
    if (!isTransparent())
        info.itemRect.setLeft(info.viewRect.right()+1);

    return true;
}

bool LayoutRight::doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const
{
    QSize viewSize = viewInfo.size();

    info.viewRect = info.itemRect;
    info.viewRect.setLeft(qMax(info.viewRect.left(), info.viewRect.right() - viewSize.width()));
    if (!isTransparent())
        info.itemRect.setRight(info.viewRect.left()-1);

    return true;
}

bool LayoutTop::doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const
{
    QSize viewSize = viewInfo.size();

    info.viewRect = info.itemRect;
    info.viewRect.setBottom(qMin(info.viewRect.bottom(), info.viewRect.top() + viewSize.height()));
    if (!isTransparent())
        info.itemRect.setTop(info.viewRect.bottom()+1);

    return true;
}

bool LayoutBottom::doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const
{
    QSize viewSize = viewInfo.size();

    info.viewRect = info.itemRect;
    info.viewRect.setTop(qMax(info.viewRect.top(), info.viewRect.bottom() - viewSize.height()));
    if (!isTransparent())
        info.itemRect.setBottom(info.viewRect.top()-1);

    return true;
}

bool LayoutSquareLeft::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    QSize viewSize = QSize(info.itemRect.height(), info.itemRect.height());

    info.viewRect = info.itemRect;
    info.viewRect.setRight(qMin(info.viewRect.right(), info.viewRect.left() + viewSize.width()));
    if (!isTransparent())
        info.itemRect.setLeft(info.viewRect.right()+1);

    return true;
}

bool LayoutSquareRight::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    QSize viewSize = QSize(info.itemRect.height(), info.itemRect.height());

    info.viewRect = info.itemRect;
    info.viewRect.setLeft(qMax(info.viewRect.left(), info.viewRect.right() - viewSize.width()));
    if (!isTransparent())
        info.itemRect.setRight(info.viewRect.left()-1);

    return true;
}

bool LayoutSquareTop::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    QSize viewSize = QSize(info.itemRect.width(), info.itemRect.width());

    info.viewRect = info.itemRect;
    info.viewRect.setBottom(qMin(info.viewRect.bottom(), info.viewRect.top() + viewSize.height()));
    if (!isTransparent())
        info.itemRect.setTop(info.viewRect.bottom()+1);

    return true;
}

bool LayoutSquareBottom::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    QSize viewSize = QSize(info.itemRect.width(), info.itemRect.width());

    info.viewRect = info.itemRect;
    info.viewRect.setTop(qMax(info.viewRect.top(), info.viewRect.bottom() - viewSize.height()));
    if (!isTransparent())
        info.itemRect.setBottom(info.viewRect.top()-1);

    return true;
}

bool LayoutFixedLeft::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    info.viewRect = info.itemRect;
    info.viewRect.setRight(qMin(info.viewRect.right(), info.viewRect.left() + m_width));
    if (!isTransparent())
        info.itemRect.setLeft(info.viewRect.right()+1);

    return true;
}

bool LayoutFixedRight::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    info.viewRect = info.itemRect;
    info.viewRect.setLeft(qMax(info.viewRect.left(), info.viewRect.right() - m_width));
    if (!isTransparent())
        info.itemRect.setRight(info.viewRect.left()-1);

    return true;
}

bool LayoutFixedTop::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    info.viewRect = info.itemRect;
    info.viewRect.setBottom(qMin(info.viewRect.bottom(), info.viewRect.top() + m_height));
    if (!isTransparent())
        info.itemRect.setTop(info.viewRect.bottom()+1);

    return true;
}

bool LayoutFixedBottom::doLayoutImpl(const ViewInfo& /*viewInfo*/, LayoutInfo& info) const
{
    info.viewRect = info.itemRect;
    info.viewRect.setTop(qMax(info.viewRect.top(), info.viewRect.bottom() - m_height));
    if (!isTransparent())
        info.itemRect.setBottom(info.viewRect.top()-1);

    return true;
}

} // end namespace Qi
