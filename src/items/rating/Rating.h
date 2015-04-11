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

#ifndef QI_RATING_H
#define QI_RATING_H

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

#endif // QI_RATING_H
