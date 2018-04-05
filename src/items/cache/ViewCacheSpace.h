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

typedef ModelTyped<SharedPtr<CacheSpace>> ModelCacheSpace;
typedef ModelStorageValue<SharedPtr<CacheSpace>> ModelCacheSpaceValue;
typedef ModelCallback<SharedPtr<CacheSpace>> ModelCacheSpaceCallback;

class QI_EXPORT ViewCacheSpace: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCacheSpace)

public:
    ViewCacheSpace(const SharedPtr<ModelCacheSpace>& model, bool useController = true);
    virtual ~ViewCacheSpace();

protected:
    CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, ID id, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const override;
    QSize sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    bool tooltipByPointImpl(QPoint point, ID item, TooltipInfo &tooltipInfo) const override;

private slots:
    void onModelChanged(const Model*);

private:
    SharedPtr<ModelCacheSpace> m_model;
};

class QI_EXPORT ControllerMouseCacheSpace: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseCacheSpace)

public:
    ControllerMouseCacheSpace(SharedPtr<ModelCacheSpace> model);

private:
    void tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo) override;

    SharedPtr<ModelCacheSpace> m_model;
};

} // end namespace Qi

#endif // QI_VIEW_CACHE_H
