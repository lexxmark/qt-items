#ifndef QI_VIEWS_H
#define QI_VIEWS_H

#include "core/View.h"
#include "core/Model.h"

namespace Qi
{

class QI_EXPORT ViewCallback: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCallback)

public:
    ViewCallback()
    {}

    std::function<void(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip)> drawFunction;
    std::function<QSize(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode)> sizeFunction;
    std::function<bool(const ItemID& item, QString& txt)> textFunction;


protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    bool textImpl(const ItemID& item, QString& txt) const override;
};

} // end namespace Qi

#endif // QI_VIEWS_H
