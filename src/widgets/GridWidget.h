#ifndef QI_GRID_WIDGET_H
#define QI_GRID_WIDGET_H

#include "space/SpaceGrid.h"
#include "WidgetDriver.h"
#include <QAbstractScrollArea>

namespace Qi
{

class SpaceWidgetPrivate;
class CacheSpace;
class CacheSpaceGrid;
class ControllerKeyboard;

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

class QI_EXPORT GridWidget: public QAbstractScrollArea, public WidgetDriver
{
    Q_OBJECT
    Q_DISABLE_COPY(GridWidget)

public:
    explicit GridWidget(QWidget *parent = nullptr);
    virtual ~GridWidget();

    const QSharedPointer<SpaceGrid>& mainGrid() const { return m_mainGrid; }

    const QSharedPointer<SpaceGrid>& subGrid(const ItemID& subGridID = clientID) const;
    CacheSpaceGrid& cacheSubGrid(const ItemID& subGridID);

    const QSharedPointer<ControllerKeyboard>& controllerKeyboard() const;
    void setControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard);
    void addControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard);

    void ensureVisible(const ItemID& visibleItem, const ItemID& subGridID = clientID, bool validateItem = false);

    // WidgetDriver implementation
    void ensureVisible(const ItemID& visibleItem, const CacheSpace* cacheSpace, bool validateItem) override;
    bool doEdit(const ItemID& visibleItem, const CacheSpace* cacheSpace, const QKeyEvent* event) override;

protected:
    bool viewportEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    QSize viewportSizeHint() const override;

private:
    void onSubGridChanged(const Space* space, ChangeReason reason);
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);
    void updateScrollbars();
    void updateCacheScrollOffsets();
    void invalidateSubGridsLayout();
    void validateSubGridsLayout();

    QSharedPointer<SpaceGrid> m_mainGrid;

    QSharedPointer<Lines> m_rows[3];
    QSharedPointer<Lines> m_columns[3];
    QSharedPointer<CacheSpaceGrid> m_cacheSubGrids[3][3];

    QScopedPointer<SpaceWidgetPrivate> m_impl;

    bool m_isSubGridsLayoutValid;
};

} // end namespace Qi

#endif // QI_GRID_WIDGET_H
