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

#include "Check.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewCheck::ViewCheck(SharedPtr<ModelCheck> model, bool useDefaultController)
    : ViewModeled<ModelCheck>(model),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        setController(createControllerMouseCheck(model));
    }
}

QSize ViewCheck::sizeImpl(const GuiContext& ctx, ID /*id*/, ViewSizeMode /*sizeMode*/) const
{
    auto style = ctx.style();
    return QSize(style->pixelMetric(QStyle::PM_IndicatorWidth),
                 style->pixelMetric(QStyle::PM_IndicatorHeight));
}

void ViewCheck::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    auto style = ctx.style();

    QStyleOptionButton option;
    ctx.initStyleOption(option);

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.state |= styleState(cache.id);
    option.rect = cache.cacheView.rect();
    // correct rect
    option.rect = style->subElementRect(QStyle::SE_CheckBoxIndicator, &option, ctx.widget);

    // draw check box image
    style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, ctx.widget);
}

QStyle::State ViewCheck::styleState(ID id) const
{
    Qt::CheckState check = theModel()->value(id);

    QStyle::State state = m_pushableTracker.styleStateByItem(id);

    switch (check)
    {
    case Qt::Checked:
        state |= QStyle::State_On;
        break;
    case Qt::PartiallyChecked:
        state |= QStyle::State_NoChange;
        break;
    case Qt::Unchecked:
        state |= QStyle::State_Off;
        break;
    }

    return state;
}

SharedPtr<ControllerMousePushable> createControllerMouseCheck(const SharedPtr<ModelCheck>& model)
{
    auto controller = makeShared<ControllerMousePushableCallback>();
    controller->onApply = [model] (ID id, const ControllerContext& /*context*/) {
        Qt::CheckState check = model->value(id);
        model->setValue(id, (check != Qt::Unchecked) ? Qt::Unchecked : Qt::Checked);
    };
    return controller;
}

} // end namespace Qi
