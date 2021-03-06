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

#ifndef QI_GRID_WIDGET_H
#define QI_GRID_WIDGET_H

#include "space/grid/SpaceGrid.h"
#include "core/SpaceWidgetScrollAbstract.h"

namespace Qi
{

class CacheSpaceGrid;

// sub grid IDs
// -------------------------------------------
// | topLeftID    | topID    | topRightID    |
// |-----------------------------------------|
// | leftID       | clientID | rightID       |
// |-----------------------------------------|
// | bottomLeftID | bottomID | bottomRightID |
// -------------------------------------------
const GridID topLeftID = GridID(0, 0);
const GridID topID = GridID(0, 1);
const GridID topRightID = GridID(0, 2);
const GridID leftID = GridID(1, 0);
const GridID clientID = GridID(1, 1);
const GridID rightID = GridID(1, 2);
const GridID bottomLeftID = GridID(2, 0);
const GridID bottomID = GridID(2, 1);
const GridID bottomRightID = GridID(2, 2);

class QI_EXPORT GridWidget: public SpaceWidgetScrollAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(GridWidget)

public:
    explicit GridWidget(QWidget *parent = nullptr);
    virtual ~GridWidget();

    const SharedPtr<SpaceGrid>& mainGrid() const { return m_mainGrid; }

    const SharedPtr<SpaceGrid>& subGrid(GridID subGridID = clientID) const;
    const SharedPtr<CacheSpaceGrid>& cacheSubGrid(GridID subGridID = clientID) const;

    const SharedPtr<Lines>& rows(int subRowsID) const { return m_rows[subRowsID]; }
    const SharedPtr<Lines>& columns(int subColumnsID) const { return m_columns[subColumnsID]; }

protected:
    // QAbstractScrollArea implementation
    QSize viewportSizeHint() const override;

    // SpaceWidgetCore implementation
    void ensureVisibleImpl(const ID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;

    // SpaceWidgetScrollAbstract implementation
    void validateCacheItemsLayoutImpl() override;
    QSize calculateVirtualSizeImpl() const override;
    QSize calculateScrollableSizeImpl() const override;
    void updateCacheScrollOffsetImpl() override;

private:
    void onSubGridChanged(const Space* space, ChangeReason reason);
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    SharedPtr<SpaceGrid> m_mainGrid;

    SharedPtr<Lines> m_rows[3];
    SharedPtr<Lines> m_columns[3];
    SharedPtr<CacheSpaceGrid> m_cacheSubGrids[3][3];
};

} // end namespace Qi

#endif // QI_GRID_WIDGET_H
