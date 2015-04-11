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

#include "ViewCacheSpace.h"
#include "cache/space/CacheSpace.h"

namespace Qi
{

ViewCacheSpace::ViewCacheSpace(const QSharedPointer<ModelCacheSpace>& model, bool useController)
    : m_model(model)
{
    if (useController)
        setController(QSharedPointer<ControllerMouseCacheSpace>::create(model));

    connect(m_model.data(), &Model::modelChanged, this, &ViewCacheSpace::onModelChanged);
}

ViewCacheSpace::~ViewCacheSpace()
{
    disconnect(m_model.data(), &Model::modelChanged, this, &ViewCacheSpace::onModelChanged);
}

CacheView* ViewCacheSpace::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    CacheView* result = View::addCacheViewImpl(layout, ctx, item, cacheViews, itemRect, visibleItemRect);

    if (!result)
        return result;

    const auto& cacheSpace = m_model->value(item);
    if (!cacheSpace)
        return result;

    // update cache window to the Rect which will be visible
    cacheSpace->setWindow(result->rect());

    return result;
}

QSize ViewCacheSpace::sizeImpl(const GuiContext& /*ctx*/, const ItemID& item, ViewSizeMode /*sizeMode*/) const
{
    const auto& cacheSpace = m_model->value(item);

    if (cacheSpace)
        return cacheSpace->space().size();
    else
        return QSize(0, 0);
}

void ViewCacheSpace::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    const auto& cacheSpace = m_model->value(cache.item);

    if (!cacheSpace)
        return;

    cacheSpace->draw(painter, ctx);
}

bool ViewCacheSpace::tooltipByPointImpl(const QPoint& point, const ItemID& item, TooltipInfo& tooltipInfo) const
{
    const auto& cacheSpace = m_model->value(item);

    if (!cacheSpace)
        return false;

    return cacheSpace->tooltipByPoint(point, tooltipInfo);
}

void ViewCacheSpace::onModelChanged(const Model*)
{
    emitViewChanged(ChangeReasonViewContent);
}

ControllerMouseCacheSpace::ControllerMouseCacheSpace(const QSharedPointer<ModelCacheSpace>& model)
    : m_model(model)
{
}

void ControllerMouseCacheSpace::tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo)
{
    const auto& cacheSpace = m_model->value(activationInfo.cache.item);

    if (!cacheSpace)
        return;

    cacheSpace->tryActivateControllers(activationInfo.context, activatedControllers);
}

} // end namespace Qi
