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
