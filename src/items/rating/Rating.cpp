/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
