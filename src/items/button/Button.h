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

#ifndef QI_BUTTON_H
#define QI_BUTTON_H

#include "core/ext/ViewComposite.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

class QI_EXPORT ViewButton: public ViewComposite
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewButton)

public:
    ViewButton(const QSharedPointer<View>& bttnContent, ViewDefaultController createDefaultController = ViewDefaultControllerCreate);

    std::function<void (const ItemID& item, QStyle::State& bttnState)> tuneBttnState;
    std::function<void (const ItemID& item, const ControllerContext& context, const ViewButton* viewBttn)> action;

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    PushableTracker m_pushableTracker;
};

} // end namespace Qi

#endif // QI_BUTTON_H
