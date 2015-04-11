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

#ifndef QI_VIEW_CACHE_H
#define QI_VIEW_CACHE_H

#include "core/View.h"
#include "core/ControllerMouse.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"

namespace Qi
{

class CacheSpace;

typedef ModelTyped<QSharedPointer<CacheSpace>> ModelCacheSpace;
typedef ModelStorageValue<QSharedPointer<CacheSpace>> ModelCacheSpaceValue;
typedef ModelCallback<QSharedPointer<CacheSpace>> ModelCacheSpaceCallback;

class QI_EXPORT ViewCacheSpace: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCacheSpace)

public:
    ViewCacheSpace(const QSharedPointer<ModelCacheSpace>& model, bool useController = true);
    virtual ~ViewCacheSpace();

protected:
    CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const override;
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    bool tooltipByPointImpl(const QPoint& point, const ItemID& item, TooltipInfo &tooltipInfo) const override;

private slots:
    void onModelChanged(const Model*);

private:
    QSharedPointer<ModelCacheSpace> m_model;
};

class QI_EXPORT ControllerMouseCacheSpace: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseCacheSpace)

public:
    ControllerMouseCacheSpace(const QSharedPointer<ModelCacheSpace>& model);

private:
    void tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo) override;

    QSharedPointer<ModelCacheSpace> m_model;
};

} // end namespace Qi

#endif // QI_VIEW_CACHE_H
