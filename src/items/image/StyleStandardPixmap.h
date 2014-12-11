#ifndef QI_STYLE_STANDARD_PIXMAP_H
#define QI_STYLE_STANDARD_PIXMAP_H

#include "core/View.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

class QI_EXPORT ViewStyleStandardPixmap: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewStyleStandardPixmap)

public:
    ViewStyleStandardPixmap(QStyle::StandardPixmap standardPixmap, bool useDefaultController = true);

    std::function<void (const ItemID& item, const ControllerContext& context, const ViewStyleStandardPixmap* view)> action;

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    QStyle::StandardPixmap m_standardPixmap;
    PushableTracker m_pushableTracker;
};

} // end namespace Qi

#endif // QI_STYLE_STANDARD_PIXMAP_H
