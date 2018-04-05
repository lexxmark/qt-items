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
#include "space/item/SpaceItem.h"
#include "space/grid/SpaceGrid.h"
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

    const SharedPtr<SpaceGrid>& grid() const { return m_grid; }

    const SharedPtr<Lines>& rows() const { return m_grid->rows(); }
    const SharedPtr<Lines>& columns() const { return m_grid->columns(); }

    const SharedPtr<CacheSpaceGrid>& cacheGrid() const { return m_cacheGrid;}

    bool installEmptyView(SharedPtr<View> view, SharedPtr<Layout> layout);

protected:
    QPixmap createPixmapImpl() const;

private:
    void onCacheSpaceGridChanged(const CacheSpace* cache, ChangeReason reason);
    void onSpaceGridChanged(const Space* space, ChangeReason reason);

    SharedPtr<SpaceGrid> m_grid;
    SharedPtr<CacheSpaceGrid> m_cacheGrid;

    SharedPtr<SpaceItem> m_mainSpace;
    SharedPtr<CacheSpaceItem> m_mainCache;

    SharedPtr<ViewVisible> m_emptyView;
};

} // end namespace Qi

#endif // QI_LIST_WIDGET_H
