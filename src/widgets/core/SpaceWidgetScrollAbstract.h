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

#ifndef QI_SPACE_WIDGET_SCROLL_ABSTRACT_H
#define QI_SPACE_WIDGET_SCROLL_ABSTRACT_H

#include "SpaceWidgetCore.h"
#include <QAbstractScrollArea>

namespace Qi
{

class QI_EXPORT SpaceWidgetScrollAbstract: public QAbstractScrollArea, public SpaceWidgetCore
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceWidgetScrollAbstract)

public:
    virtual ~SpaceWidgetScrollAbstract();

protected:
    explicit SpaceWidgetScrollAbstract(QWidget *parent = nullptr);

    bool initSpaceWidgetScrollable(const QSharedPointer<CacheSpace>& mainCacheSpace, const QSharedPointer<CacheSpace>& scrollableCacheSpace);

    bool viewportEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;
    void scrollContentsBy(int dx, int dy) override;
    QSize viewportSizeHint() const override;

    // SpaceWidgetCore implementation
    void ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;

    void updateScrollbars();
    void invalidateCacheItemsLayout();
    void validateCacheItemsLayout();

    virtual void validateCacheItemsLayoutImpl();
    virtual QSize calculateVirtualSizeImpl() const;
    virtual QSize calculateScrollableSizeImpl() const;
    virtual void updateCacheScrollOffsetImpl();

private:
    // hide method
    using SpaceWidgetCore::initSpaceWidgetCore;
    void onScrollCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    QSharedPointer<CacheSpace> m_scrollableCacheSpace;

    bool m_isCacheItemsLayoutValid;
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_SCROLL_ABSTRACT_H
