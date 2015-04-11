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

#include "SpaceWidgetScrollAbstract.h"
#include "cache/space/CacheSpace.h"
#include "cache/CacheItem.h"

#include <QScrollBar>
#include <QKeyEvent>

namespace Qi
{

SpaceWidgetScrollAbstract::SpaceWidgetScrollAbstract(QWidget* parent)
    : QAbstractScrollArea(parent),
      SpaceWidgetCore(viewport()),
      m_isCacheItemsLayoutValid(false)
{
    // enable tracking mouse moves
    //viewport()->setMouseTracking(true);
    // enable focus
    setFocusPolicy(Qt::StrongFocus);
}

SpaceWidgetScrollAbstract::~SpaceWidgetScrollAbstract()
{
}

bool SpaceWidgetScrollAbstract::initSpaceWidgetScrollable(const QSharedPointer<CacheSpace>& mainCacheSpace, const QSharedPointer<CacheSpace>& scrollableCacheSpace)
{
    // one time initialization allowed
    if(!m_scrollableCacheSpace.isNull())
    {
        Q_ASSERT(false);
        return false;
    }

    Q_ASSERT(!scrollableCacheSpace.isNull());

    m_scrollableCacheSpace = scrollableCacheSpace;
    connect(m_scrollableCacheSpace.data(), &CacheSpace::cacheChanged, this, &SpaceWidgetScrollAbstract::onScrollCacheSpaceChanged);

    return initSpaceWidgetCore(mainCacheSpace);
}

void SpaceWidgetScrollAbstract::onScrollCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason)
{
    Q_UNUSED(cache);
    Q_ASSERT(cache == m_scrollableCacheSpace.data());

    if (reason & ChangeReasonSpaceStructure)
    {
        invalidateCacheItemsLayout();
        updateScrollbars();
        // request to recalculate layout
        updateGeometry();
    }
}

