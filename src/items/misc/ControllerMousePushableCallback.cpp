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

#include "ControllerMousePushableCallback.h"

namespace Qi
{

ControllerMousePushableCallback::ControllerMousePushableCallback(ControllerMousePriority priority, bool processDblClick)
    : ControllerMousePushable(priority, processDblClick)
{
}

void ControllerMousePushableCallback::applyImpl()
{
    emitOnApply();
}

bool ControllerMousePushableCallback::acceptInplaceEditImpl(const ItemID& /*item*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* keyEvent) const
{
    return keyEvent && (keyEvent->type() == QEvent::KeyPress) && (keyEvent->key() == Qt::Key_Space);
}

void ControllerMousePushableCallback::doInplaceEditImpl(const QKeyEvent* keyEvent)
{
    Q_UNUSED(keyEvent);
    Q_ASSERT(keyEvent);
    Q_ASSERT(keyEvent->type() == QEvent::KeyPress);
    Q_ASSERT(keyEvent->key() == Qt::Key_Space);

    emitOnApply();
}

void ControllerMousePushableCallback::emitOnApply()
{
    Q_ASSERT(isActive());
    if (!onApply)
        return;

    onApply(activeItem(), activationState().context);
}

} // end namespace Qi
