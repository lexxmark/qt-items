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

#ifndef QI_LAYOUTS_AUX_H
#define QI_LAYOUTS_AUX_H

#include "core/Layout.h"

namespace Qi
{

class QI_EXPORT LayoutHor : public Layout
{
    Q_DISABLE_COPY(LayoutHor)

public:
    LayoutHor(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutVer : public Layout
{
    Q_DISABLE_COPY(LayoutVer)

public:
    LayoutVer(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutSquareHor : public Layout
{
    Q_DISABLE_COPY(LayoutSquareHor)

public:
    LayoutSquareHor(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutSquareVer : public Layout
{
    Q_DISABLE_COPY(LayoutSquareVer)

public:
    LayoutSquareVer(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutFixedHor : public Layout
{
    Q_DISABLE_COPY(LayoutFixedHor)

public:
    LayoutFixedHor(int width, LayoutBehaviorMask behavior = LayoutBehaviorNone)
        : Layout(behavior),
          m_width(width)
    { Q_ASSERT(m_width >= 0); }

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;

    int m_width;
};

class QI_EXPORT LayoutFixedVer : public Layout
{
    Q_DISABLE_COPY(LayoutFixedVer)

public:
    LayoutFixedVer(int height, LayoutBehaviorMask behavior = LayoutBehaviorNone)
        : Layout(behavior),
          m_height(height)
    { Q_ASSERT(m_height >= 0); }

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;

    int m_height;
};

} // end namespace Qi

#endif // QI_LAYOUTS_AUX_H
