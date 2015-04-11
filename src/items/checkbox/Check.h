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

#ifndef QI_CHECK_H
#define QI_CHECK_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"
#include "core/ext/ModelCallback.h"

namespace Qi
{

typedef ModelTyped<Qt::CheckState> ModelCheck;
typedef ModelCallback<Qt::CheckState, false> ModelCheckCallback;

class QI_EXPORT ViewCheck: public ViewModeled<ModelCheck>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCheck)

public:
    ViewCheck(const QSharedPointer<ModelCheck>& model, bool useDefaultController = true);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    QStyle::State styleState(const ItemID& item) const;

    PushableTracker m_pushableTracker;
};

QI_EXPORT QSharedPointer<ControllerMousePushable> createControllerMouseCheck(const QSharedPointer<ModelCheck>& model);

} // end namespace Qi

#endif // QI_CHECK_H
