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

#include "Color.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QColorDialog>

namespace Qi
{

ViewColor::ViewColor(SharedPtr<ModelColor> model, bool useDefaultController, bool withBorder)
    : ViewModeled<ModelColor>(std::move(model)),
      m_withBorder(withBorder)
{
    if (useDefaultController)
    {
        setController(createControllerMouseColor(theModel()));
    }
}

void ViewColor::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QColor color = theModel()->value(cache.id);
    if (!color.isValid())
        return;

    QRect rect = cache.cacheView.rect();

    if (m_withBorder)
    {
        rect.adjust(2, 2, -2, -2);

        painter->fillRect(rect, Qt::black);
        rect.adjust(1, 1, -1, -1);
        painter->fillRect(rect, color);
/*
        rect.adjust(2, 2, -2, -2);

        QBrush oldBrush = painter->brush();
        painter->setBrush(color);
        painter->drawRect(rect);
        painter->setBrush(oldBrush);
        */
    }
    else
    {
        painter->fillRect(rect, color);
    }
}

SharedPtr<ControllerMousePushable> createControllerMouseColor(SharedPtr<ModelColor> model)
{
    auto controller = makeShared<ControllerMousePushableCallback>();
    controller->onApply = [model = std::move(model)] (ID id, const ControllerContext& context) {
        QColorDialog dlg(model->value(id), context.widget);
        if (dlg.exec() == QDialog::Accepted)
        {
            model->setValue(id, dlg.currentColor());
        }
    };
    return controller;
}

} // end namespace Qi
