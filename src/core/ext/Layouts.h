#ifndef QI_LAYOUTS_H
#define QI_LAYOUTS_H

#include "core/Layout.h"

namespace Qi
{

class QI_EXPORT LayoutCenter : public Layout
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutCenter)

public:
    LayoutCenter(LayoutBehaviorMask behavior) : Layout(behavior) {}

protected:
    bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const override;
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutHor : public Layout
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutHor)

public:
    LayoutHor(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutVer : public Layout
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutVer)

public:
    LayoutVer(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
};

class QI_EXPORT LayoutClient : public Layout
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutClient)

public:
    LayoutClient(LayoutBehaviorMask behavior = LayoutBehaviorNone) : Layout(behavior) {}

protected:
    bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const override;
    void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const override;
    bool isFinalImpl() const override { return !isTransparent(); }
};

class QI_EXPORT LayoutLeft : public LayoutHor
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutLeft)

public:
    LayoutLeft(LayoutBehaviorMask behavior = LayoutBehaviorNone) : LayoutHor(behavior) {}

protected:
    bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const override;
};

class QI_EXPORT LayoutRight : public LayoutHor
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutRight)

public:
    LayoutRight(LayoutBehaviorMask behavior = LayoutBehaviorNone) : LayoutHor(behavior) {}

protected:
    bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const override;
};

class QI_EXPORT LayoutTop : public LayoutVer
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutTop)

public:
    LayoutTop(LayoutBehaviorMask behavior = LayoutBehaviorNone) : LayoutVer(behavior) {}

protected:
    bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const override;
};

class QI_EXPORT LayoutBottom : public LayoutVer
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutBottom)

public:
    LayoutBottom(LayoutBehaviorMask behavior = LayoutBehaviorNone) : LayoutVer(behavior) {}

protected:
    bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const override;
};

} // end namespace Qi

#endif // QI_LAYOUTS_H
