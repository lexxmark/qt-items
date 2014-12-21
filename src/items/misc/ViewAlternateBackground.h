#ifndef QI_VIEW_ALTERNATE_BACKGROUND_H
#define QI_VIEW_ALTERNATE_BACKGROUND_H

#include "core/View.h"

namespace Qi
{

class QI_EXPORT ViewAlternateBackground: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewAlternateBackground)

public:
    ViewAlternateBackground();

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
};

} // end namespace Qi

#endif // QI_VIEW_ALTERNATE_BACKGROUND_H
