#ifndef QI_STARS_H
#define QI_STARS_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ModelCallback.h"
#include <QPixmap>

namespace Qi
{

typedef ModelTyped<int> ModelRating;
typedef ModelCallback<int> ModelRatingCallback;

class QI_EXPORT ViewRating: public ViewModeled<ModelRating>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewRating)

public:
    ViewRating(const QSharedPointer<ModelRating>& model, const QPixmap& rateImageOn, const QPixmap& rateImageOff, int maxRate = 5, bool useDefaultController= true);

    QPixmap rateImageOn() const { return m_rateImageOn; }
    QPixmap rateImageOff() const { return m_rateImageOff; }
    int maxRate() const { return m_maxRate; }

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    QPixmap m_rateImageOn;
    QPixmap m_rateImageOff;
    int m_maxRate;
};

} // end namespace Qi

#endif // QI_STARS_H
