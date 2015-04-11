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

#ifndef QI_COLOR_H
#define QI_COLOR_H

#include "core/ext/ModelCallback.h"
#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

namespace Private
{
    template <>
    inline int compareValues(const QColor& /*leftValue*/, const QColor& /*rightValue*/)
    {
        return 0;
    }
}

typedef ModelTyped<QColor> ModelColor;
typedef ModelCallback<QColor> ModelColorCallback;

class QI_EXPORT ViewColor: public ViewModeled<ModelColor>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewColor)

public:
    ViewColor(const QSharedPointer<ModelColor>& model, bool useDefaultController = true, bool withBorder = true);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    bool m_withBorder;
};

QI_EXPORT QSharedPointer<ControllerMousePushable> createControllerMouseColor(const QSharedPointer<ModelColor>& model);


} // end namespace Qi

#endif // QI_COLOR_H
