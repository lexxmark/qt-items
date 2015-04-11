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

#ifndef QI_LINK_H
#define QI_LINK_H

#include "items/text/Text.h"
#include "items/misc/ControllerMousePushableCallback.h"

namespace Qi
{

typedef ModelTyped<QUrl> ModelUrl;

class QI_EXPORT ViewLink: public ViewText
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewLink)

public:
    ViewLink(const QSharedPointer<ModelText>& model, ViewDefaultController createDefaultController = ViewDefaultControllerCreate, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone);
    ViewLink(const QSharedPointer<ModelText>& model, const QSharedPointer<ModelUrl>& modelUrl, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone);

    std::function<void (const ItemID& item, const ControllerContext& context, const ViewLink* viewLink)> action;

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    PushableTracker m_pushableTracker;
};

class ControllerMouseLink: public ControllerMousePushableCallback
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseLink)

public:
    ControllerMouseLink(ControllerMousePriority priority = ControllerMousePriorityNormal, bool processDblClick = true);

protected:
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;
};

} // end namespace Qi

#endif // QI_LINK_H
