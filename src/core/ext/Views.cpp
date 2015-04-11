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

#include "Views.h"
#include <QStyleOptionButton>

namespace Qi
{

void ViewCallback::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    Q_ASSERT(drawFunction);
    drawFunction(painter, ctx, cache, showTooltip);
}

QSize ViewCallback::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    if (sizeFunction)
        return sizeFunction(ctx, item, sizeMode);
    return View::sizeImpl(ctx, item, sizeMode);
}

bool ViewCallback::textImpl(const ItemID& item, QString& txt) const
{
    if (textFunction)
        return textFunction(item, txt);
    return View::textImpl(item, txt);
}

} // end namespace Qi