void SpaceWidgetScrollAbstract::ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem)
{
    if (cacheSpace != m_scrollableCacheSpace.data())
    {
        // should be implemented by descendant class
        Q_ASSERT(false);
        return;
    }

    // calculate scroll size and position if needed
    validateCacheItemsLayout();

    QSize visibleSize = viewport()->size();
    if (visibleSize.isEmpty())
        return;

    QPoint scrollPos(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QRect itemRect = cacheSpace->space().itemRect(visibleItem);

    if ((itemRect.width() >= visibleSize.width()) || (itemRect.left() < scrollPos.x()))
        scrollPos.rx() = itemRect.left();
    else if (itemRect.right() > (scrollPos.x() + visibleSize.width()))
        scrollPos.rx() = itemRect.right() - visibleSize.width();

    if ((itemRect.height() >= visibleSize.height()) || (itemRect.top() < scrollPos.y()))
        scrollPos.ry() = itemRect.top();
    else if (itemRect.bottom() > (scrollPos.y() + visibleSize.height()))
        scrollPos.ry() = itemRect.bottom() - visibleSize.height();

    // scroll to new position
    horizontalScrollBar()->setValue(scrollPos.x());
    verticalScrollBar()->setValue(scrollPos.y());

    if (validateItem)
    {
        validateCacheItemsLayout();

        // validate cache views
        CacheItem* cacheItem = const_cast<CacheItem*>(cacheSpace->cacheItem(visibleItem));
        if (!cacheItem)
            return;

        cacheItem->validateCacheView(guiContext(), &cacheSpace->window());
    }
}

bool SpaceWidgetScrollAbstract::viewportEvent(QEvent* event)
{
    bool result = QAbstractScrollArea::viewportEvent(event);

    switch (event->type())
    {
    case QEvent::Paint:
        validateCacheItemsLayout();
        break;

    case QEvent::Resize:
        updateScrollbars();
        invalidateCacheItemsLayout();
        break;

    default:
        ;
    }

    result |= processOwnerEvent(event);
    return result;
}

void SpaceWidgetScrollAbstract::keyPressEvent(QKeyEvent *event)
{
    //don't let QAbstractScrollArea to handle keyboard events
    processOwnerEvent(event);
}

void SpaceWidgetScrollAbstract::keyReleaseEvent(QKeyEvent *event)
{
    //don't let QAbstractScrollArea to handle keyboard events
    processOwnerEvent(event);
}

void SpaceWidgetScrollAbstract::focusInEvent(QFocusEvent * event)
{
    QAbstractScrollArea::focusInEvent(event);
    processOwnerEvent(event);
}

void SpaceWidgetScrollAbstract::focusOutEvent(QFocusEvent * event)
{
    QAbstractScrollArea::focusOutEvent(event);
    processOwnerEvent(event);
}

void SpaceWidgetScrollAbstract::scrollContentsBy(int dx, int dy)
{
    QAbstractScrollArea::scrollContentsBy(dx, dy);
    updateCacheScrollOffsetImpl();
}

QSize SpaceWidgetScrollAbstract::viewportSizeHint() const
{
    return calculateVirtualSizeImpl();
}

void SpaceWidgetScrollAbstract::updateScrollbars()
{
    // client visible area only
    QSize scrollableSize = calculateScrollableSizeImpl();

    // no space for scrollable area
    if (scrollableSize.width() <= 0 || scrollableSize.height() <= 0)
    {
        // don't show scrollbars
        verticalScrollBar()->setSingleStep(0);
        verticalScrollBar()->setPageStep(0);
        verticalScrollBar()->setRange(0, 0);
        horizontalScrollBar()->setSingleStep(0);
        horizontalScrollBar()->setPageStep(0);
        horizontalScrollBar()->setRange(0, 0);
    }
    else
    {
        QSize virtualSize = calculateVirtualSizeImpl();
        // update vertical scrollbar
        verticalScrollBar()->setSingleStep(scrollableSize.height() / 10);
        verticalScrollBar()->setPageStep(scrollableSize.height());
        verticalScrollBar()->setRange(0, qMax(0, virtualSize.height() - scrollableSize.height() + 2));
        // update horizontal scrollbar
        horizontalScrollBar()->setSingleStep(scrollableSize.width() / 10);
        horizontalScrollBar()->setPageStep(scrollableSize.width());
        horizontalScrollBar()->setRange(0, qMax(0, virtualSize.width() - scrollableSize.width() + 2));
    }
}

QSize SpaceWidgetScrollAbstract::calculateVirtualSizeImpl() const
{
    Q_ASSERT(!m_scrollableCacheSpace.isNull());
    if (m_scrollableCacheSpace.isNull())
        return QSize(0, 0);

    return m_scrollableCacheSpace->space().size();
}

QSize SpaceWidgetScrollAbstract::calculateScrollableSizeImpl() const
{
    return viewport()->size();
}

void SpaceWidgetScrollAbstract::updateCacheScrollOffsetImpl()
{
    // get scroll positions
    QPoint scrollPos(horizontalScrollBar()->value(), verticalScrollBar()->value());

    // update scrollable cache scroll position
    Q_ASSERT(!m_scrollableCacheSpace.isNull());
    if (m_scrollableCacheSpace.isNull())
        return;

    m_scrollableCacheSpace->setScrollOffset(scrollPos);
}

void SpaceWidgetScrollAbstract::validateCacheItemsLayoutImpl()
{
    rMainCacheSpace().setWindow(viewport()->rect());
}

void SpaceWidgetScrollAbstract::invalidateCacheItemsLayout()
{
    // stop handling controllers
    //stopControllers();

    m_isCacheItemsLayoutValid = false;
    // request to repaint
    update();
}

void SpaceWidgetScrollAbstract::validateCacheItemsLayout()
{
    if (m_isCacheItemsLayoutValid)
        return;

    validateCacheItemsLayoutImpl();
    updateCacheScrollOffsetImpl();

    m_isCacheItemsLayoutValid = true;

    // continue handling controllers
    resumeControllers();
}


} // end namespace Qi
