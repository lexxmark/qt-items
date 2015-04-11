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

#ifndef QI_CACHE_CONTROLLER_MOUSE_H
#define QI_CACHE_CONTROLLER_MOUSE_H

#include "core/ControllerMouse.h"

namespace Qi
{

class SpaceWidgetCore;
class CacheSpace;
class View;
struct TooltipInfo;

class QI_EXPORT CacheControllerMouse: public ControllerContext
{
public:
    CacheControllerMouse(QWidget* owner, SpaceWidgetCore* widgetCore, const QSharedPointer<CacheSpace>& cacheSpace);
    virtual ~CacheControllerMouse();

    void addCacheSpace(const QSharedPointer<CacheSpace>& cacheSpace);

    bool isBusy() const { return m_isBusy; }

    void updatePosition(const QPoint& point);
    void stopCapturing();
    bool isCapturing() const { return m_capturingController; }

    void stop();
    void resume();
    bool isStopped() const { return m_isStopped; }

    void clear();

    bool processEvent(QEvent* event);
    void processPaint(QPaintEvent* event);
    void processKillFocus(QFocusEvent* event);
    bool processLButtonDown(QMouseEvent* event);
    bool processLButtonUp(QMouseEvent* event);
    bool processLButtonDblClick(QMouseEvent* event);
    bool processMouseMove(QMouseEvent* event);
    bool processContextMenu(QContextMenuEvent* event);

    bool doInplaceEdit(const CacheSpace& cacheSpace, const ItemID& visibleItem, const QKeyEvent* keyEvent, const View* view);

    bool tooltipByPoint(const QPoint& point, TooltipInfo& tooltipInfo) const;

private:
    //
    void notifyStartCapturing(ControllerMouse& controller) override;
    void notifyStopCapturing(ControllerMouse& controller) override;

    bool isEmpty() const { return m_activeControllers.empty(); }
    void updateActiveControllers();

    template <typename Func> bool processFunc(Func func)
    {
        if (isStopped())
            return false;

        if (m_capturingController)
            return (m_capturingController->*func)();
        else
        {
            // enumerate controllers from high to low
            for (auto controller: m_activeControllers)
            {
                // controller should be valid
                Q_ASSERT(!m_activeControllers.isEmpty());

                if ((controller->*func)())
                    return true;
            }
            return false;
        }
    }

    template <typename Func, typename Arg> bool processFunc(Func func, Arg arg)
    {
        if (isStopped())
            return false;

        if (m_capturingController)
            return (m_capturingController->*func)(arg);
        else
        {
            // enumerate controllers from high to low
            for (auto controller: m_activeControllers)
            {
                // controller should be valid
                Q_ASSERT(!m_activeControllers.empty());

                if ((controller->*func)(arg))
                    return true;
            }
            return false;
        }
    }

    QVector<QSharedPointer<CacheSpace>> m_cacheSpaces;
    QVector<ControllerMouse*> m_activeControllers;
    ControllerMouse* m_capturingController;
    bool m_isStopped;
    bool m_isBusy;
};

} // end namespace Qi

#endif // QI_CACHE_CONTROLLER_MOUSE_H
