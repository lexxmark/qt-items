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

#include "CacheControllerMouse.h"
#include "cache/CacheItemFactory.h"
#include "cache/space/CacheSpace.h"
#include "widgets/core/SpaceWidgetCore.h"
#include "utils/auto_value.h"
#include <QDebug>

namespace Qi
{

CacheControllerMouse::CacheControllerMouse(QWidget* owner, SpaceWidgetCore* widgetCore, const QSharedPointer<CacheSpace> &cacheSpace)
    : ControllerContext(owner, widgetCore),
      m_cacheSpaces(1, cacheSpace),
      m_capturingController(nullptr),
      m_isStopped(false),
      m_isBusy(false)
{
}

CacheControllerMouse::~CacheControllerMouse()
{
    clear();

    Q_ASSERT(!m_capturingController);
    Q_ASSERT(m_activeControllers.isEmpty());
}

void CacheControllerMouse::addCacheSpace(const QSharedPointer<CacheSpace> &cacheSpace)
{
    clear();
    m_cacheSpaces.append(cacheSpace);
}

void CacheControllerMouse::stop()
{
    m_isStopped = true;
    clear();
}

void CacheControllerMouse::resume()
{
    m_isStopped = false;

    if (m_capturingController)
        m_capturingController->updateInplaceEditLayout();
}

void CacheControllerMouse::clear()
{
    QVector<ControllerMouse*> activeControllers;
    activeControllers.swap(m_activeControllers);

    for (auto controller: activeControllers)
    {
        controller->deactivate();
    }

    // capturing controller should stop capturing when deactivate all controllers
    Q_ASSERT(!m_capturingController);
}

bool CacheControllerMouse::doInplaceEdit(const CacheSpace& cacheSpace, const ItemID& visibleItem, const QKeyEvent* keyEvent, const View* view)
{
    bool needEnsureVisible = false;

    QScopedPointer<CacheItem> newItem;
    // look up cacheItem in existing cache grid
    CacheItem* cacheItem = const_cast<CacheItem*>(cacheSpace.cacheItem(visibleItem));
    if (!cacheItem)
    {
        // create temporary cache item
        needEnsureVisible = true;
        const auto& cacheItemFactory = cacheSpace.cacheItemFactory();
        newItem.reset(new CacheItem(cacheItemFactory.create(visibleItem)));
        cacheItem = newItem.data();
    }

    QVector<const View*> itemViews;
    if (cacheItem->schema.view)
        cacheItem->schema.view->addView(cacheItem->item, itemViews);

    // search for controller acceptable for edit
    for (auto itemView: itemViews)
    {
        if (view && (view != itemView))
            continue;

        const auto& controller = itemView->controller();
        if (controller)
        {
            if (!controller->acceptInplaceEdit(cacheItem->item, cacheSpace, keyEvent))
                continue;

            if (needEnsureVisible)
            {
                widgetCore->ensureVisible(visibleItem, &cacheSpace, true);
                // get cache item from existing cache grid
                cacheItem = const_cast<CacheItem*>(cacheSpace.cacheItem(visibleItem));
                Q_ASSERT(cacheItem);
                if (!cacheItem)
                    return false;
            }

            // deactivate all controllers
            clear();

            // capturing controller should stop capturing when deactivate all controllers
            Q_ASSERT(!m_capturingController);
            Q_ASSERT(m_activeControllers.isEmpty());

            // cacheItem should be prepared
            Q_ASSERT(cacheItem->isCacheViewValid());
            const CacheView* cacheView = cacheItem->findCacheViewByController(controller.data());
            Q_ASSERT(cacheView);
            if (!cacheView)
                return false;

            CacheContext cache(cacheItem->item, cacheItem->rect, *cacheView, &cacheSpace.window());
            // start edit
            controller->doInplaceEdit(m_activeControllers, *this, cache, cacheSpace, keyEvent);
            return true;
        }
    }

    return false;
}

bool CacheControllerMouse::tooltipByPoint(const QPoint& point, TooltipInfo& tooltipInfo) const
{
    for (auto cacheSpace: m_cacheSpaces)
    {
        if (cacheSpace->tooltipByPoint(point, tooltipInfo))
            return true;
    }

    return false;
}

void CacheControllerMouse::updatePosition(const QPoint& point)
{
    if (isStopped())
        return;

    this->point = point;

    if (!m_capturingController)
        updateActiveControllers();
}

void CacheControllerMouse::updateActiveControllers()
{
    Q_ASSERT(!m_capturingController);
    Q_ASSERT(!m_isBusy);

    auto_value<bool> isBusy(m_isBusy, true);

    // try to activate controllers under the point
    // and get actually activated controllers
    QVector<ControllerMouse*> activatedControllers;
    for (auto cacheSpace: m_cacheSpaces)
    {
        cacheSpace->tryActivateControllers(*this, activatedControllers);
    }

    // range controllers by priority from high to low
    std::stable_sort(activatedControllers.begin(), activatedControllers.end(), [](const ControllerMouse* left, const ControllerMouse* right) {
        return left->priority() > right->priority();
    });

    // deactivate previously activated controllers
    for (int i = m_activeControllers.size() - 1; i >= 0; --i)
        m_activeControllers[i]->deactivate();

    m_activeControllers.swap(activatedControllers);
}

void CacheControllerMouse::stopCapturing()
{
    if (m_capturingController)
    {
        m_capturingController->stopCapturing();
        Q_ASSERT(!m_capturingController);
    }
}

void CacheControllerMouse::notifyStartCapturing(ControllerMouse& controller)
{
    Q_ASSERT(controller.isCapturing());
    Q_ASSERT(!m_capturingController);
    Q_ASSERT(std::find(m_activeControllers.begin(), m_activeControllers.end(), &controller) != m_activeControllers.end());

    if (m_capturingController)
        return;

    m_capturingController = &controller;
}

void CacheControllerMouse::notifyStopCapturing(ControllerMouse& controller)
{
    Q_UNUSED(controller);
    Q_ASSERT(m_capturingController == &controller);
    Q_ASSERT(!controller.isCapturing());

    if (!m_capturingController)
        return;

    m_capturingController = nullptr;
}

bool CacheControllerMouse::processEvent(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::Paint:
        {
            processPaint(static_cast<QPaintEvent*>(event));
            break;
        }

        case QEvent::FocusOut:
        {
            QFocusEvent* focusEvent = static_cast<QFocusEvent*>(event);
            processKillFocus(focusEvent);
            break;
        }

        case QEvent::Leave:
        {
            //qDebug() << "Leave event";
            // clear controllers if non active
            if (!isCapturing())
                clear();
            break;
        }

        case QEvent::MouseButtonPress:
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                processLButtonDown(mouseEvent);
            break;
        }

