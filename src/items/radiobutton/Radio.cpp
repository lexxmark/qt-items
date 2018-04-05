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

#include "Radio.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

int ModelRadio::compareImpl(ID left, ID right) const
{
    if (isRadioItem(left))
        return 1;
    else if (isRadioItem(right))
        return -1;
    else
        return 0;
}

bool ModelRadioCallback::isRadioItemImpl(ID id) const
{
    Q_ASSERT(isRadioItem);
    return isRadioItem(id);
}

bool ModelRadioCallback::setRadioItemImpl(ID id)
{
    Q_ASSERT(setRadioItem);

    if (setRadioItem(id))
    {
        emit modelChanged(this);
        return true;
    }

    return false;
}

ModelRadioStorage::ModelRadioStorage(ID radioId)
    : m_radioId(radioId)
{
}

bool ModelRadioStorage::isRadioItemImpl(ID id) const
{
    return m_radioId ==id;
}

bool ModelRadioStorage::setRadioItemImpl(ID id)
{
    if (m_radioId == id)
        return false;

    m_radioId = id;
    emit modelChanged(this);

    return true;
}

ViewRadio::ViewRadio(const SharedPtr<ModelRadio>& model, bool useDefaultController)
    : ViewModeled<ModelRadio>(model),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        setController(createControllerMouseRadio(model));
    }
}

QSize ViewRadio::sizeImpl(const GuiContext& ctx, ID /*id*/, ViewSizeMode /*sizeMode*/) const
{
    auto style = ctx.style();
    return QSize(style->pixelMetric(QStyle::PM_ExclusiveIndicatorWidth),
                 style->pixelMetric(QStyle::PM_ExclusiveIndicatorHeight));
}

void ViewRadio::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
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
    option.rect = style->subElementRect(QStyle::SE_RadioButtonIndicator, &option, ctx.widget);

    // draw radio button image
    style->drawPrimitive(QStyle::PE_IndicatorRadioButton, &option, painter, ctx.widget);
}

QStyle::State ViewRadio::styleState(ID id) const
{
    bool isRadioItem = theModel()->isRadioItem(id);

    QStyle::State state = m_pushableTracker.styleStateByItem(id);
    state |= (isRadioItem ? QStyle::State_On : QStyle::State_Off);

    return state;
}

SharedPtr<ControllerMousePushable> createControllerMouseRadio(SharedPtr<ModelRadio> model)
{
    auto controller = makeShared<ControllerMousePushableCallback>();
    controller->onApply = [model] (ID item, const ControllerContext& /*context*/) {
        model->setRadioItem(item);
    };
    return controller;
}

} // end namespace Qi
