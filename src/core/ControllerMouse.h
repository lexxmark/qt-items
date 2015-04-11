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

#ifndef QI_CONTROLLER_MOUSE_H
#define QI_CONTROLLER_MOUSE_H

#include "core/misc/ControllerMouseAuxiliary.h"
#include "ItemID.h"
#include <QObject>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QFocusEvent>

namespace Qi
{

class CacheContext;
class CacheSpace;

class QI_EXPORT ControllerMouse: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouse)

public:
    virtual ~ControllerMouse();

    ControllerMousePriority priority() const { return m_priority; }
    bool isCapturing() const { return m_state == ControllerMouseStateCapturing; }
    bool isActive() const { return m_state != ControllerMouseStateInactive; }
    ItemID activeItem() const;

    void tryActivate(QVector<ControllerMouse*>& activatedControllers, const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace);
    void deactivate();
    void stopCapturing();

    virtual void processPaint(QPaintEvent* /*event*/) {}
    virtual void processKillFocus(QFocusEvent* /*event*/) {}
    virtual bool processLButtonDown(QMouseEvent* /*event*/) { return false; }
    virtual bool processLButtonUp(QMouseEvent* /*event*/) { return false; }
    virtual bool processLButtonDblClick(QMouseEvent* /*event*/) { return false; }
    virtual bool processMouseMove(QMouseEvent* /*event*/) { return false; }
    virtual bool processContextMenu(QContextMenuEvent* /*event*/) { return false; }

    bool acceptInplaceEdit(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent = nullptr) const;
    void doInplaceEdit(QVector<ControllerMouse*>& activatedControllers, const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent = nullptr);
    void updateInplaceEditLayout();

protected:
    ControllerMouse(ControllerMousePriority priority = ControllerMousePriorityNormal);

    enum ControllerMouseState
    {
        ControllerMouseStateInactive,
        ControllerMouseStateActive,
        ControllerMouseStateDoubleActive,
        ControllerMouseStateCapturing
    };
    ControllerMouseState state() const { return m_state; }

    class QI_EXPORT ActivationInfo
    {
        Q_DISABLE_COPY(ActivationInfo)

    public:
        ActivationInfo(const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace);

        const ItemID& item() const;
        const QPoint& point() const { return context.point; }

        const ControllerContext& context;
        const CacheContext& cache;
        const CacheSpace& cacheSpace;
    };

    class QI_EXPORT ActivationState
    {
        Q_DISABLE_COPY(ActivationState)

    public:
        explicit ActivationState(const ActivationInfo& info);

        const QPoint& point() const { return context.point; }
        ItemID visibleItem() const;

        const ControllerContext& context;
        const ItemID item;
        const CacheSpace& cacheSpace;

        // these members can be out of sync
        // for example when grid "auto-scrolls" and user drags mouse
        // near the grid edge
        const QRect itemRect;
        const QRect viewRect;
        const QRect* visibleRect;
    };

    void startCapturing();
    const ActivationState& activationState() const;

    virtual void tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo);
    virtual bool acceptImpl(const ActivationInfo& /*activationInfo*/) const { return true; }
    virtual void activateImpl(const ActivationInfo& activationInfo);
    virtual bool needDeactivateImpl(const ActivationInfo& activationInfo) const;
    virtual void deactivateImpl();
    virtual void startCapturingImpl();
    virtual void stopCapturingImpl();

    virtual bool acceptInplaceEditImpl(const ItemID& /*item*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* /*keyEvent*/) const { return false; }
    virtual void doInplaceEditImpl(const QKeyEvent* /*keyEvent*/) {}
    virtual void updateInplaceEditLayoutImpl() {}

private:
    void activate(const ActivationInfo& activationInfo);

    ControllerMousePriority m_priority;
    ControllerMouseState m_state;
    QScopedPointer<ActivationState> m_activationState;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_H
