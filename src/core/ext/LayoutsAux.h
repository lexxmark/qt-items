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

} // end namespace Qi

#endif // QI_LAYOUTS_AUX_H