        case QEvent::MouseButtonRelease:
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                processLButtonUp(mouseEvent);
            break;
        }

        case QEvent::MouseButtonDblClick:
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                processLButtonDblClick(mouseEvent);
            break;
        }

        case QEvent::MouseMove:
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            processMouseMove(mouseEvent);
            break;
        }

        case QEvent::ContextMenu:
        {
            QContextMenuEvent* contextEvent = static_cast<QContextMenuEvent*>(event);
            processContextMenu(contextEvent);
            break;
        }

        default:
            break;
    }

    return false;
}

void CacheControllerMouse::processPaint(QPaintEvent* event)
{
    // only capturing controller paints
    if (m_capturingController)
        m_capturingController->processPaint(event);
}

void CacheControllerMouse::processKillFocus(QFocusEvent* event)
{
    if (isStopped())
        return;

    if (m_capturingController)
    {
        m_capturingController->processKillFocus(event);
    }
    else
    {
        // enumerate controllers from high to low
        for (auto controller: m_activeControllers)
        {
            // controller should be valid
            Q_ASSERT(!m_activeControllers.isEmpty());
            controller->processKillFocus(event);
        }
    }
}

bool CacheControllerMouse::processLButtonDown(QMouseEvent* event)
{
    updatePosition(event->pos());
    return processFunc(&ControllerMouse::processLButtonDown, event);
}

bool CacheControllerMouse::processLButtonUp(QMouseEvent* event)
{
    updatePosition(event->pos());
    return processFunc(&ControllerMouse::processLButtonUp, event);
}

bool CacheControllerMouse::processLButtonDblClick(QMouseEvent* event)
{
    updatePosition(event->pos());
    return processFunc(&ControllerMouse::processLButtonDblClick, event);
}

bool CacheControllerMouse::processMouseMove(QMouseEvent* event)
{
    updatePosition(event->pos());
    return processFunc(&ControllerMouse::processMouseMove, event);
}

bool CacheControllerMouse::processContextMenu(QContextMenuEvent* event)
{
    updatePosition(event->pos());
    return processFunc(&ControllerMouse::processContextMenu, event);
}

} // end namespace Qi
