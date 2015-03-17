#ifndef QI_GRID_WIDGET_H
#define QI_GRID_WIDGET_H

#include "space/SpaceGrid.h"
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
const ItemID topLeftID = ItemID(0, 0);
const ItemID topID = ItemID(0, 1);
const ItemID topRightID = ItemID(0, 2);
const ItemID leftID = ItemID(1, 0);
const ItemID clientID = ItemID(1, 1);
const ItemID rightID = ItemID(1, 2);
const ItemID bottomLeftID = ItemID(2, 0);
const ItemID bottomID = ItemID(2, 1);
const ItemID bottomRightID = ItemID(2, 2);

class QI_EXPORT GridWidget: public SpaceWidgetScrollAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(GridWidget)

public:
    explicit GridWidget(QWidget *parent = nullptr);
    virtual ~GridWidget();

    const QSharedPointer<SpaceGrid>& mainGrid() const { return m_mainGrid; }

    const QSharedPointer<SpaceGrid>& subGrid(const ItemID& subGridID = clientID) const;
    CacheSpaceGrid& cacheSubGrid(const ItemID& subGridID);

    const QSharedPointer<Lines>& rows(int subRowsID) const { return m_rows[subRowsID]; }
    const QSharedPointer<Lines>& columns(int subColumnsID) const { return m_columns[subColumnsID]; }

protected:
    // QAbstractScrollArea implementation
    QSize viewportSizeHint() const override;

    // SpaceWidgetCore implementation
    void ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;

    // SpaceWidgetScrollAbstract implementation
    void validateCacheItemsLayoutImpl() override;
    QSize calculateVirtualSizeImpl() const override;
    QSize calculateScrollableSizeImpl() const override;
    void updateCacheScrollOffsetImpl() override;

private:
    void onSubGridChanged(const Space* space, ChangeReason reason);
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    QSharedPointer<SpaceGrid> m_mainGrid;

    QSharedPointer<Lines> m_rows[3];
    QSharedPointer<Lines> m_columns[3];
    QSharedPointer<CacheSpaceGrid> m_cacheSubGrids[3][3];
};

} // end namespace Qi

#endif // QI_GRID_WIDGET_H
