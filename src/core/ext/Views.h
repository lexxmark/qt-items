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
