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

#ifndef QI_CONTROLLER_MOUSE_PUSHABLE_CALLBACK_H
#define QI_CONTROLLER_MOUSE_PUSHABLE_CALLBACK_H

#include "core/ext/ControllerMousePushable.h"
#include <functional>

namespace Qi
{

// emits onApply when user clicks or presses Space key
class QI_EXPORT ControllerMousePushableCallback: public ControllerMousePushable
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMousePushableCallback)

public:
    ControllerMousePushableCallback(ControllerMousePriority priority = ControllerMousePriorityNormal, bool processDblClick = true);

    std::function<void (const ItemID& item, const ControllerContext& context)> onApply;

protected:
    void applyImpl() override;
    bool acceptInplaceEditImpl(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent) const override;
    void doInplaceEditImpl(const QKeyEvent* keyEvent) override;

private:
    void emitOnApply();
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_PUSHABLE_CALLBACK_H
