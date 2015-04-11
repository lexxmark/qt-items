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

#include "Image.h"

namespace Qi
{

ViewImage::ViewImage(const QSharedPointer<ModelImage>& model)
    : ViewModeled<ModelImage>(model)
{
}

QSize ViewImage::sizeImpl(const GuiContext& /*ctx*/, const ItemID& item, ViewSizeMode /*sizeMode*/) const
{
    QImage image = theModel()->value(item);
    return image.size();
}

void ViewImage::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QImage image = theModel()->value(cache.item);
    QRect viewRect = cache.cacheView.rect();
    int x = viewRect.left() + (viewRect.width() - image.width()) / 2;
    int y = viewRect.top() + (viewRect.height() - image.height()) / 2;

    painter->drawImage(x, y, image);
}

} // end namespace Qi
