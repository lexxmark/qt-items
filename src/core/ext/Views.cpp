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

QSize ViewCallback::sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const
{
    if (sizeFunction)
        return sizeFunction(ctx, id, sizeMode);
    return View::sizeImpl(ctx, id, sizeMode);
}

bool ViewCallback::textImpl(ID id, QString& txt) const
{
    if (textFunction)
        return textFunction(id, txt);
    return View::textImpl(id, txt);
}

} // end namespace Qi
