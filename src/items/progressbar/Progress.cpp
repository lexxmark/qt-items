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

#include "Progress.h"
#include <QStyleOptionProgressBar>

namespace Qi
{

ViewProgressBase::ViewProgressBase(SharedPtr<ModelProgress> model)
    : ViewModeled<ModelProgress>(std::move(model))
{
}

ViewProgressContents::ViewProgressContents(SharedPtr<ModelProgress> model)
    : ViewProgressBase(std::move(model))
{
}

void ViewProgressContents::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    float value = theModel()->value(cache.id);
    if (tuneValue)
    {
        if (!tuneValue(value, cache.id))
            return;
    }

    Q_ASSERT(value >= 0.f || qFuzzyCompare(value, 0.f));
    Q_ASSERT(value <= 1.f || qFuzzyCompare(value, 1.f));
    value = qBound(0.f, value, 1.f);

    auto style = ctx.style();

    QStyleOptionProgressBar option;
    ctx.initStyleOption(option);
    option.textVisible = false;
    option.minimum = 0;
    option.maximum = 100;
    option.progress = int(100.f * value);

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.rect = cache.cacheView.rect();
    option.rect.adjust(2, 2, -2, -2);

    // draw progress
    style->drawControl(QStyle::CE_ProgressBarContents, &option, painter, ctx.widget);
}

ViewProgressLabel::ViewProgressLabel(SharedPtr<ModelProgress> model, ProgressLabelMode mode)
    : ViewProgressBase(std::move(model)),
      m_mode(mode)
{
}

void ViewProgressLabel::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    float value = theModel()->value(cache.id);
    if (tuneValue)
    {
        if (!tuneValue(value, cache.id))
            return;
    }

    Q_ASSERT(value >= 0.f || qFuzzyCompare(value, 0.f));
    Q_ASSERT(value <= 1.f || qFuzzyCompare(value, 1.f));
    value = qBound(0.f, value, 1.f);

    auto style = ctx.style();

    QStyleOptionProgressBar option;
    ctx.initStyleOption(option);
    option.textVisible = true;
    option.minimum = 0;
    option.maximum = 100;
    option.progress = int(100.f * value);

    switch (m_mode)
    {
    case ProgressLabelModeNormalized:
        option.text.setNum(value, 'g', 2);
        break;

    case ProgressLabelModeProgress:
        option.text.setNum(value*100.f, 'g', 2);
        break;

    case ProgressLabelModePercent:
        option.text.setNum(value*100.f, 'g', 2);
        option.text += '%';
        break;
    }

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.rect = cache.cacheView.rect();
    option.rect.adjust(2, 2, -2, -2);

    // draw progress
    style->drawControl(QStyle::CE_ProgressBarLabel, &option, painter, ctx.widget);
}

ViewProgressBox::ViewProgressBox(SharedPtr<ModelProgress> model)
    : ViewProgressBase(std::move(model))
{
    contentsColor = Qt::magenta;
    boundsColor = contentsColor.darker();
}

void ViewProgressBox::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    float value = theModel()->value(cache.id);
    if (tuneValue)
    {
        if (!tuneValue(value, cache.id))
            return;
    }

    Q_ASSERT(value >= 0.f || qFuzzyCompare(value, 0.f));
    Q_ASSERT(value <= 1.f || qFuzzyCompare(value, 1.f));
    value = qBound(0.f, value, 1.f);

    QRect rect = cache.cacheView.rect();
    rect.adjust(0, 2, 0, -2);
    rect.setRight(rect.left() + (int)(value * (float)rect.width()));

    if (rect.width() > 0)
    {
        PainterState state;
        state.save(painter);

        painter->fillRect(rect, boundsColor);
        rect.adjust(1, 1, -1, -1);
        painter->fillRect(rect, contentsColor);

//        painter->setPen(boundsColor);
//        painter->setBrush(contentsColor);
//        painter->drawRect(rect);

        state.restore(painter);
    }
}

} // end namespace Qi
