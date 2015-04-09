#ifndef QI_PIXMAP_H
#define QI_PIXMAP_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"
#include <QPixmap>

namespace Qi
{

namespace Private
{
    template <>
    inline int compareValues(const QPixmap& /*leftValue*/, const QPixmap& /*rightValue*/)
    {
        return 0;
    }

    class NotEqPixmaps: std::binary_function<QPixmap, QPixmap, bool>
    {
    public:
        bool operator()(const QPixmap&, const QPixmap&) const
        { return false; }
    };
}

typedef ModelTyped<QPixmap> ModelPixmap;
typedef ModelCallback<QPixmap> ModelPixmapCallback;
typedef ModelStorageValue<QPixmap, QPixmap, Private::NotEqPixmaps> ModelPixmapValue;

class QI_EXPORT ViewPixmap: public ViewModeled<ModelPixmap>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewPixmap)

public:
    ViewPixmap(const QSharedPointer<ModelPixmap>& model);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
};

} // end namespace Qi

#endif // QI_PIXMAP_H
