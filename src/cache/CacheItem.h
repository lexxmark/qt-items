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

#ifndef QI_CACHE_ITEM_H
#define QI_CACHE_ITEM_H

#include "CacheView.h"
#include "core/ItemSchema.h"

namespace Qi
{

class ControllerMouse;
class ControllerContext;
class CacheSpace;
struct TooltipInfo;

class QI_EXPORT CacheItemInfo
{
public:
    CacheItemInfo();
    CacheItemInfo(const CacheItemInfo&);
    CacheItemInfo& operator=(const CacheItemInfo& other);

    ItemID item;
    QRect rect;
    ViewSchema schema;
};

class QI_EXPORT CacheItem: public CacheItemInfo
{
public:
    CacheItem();
    explicit CacheItem(const CacheItemInfo& info);
    CacheItem(const CacheItem& other);
    CacheItem& operator=(const CacheItem& other);

    const CacheView* cacheView() const { return m_cacheView.data(); }
    CacheView* cacheView() { return m_cacheView.data(); }

    bool isCacheViewValid() const { return m_isCacheViewValid; }
    const CacheView* findCacheViewByController(const ControllerMouse* controller) const;

    void invalidateCacheView();
    void validateCacheView(const GuiContext& ctx, const QRect* visibleRect = nullptr);
    void correctRectangles(const QPoint& offset);

    QSize calculateItemSize(const GuiContext& ctx, ViewSizeMode sizeMode = ViewSizeModeExact) const;
    QString text() const;
    void tryActivateControllers(const ControllerContext& context, const CacheSpace& cacheSpace, const QRect* visibleRect, QVector<ControllerMouse*>& controllers) const;
    bool tooltipByPoint(const QPoint& point, TooltipInfo& tooltipInfo) const;

    std::function<void(CacheItem*, QPainter*, const GuiContext&, const QRect*)> drawProxy;

    void draw(QPainter* painter, const GuiContext& ctx, const QRect* visibleRect = nullptr);
    void drawRaw(QPainter* painter, const GuiContext& ctx, const QRect* visibleRect = nullptr);


private:
    QSharedPointer<CacheView> m_cacheView;
    bool m_isCacheViewValid;
    bool m_isAnyFloatView;
};

} // end namespace Qi

#endif // QI_CACHE_ITEM_H
