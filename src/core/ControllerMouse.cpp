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

#include "ControllerMouse.h"
#include "cache/CacheView.h"
#include "space/CacheSpace.h"
#include <QDebug>

namespace Qi
{

ControllerMouse::ActivationInfo::ActivationInfo(const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace)
    : context(context),
      cache(cache),
      cacheSpace(cacheSpace)
{
}

ID ControllerMouse::ActivationInfo::id() const
{
    return cache.id;
}

ControllerMouse::ActivationState::ActivationState(const ControllerMouse::ActivationInfo& info)
    : context(info.context),
      id(info.cache.id),
      cacheSpace(info.cacheSpace),
      itemRect(info.cache.itemRect),
      viewRect(info.cache.cacheView.rect()),
      visibleRect(info.cache.visibleRect)
{
}

ID ControllerMouse::ActivationState::visibleId() const
{
    return cacheSpace.space().toVisible(id);
}

ControllerMouse::ControllerMouse(ControllerMousePriority priority)
    : m_priority(priority),
      m_state(ControllerMouseStateInactive)
{
}

ControllerMouse::~ControllerMouse()
{
    Q_ASSERT(m_state == ControllerMouseStateInactive);
}

const ID *ControllerMouse::activeId() const
{
    if (!isActive())
        return nullptr;
    else
        return &m_activationState->id;
}

const ControllerMouse::ActivationState& ControllerMouse::activationState() const
{
    Q_ASSERT(m_activationState.data());
    return *m_activationState.data();
}

void ControllerMouse::tryActivate(QVector<ControllerMouse*>& activatedControllers, const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace)
{
    tryActivateImpl(activatedControllers, ActivationInfo(context, cache, cacheSpace));
}

void ControllerMouse::tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo)
{
    if (acceptImpl(activationInfo))
    {
        activate(activationInfo);
        activatedControllers.append(this);
    }
}

void ControllerMouse::activate(const ActivationInfo& activationInfo)
{
    switch (m_state)
    {
        case ControllerMouseStateInactive:
        {
            activateImpl(activationInfo);
            Q_ASSERT(m_state == ControllerMouseStateActive);
            break;
        }

        case ControllerMouseStateActive:
        {
            if (needDeactivateImpl(activationInfo))
            {
                deactivate();
                activate(activationInfo);
                Q_ASSERT(m_state == ControllerMouseStateActive);
            }
            m_state = ControllerMouseStateDoubleActive;
            break;
        }

        default:
            // all other states are invalid here
            Q_ASSERT(false);
    }
}

void ControllerMouse::activateImpl(const ActivationInfo& activationInfo)
{
    Q_ASSERT(m_state == ControllerMouseStateInactive);
    m_activationState.reset(new ActivationState(activationInfo));
    m_state = ControllerMouseStateActive;
}

void ControllerMouse::deactivate()
{
    switch (m_state)
    {
        case ControllerMouseStateActive:
        {
            deactivateImpl();
            Q_ASSERT(m_state == ControllerMouseStateInactive);
            break;
        }

        case ControllerMouseStateDoubleActive:
        {
            m_state = ControllerMouseStateActive;
            break;
        }

        case ControllerMouseStateCapturing:
        {
            stopCapturing();
            Q_ASSERT(m_state == ControllerMouseStateActive);
            deactivateImpl();
            Q_ASSERT(m_state == ControllerMouseStateInactive);
            break;
        }

        default:
            // all other states are invalid here
            Q_ASSERT(false);
    }
}

void ControllerMouse::deactivateImpl()
{
    // don't delete activation state
    // it should be valid if user opens
    // modal dialog during controller message handling
    //m_activationState.reset();
    m_state = ControllerMouseStateInactive;
}

bool ControllerMouse::needDeactivateImpl(const ActivationInfo& activationInfo) const
{
    Q_ASSERT(m_activationState.data());

    // should deactivate if contoller moved to another item
    if (m_activationState->id != activationInfo.cache.id)
        return true;

    // or view's rect has changed
    if (m_activationState->viewRect != activationInfo.cache.cacheView.rect())
        return true;

    return false;
}

void ControllerMouse::startCapturing()
{
    Q_ASSERT(m_state == ControllerMouseStateActive);
    startCapturingImpl();
    Q_ASSERT(m_state == ControllerMouseStateCapturing);
    Q_ASSERT(m_activationState.data());
    ControllerContext& context = const_cast<ControllerContext&>(m_activationState->context);
    context.notifyStartCapturing(*this);
}

void ControllerMouse::startCapturingImpl()
{
    Q_ASSERT(m_state == ControllerMouseStateActive);
    m_state = ControllerMouseStateCapturing;
}

void ControllerMouse::stopCapturing()
{
    stopCapturingImpl();
    Q_ASSERT(m_state == ControllerMouseStateActive);
    Q_ASSERT(m_activationState.data());
    ControllerContext& context = const_cast<ControllerContext&>(m_activationState->context);
    context.notifyStopCapturing(*this);
}

void ControllerMouse::stopCapturingImpl()
{
    Q_ASSERT(m_state == ControllerMouseStateCapturing);
    m_state = ControllerMouseStateActive;
}

bool ControllerMouse::acceptInplaceEdit(ID id, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent) const
{
    return acceptInplaceEditImpl(id, cacheSpace, keyEvent);
}

void ControllerMouse::doInplaceEdit(QVector<ControllerMouse*>& activatedControllers, const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent)
{
    Q_ASSERT(m_state == ControllerMouseStateInactive);
    if (isCapturing())
        return;

    ActivationInfo activationInfo(context, cache, cacheSpace);
    activate(activationInfo);
    activatedControllers.append(this);

    doInplaceEditImpl(keyEvent);
}

void ControllerMouse::updateInplaceEditLayout()
{
    updateInplaceEditLayoutImpl();
}

} // end namespace Qi
