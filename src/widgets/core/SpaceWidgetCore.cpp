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

#include "SpaceWidgetCore.h"
#include "cache/space/CacheSpace.h"
#include "cache/CacheControllerMouse.h"
#include "core/ControllerKeyboard.h"
#include "utils/PainterState.h"

#include <QWidget>
#include <QToolTip>

namespace Qi
{

SpaceWidgetCore::SpaceWidgetCore(QWidget* owner)
    : m_owner(owner),
      m_guiContext(owner)
{
    Q_ASSERT(m_owner);

#if !defined(QT_NO_DEBUG)
    m_trackOwner = m_owner;
#endif
}

SpaceWidgetCore::~SpaceWidgetCore()
{
#if !defined(QT_NO_DEBUG)
    // owner should exist
    Q_ASSERT(m_trackOwner);
#endif

    QObject::disconnect(m_connection);
}

const Space& SpaceWidgetCore::mainSpace() const
{
    return m_mainCacheSpace->space();
}

Space& SpaceWidgetCore::rMainSpace()
{
    return m_mainCacheSpace->rSpace();
}

bool SpaceWidgetCore::initSpaceWidgetCore(const QSharedPointer<CacheSpace>& mainCacheSpace)
{
    if (!m_mainCacheSpace.isNull())
    {
        // one time initialization allowed
        Q_ASSERT(false);
        return false;
    }

    Q_ASSERT(mainCacheSpace);
    m_mainCacheSpace = mainCacheSpace;
    m_cacheControllers.reset(new CacheControllerMouse(m_owner, this, m_mainCacheSpace));

    m_connection = QObject::connect(m_mainCacheSpace.data(), &CacheSpace::cacheChanged, [this](const CacheSpace* cache, ChangeReason reason) {
        onCacheSpaceChanged(cache, reason);
    });

    // enable tracking mouse moves
    m_owner->setMouseTracking(true);

    return true;
}

void SpaceWidgetCore::setControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard)
{
    m_controllerKeyboard = controllerKeyboard;
}

void SpaceWidgetCore::addControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard)
{
    if (m_controllerKeyboard.isNull())
        m_controllerKeyboard = controllerKeyboard;
    else
    {
        auto newController = QSharedPointer<ControllerKeyboardChain>::create();
        newController->append(m_controllerKeyboard);
        newController->append(controllerKeyboard);
        m_controllerKeyboard = newController;
    }
}

void SpaceWidgetCore::ensureVisible(const ItemID& visibleItem, const CacheSpace* cacheSpace, bool validateItem)
{
    ensureVisibleImpl(visibleItem, cacheSpace, validateItem);
}

bool SpaceWidgetCore::processOwnerEvent(QEvent* event)
{
    Q_ASSERT(m_mainCacheSpace);
    if (!m_mainCacheSpace)
        return false;

    bool processed = true;

    switch (event->type())
    {
    case QEvent::Resize:
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        // resize window of the cache to occupy whole widget rect
        m_mainCacheSpace->setWindow(QRect(QPoint(0, 0), resizeEvent->size()));
    } break;

    case QEvent::Paint:
    {
        QPainter painter(m_owner);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
        painter.setBackgroundMode(Qt::TransparentMode);
        // draw cache
        m_mainCacheSpace->draw(&painter, GuiContext(m_owner));
    } break;

    case QEvent::ToolTip:
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent *>(event);

        // show tooltip from the cache
        TooltipInfo tooltipInfo;
        if (m_mainCacheSpace->tooltipByPoint(helpEvent->pos(), tooltipInfo))
            QToolTip::showText(helpEvent->globalPos(), tooltipInfo.text, m_owner, tooltipInfo.rect);
        else
            QToolTip::hideText();
    } break;

    case QEvent::FocusIn:
    {
        if (m_controllerKeyboard)
            m_controllerKeyboard->startCapturing();
        // repaint owner
        m_owner->update();
    } break;

    case QEvent::FocusOut:
    {
        if (m_controllerKeyboard)
            m_controllerKeyboard->stopCapturing();
        // repaint owner
        m_owner->update();
    } break;

    case QEvent::KeyPress:
    {
        if (!m_cacheControllers->isCapturing() && m_controllerKeyboard)
            m_controllerKeyboard->processKeyPress(static_cast<QKeyEvent*>(event));
    } break;

    case QEvent::KeyRelease:
    {
        if (!m_cacheControllers->isCapturing() && m_controllerKeyboard)
            m_controllerKeyboard->processKeyRelease(static_cast<QKeyEvent*>(event));
    } break;

    default:
        processed = false;
        break;
    }

    // process event by controllers
    return m_cacheControllers->processEvent(event) || processed;
}

bool SpaceWidgetCore::doInplaceEdit(const ItemID& visibleItem, const CacheSpace* cacheSpace, const QKeyEvent* event)
{
    Q_ASSERT(m_cacheControllers);
    if (m_cacheControllers.isNull())
        return false;

    return m_cacheControllers->doInplaceEdit(*cacheSpace, visibleItem, event, nullptr);
}

void SpaceWidgetCore::stopControllers()
{
    Q_ASSERT(m_cacheControllers);
    if (!m_cacheControllers.isNull())
        m_cacheControllers->stop();
}

void SpaceWidgetCore::resumeControllers()
{
    Q_ASSERT(m_cacheControllers);
    if (!m_cacheControllers.isNull())
        m_cacheControllers->resume();
}

void SpaceWidgetCore::onCacheSpaceChanged(const CacheSpace* cache, ChangeReason /*reason*/)
{
    Q_UNUSED(cache);
    Q_ASSERT(m_mainCacheSpace.data() == cache);
    // repaint owner widget
    m_owner->update();
}

QPixmap SpaceWidgetCore::createPixmapImpl() const
{
    QPixmap image(m_mainCacheSpace->window().size());
    image.fill(m_owner->palette().color(m_owner->backgroundRole()));

    {
        QPainter painter(&image);
        copyPainterState(m_owner, &painter);
        painter.setWindow(m_mainCacheSpace->window());

        m_mainCacheSpace->drawRaw(&painter, guiContext());
    }

    return image;
}

} // end namespace Qi
