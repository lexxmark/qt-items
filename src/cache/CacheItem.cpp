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

#include "CacheItem.h"
#include "core/View.h"
#include "core/ControllerMouse.h"

//#define DEBUG_RECTS

namespace Qi
{

CacheItemInfo::CacheItemInfo(ID id)
    : id(id)
{
}

CacheItemInfo& CacheItemInfo::operator=(const CacheItemInfo& other)
{
    if (this == &other)
        return *this;

    id = other.id;
    rect = other.rect;
    schema = other.schema;
    return *this;
}

CacheItem::CacheItem(ID id)
    : CacheItemInfo(id),
      m_isCacheViewValid(false)
{
}

CacheItem::CacheItem(const CacheItemInfo& info)
    : CacheItemInfo(info),
      m_isCacheViewValid(false)
{
}

CacheItem::CacheItem(const CacheItem& other)
    : CacheItemInfo(other),
      m_cacheView(other.m_cacheView),
      m_isCacheViewValid(other.m_isCacheViewValid)
{
}

CacheItem& CacheItem::operator=(const CacheItem& other)
{
    CacheItemInfo::operator =(other);

    m_cacheView = other.m_cacheView;
    m_isCacheViewValid = other.m_isCacheViewValid;

    return *this;
}

const CacheView2* CacheItem::findCacheViewByController(const ControllerMouse* controller) const
{
    if (!m_isCacheViewValid || !m_cacheView)
        return nullptr;

    const CacheView2* result = nullptr;

    m_cacheView->forEachCacheView([&result, controller](const CacheView2* cacheView)->bool {
        if (cacheView->view()->controller().data() == controller)
        {
            result = cacheView;
            return false;
        }
        else
            return true;
    });

    return result;
}

void CacheItem::invalidateCacheView()
{
    m_cacheView.reset();
    m_isCacheViewValid = false;
}

void CacheItem::correctRectangles(const QPoint &offset)
{
    // offset cell rect
    rect.translate(offset);

    // nothing to correct any more
    if (!m_isCacheViewValid)
        return;

    // regenerate view rects if any flying view presents
//    if (m_isAnyFloatView)
//    {
//        invalidateCacheView();
//        return;
//    }

    // just offset all rects
    if (m_cacheView)
    {
        m_cacheView->forEachCacheView([&offset](CacheView2* cacheView)->bool {
            cacheView->rRect().translate(offset);
            return true;
        });
    }
}
QString CacheItem::text() const
{
    QString text;

    if (schema.view)
        schema.view->text(id, text);

    return text;
}

void CacheItem::draw(QPainter *painter, const GuiContext& ctx, const QRect* visibleRect)
{
    if (drawProxy)
        drawProxy(this, painter, ctx, visibleRect);
    else
        drawRaw(painter, ctx, visibleRect);
}

void CacheItem::drawRaw(QPainter *painter, const GuiContext& ctx, const QRect* visibleRect)
{
    validateCacheView(ctx, visibleRect);

    if (!m_cacheView)
        return;

    //*ctx.PreDrawCell(m_rect);

    m_cacheView->draw(painter, ctx, id, rect, visibleRect);
    m_cacheView->cleanupDraw(painter, ctx, id, rect, visibleRect);

    //*ctx.PostDrawCell();
}

void CacheItem::tryActivateControllers(const ControllerContext& context, const CacheSpace& cacheSpace, const QRect* visibleRect, QVector<ControllerMouse*>& controllers) const
{
    // don't handle if CacheCellEx is not ready yet
    if (!m_isCacheViewValid || !m_cacheView)
        return;

    typedef QPair<ControllerMouse*, const CacheView2*> ControllerInfo_t;
    QVector<ControllerInfo_t> itemControllersInfo;

    // collect affected controllers
    m_cacheView->forEachCacheView([&itemControllersInfo, &context](const CacheView2* cacheView)->bool {
        if (!cacheView->view()->controller())
            return true;

        if (!cacheView->rect().contains(context.point))
            return true;

        itemControllersInfo.push_back(ControllerInfo_t(cacheView->view()->controller().data(), cacheView));

        return true;
    });

    if (itemControllersInfo.isEmpty())
        return;

    // range controllers by priority from low to high
    std::stable_sort(itemControllersInfo.begin(), itemControllersInfo.end(), [](const ControllerInfo_t& left, const ControllerInfo_t& right) {
        return left.first->priority() < right.first->priority();
    });

    // m_cacheView should exists at this time
    Q_ASSERT(m_cacheView);

    // activate controllers in reversed order
    for (int i = itemControllersInfo.size() - 1; i >= 0; --i)
    {
        itemControllersInfo[i].first->tryActivate(controllers, context, CacheContext(id, rect, *(itemControllersInfo[i].second), visibleRect), cacheSpace);

        // if CacheCellEx was invalidated during TryActivate -> stop activate controllers
        if (!m_cacheView)
        {
            qDebug("TryActivateControllers break\n");
            break;
        }
    }
}

bool CacheItem::tooltipByPoint(const QPoint& point, TooltipInfo &tooltipInfo) const
{
    // don't handle if CacheCellEx is not ready yet
    if (!m_isCacheViewValid || !m_cacheView)
        return false;

    bool success = false;
    m_cacheView->forEachCacheView([&success, &point, &tooltipInfo, this](const CacheView2* cacheView)->bool {
        // skip views not under the point
        if (!cacheView->rect().contains(point))
            return true;

        if (cacheView->tooltipText(id, tooltipInfo.text))
        {
            // save view rect
            tooltipInfo.rect = cacheView->rect();
            // mark success flag
            success = true;
            // stop searching
            return false;
        }
        else
        {
            if (cacheView->view()->tooltipByPoint(point, id, tooltipInfo))
            {
                success = true;
                // stop searching
                return false;
            }
        }

        return true;
    });

    return success;
}

void CacheItem::validateCacheView(const GuiContext& ctx, const QRect* visibleRect)
{
    if (m_isCacheViewValid)
        return;

    Q_ASSERT(!m_cacheView);

    QRect* visibleItemRectPtr = nullptr;

    QRect visibleItemRect;
    if (visibleRect)
    {
        visibleItemRect = rect.intersected(*visibleRect);
        if (visibleItemRect.isValid() && visibleItemRect != rect)
        {
            // only apply if visibleCellRect partly intersects cellRect
            visibleItemRectPtr = &visibleItemRect;
        }
    }

    if (schema.isValid())
    {
        QRect itemRect = rect;
        QVector<CacheView2> cacheViews;
        CacheView2* cacheView = schema.view->addCacheView(*schema.layout, ctx, id, cacheViews, itemRect, visibleItemRectPtr);
        if (cacheView)
        {
            m_cacheView.reset(new CacheView2(*cacheView));
        }
    }

    // mark cache views as valid
    m_isCacheViewValid = true;
}

QSize CacheItem::calculateItemSize(const GuiContext& ctx, ViewSizeMode sizeMode) const
{
    if (schema.isValid())
        return schema.view->size(ctx, id, sizeMode);

    return QSize(0, 0);
}

} // end namespace Qi
