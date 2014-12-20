#ifndef QI_COLOR_H
#define QI_COLOR_H

#include "core/ext/ModelCallback.h"
#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

namespace Private
{
    template <>
    inline int compareValues(const QColor& /*leftValue*/, const QColor& /*rightValue*/)
    {
        return 0;
    }
}

typedef ModelTyped<QColor> ModelColor;
typedef ModelCallback<QColor> ModelColorCallback;

class QI_EXPORT ViewColor: public ViewModeled<ModelColor>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewColor)

public:
    ViewColor(const QSharedPointer<ModelColor>& model, bool useDefaultController = true, bool withBorder = true);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    bool m_withBorder;
};

QI_EXPORT QSharedPointer<ControllerMousePushable> createControllerMouseColor(const QSharedPointer<ModelColor>& model);


} // end namespace Qi

#endif // QI_COLOR_H
