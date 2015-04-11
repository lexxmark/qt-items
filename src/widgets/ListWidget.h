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

#ifndef QI_LIST_WIDGET_H
#define QI_LIST_WIDGET_H

#include "QiAPI.h"
#include "space/SpaceItem.h"
#include "space/SpaceGrid.h"
#include "core/SpaceWidgetScrollAbstract.h"

namespace Qi
{

class CacheSpaceGrid;
class CacheSpaceItem;
class ViewVisible;

class QI_EXPORT ListWidget: public SpaceWidgetScrollAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(ListWidget)

public:
    explicit ListWidget(QWidget *parent = nullptr);
    virtual ~ListWidget();

    const QSharedPointer<SpaceGrid>& grid() const { return m_grid; }

    const QSharedPointer<Lines>& rows() const { return m_grid->rows(); }
    const QSharedPointer<Lines>& columns() const { return m_grid->columns(); }

    const QSharedPointer<CacheSpaceGrid>& cacheGrid() const { return m_cacheGrid;}

    bool installEmptyView(const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout);

protected:
    QPixmap createPixmapImpl() const;

private:
    void onCacheSpaceGridChanged(const CacheSpace* cache, ChangeReason reason);
    void onSpaceGridChanged(const Space* space, ChangeReason reason);

    QSharedPointer<SpaceGrid> m_grid;
    QSharedPointer<CacheSpaceGrid> m_cacheGrid;

    QSharedPointer<SpaceItem> m_mainSpace;
    QSharedPointer<CacheSpaceItem> m_mainCache;

    QSharedPointer<ViewVisible> m_emptyView;
};

} // end namespace Qi

#endif // QI_LIST_WIDGET_H
