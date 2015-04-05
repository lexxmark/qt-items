#include "Rating.h"
#include "items/misc/ControllerMousePushableCallback.h"

namespace Qi
{

static const int imageGap = 2;

class QI_EXPORT ControllerMouseRating: public ControllerMousePushable
{
    Q_DISABLE_COPY(ControllerMouseRating)

public:
    ControllerMouseRating(ViewRating* viewRating)
        : m_viewRating(viewRating)
    {}

protected:
    void applyImpl() override
    {
        Q_ASSERT(!m_viewRating.isNull());

        const ActivationState& state = activationState();
        int rate = (state.context.point.x() - state.viewRect.left()) / (m_viewRating->rateImageOn().width() + imageGap) + 1;
        if (rate < 0)
            rate = 0;
        else if (rate > m_viewRating->maxRate())
            rate = m_viewRating->maxRate();

        m_viewRating->theModel()->setValue(state.item, rate);
    }

private:
    QPointer<ViewRating> m_viewRating;
};

ViewRating::ViewRating(const QSharedPointer<ModelRating>& model, const QPixmap& rateImageOn, const QPixmap& rateImageOff, int maxRate, bool useDefaultController)
    : ViewModeled<ModelRating>(model),
      m_rateImageOn(rateImageOn),
      m_rateImageOff(rateImageOff),
      m_maxRate(maxRate)
{
    Q_ASSERT(m_rateImageOn.size() == m_rateImageOff.size());
    Q_ASSERT(m_maxRate > 0);

    if (useDefaultController)
    {
        setController(QSharedPointer<ControllerMouseRating>::create(this));
    }
}

QSize ViewRating::sizeImpl(const GuiContext& /*ctx*/, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    return QSize((m_rateImageOn.width()+imageGap)*m_maxRate, m_rateImageOn.size().height());
}

void ViewRating::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    int rating = theModel()->value(cache.item);

    int rateImageWidth = m_rateImageOn.width() + imageGap;
    QPoint starPoint(cache.cacheView.rect().topLeft());

    painter->save();
    painter->setClipRect(cache.cacheView.rect(), Qt::IntersectClip);

    for (int i = 0; i < m_maxRate; ++i)
    {
        if (i < rating)
            painter->drawPixmap(starPoint, m_rateImageOn);
        else
            painter->drawPixmap(starPoint, m_rateImageOff);
       starPoint.rx() += rateImageWidth;
    }

    painter->restore();
}

} // end namespace Qi
