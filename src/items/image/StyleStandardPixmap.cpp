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

#include "StyleStandardPixmap.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QStyleOptionButton>

namespace Qi
{

ViewStyleStandardPixmap::ViewStyleStandardPixmap(QStyle::StandardPixmap standardPixmap, bool useDefaultController)
    : m_standardPixmap(standardPixmap),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        auto controller = QSharedPointer<ControllerMousePushableCallback>::create();
        controller->onApply = [this] (const ItemID& item, const ControllerContext& context) {
            Q_ASSERT(item.isValid());
            if (action)
                action(item, context, this);
        };
        setController(controller);
    }
}

QSize ViewStyleStandardPixmap::sizeImpl(const GuiContext& ctx, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    auto style = ctx.style();
    QIcon standardIcon = style->standardIcon(m_standardPixmap, nullptr, ctx.widget);
    auto sizes = standardIcon.availableSizes();
    if (sizes.isEmpty())
        return QSize(0, 0);

    return sizes.front();
}

void ViewStyleStandardPixmap::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    auto style = ctx.style();

    QStyleOption option;
    ctx.initStyleOption(option);

    // dont initialize styleObject from widget for QWindowsVistaStyle
    // this disables buggous animations
    if (style->inherits("QWindowsVistaStyle"))
        option.styleObject = nullptr;

    option.state |= m_pushableTracker.styleStateByItem(cache.item);
    option.rect = cache.cacheView.rect();

    // draw button
    QIcon standardIcon = style->standardIcon(m_standardPixmap, &option, ctx.widget);
    QIcon::Mode mode = QIcon::Disabled;
    if (option.state & QStyle::State_Enabled)
        mode = QIcon::Normal;
    if (option.state & QStyle::State_Selected)
        mode = QIcon::Selected;
    if (option.state & QStyle::State_MouseOver)
        mode = QIcon::Active;

    standardIcon.paint(painter, option.rect, Qt::AlignCenter, mode);
}

} // end namespace Qi
