#ifndef QI_IMAGE_H
#define QI_IMAGE_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ModelCallback.h"
#include <QImage>

namespace Qi
{

namespace Private
{
    template <>
    inline int compareValues(const QImage& /*leftValue*/, const QImage& /*rightValue*/)
    {
        return 0;
    }
}

typedef ModelTyped<QImage> ModelImage;
typedef ModelCallback<QImage> ModelImageCallback;

class QI_EXPORT ViewImage: public ViewModeled<ModelImage>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewImage)

public:
    ViewImage(const QSharedPointer<ModelImage>& model);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
};

} // end namespace Qi

#endif // QI_IMAGE_H
