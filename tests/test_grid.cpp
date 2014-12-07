#include "test_grid.h"
#include "test_item_id.h"
#include "space/SpaceGrid.h"
#include "SignalSpy.h"
#include <QtTest/QtTest>

using namespace Qi;

void TestGrid::test()
{
    SpaceGrid grid;
 /*
    QCOMPARE(grid.dim(), QSize(0, 0));
    QCOMPARE(grid.visibleDim(), QSize(0, 0));
    QCOMPARE(grid.size(), QSize(0, 0));
    
    grid.rows().setCount(10);
    grid.columns().setCount(12);
    
    QCOMPARE(grid.dim(), QSize(12, 10));

    auto signalSpy = createSignalSpy(&grid, &SpaceGrid::spaceChanged);

    grid.rows().setCount(10);
    // ChangeReasonLinesCountWeak should be fired
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCountWeak);
    
    grid.rows().setLineVisible(2, true);
    QCOMPARE(signalSpy.size(), 1);

    grid.rows().setCount(12);
    QCOMPARE(signalSpy.size(), 2);

    QCOMPARE(grid.dim(), QSize(12, 12));
    QCOMPARE(grid.visibleDim(), QSize(12, 12));
    QCOMPARE(grid.size(), QSize(252, 252));
    grid.rows().setAllLinesSize(50);
    grid.columns().setAllLinesSize(50);
    QCOMPARE(grid.size(), QSize(600, 600));
    QCOMPARE(grid.findVisItem(QPoint(-2, -2)), ItemID(0, 0));
    QCOMPARE(grid.findVisItem(QPoint(-2, 3)), ItemID(0, 0));
    QCOMPARE(grid.findVisItem(QPoint(-2, 53)), ItemID(1, 0));
    
    Lines& rows = grid.rows();
    
    {
        std::vector<int> perm(12, 0);
        perm[0] = 3;
        perm[1] = 6;
        perm[2] = 1;
        perm[3] = 9;
        perm[4] = 11;
        perm[5] = 0;
        perm[6] = 8;
        perm[7] = 5;
        perm[8] = 2;
        perm[9] = 4;
        perm[10] = 7;
        perm[11] = 10;
        rows.setPermutation(perm);
    }
    
    rows.setLineVisible(5, false);
    rows.setLineVisible(6, false);
    rows.setLineVisible(9, false);
    
    rows.setAllLinesSize(15);
    rows.setLineSize(2, 20);
    rows.setLineSize(3, 5);
    rows.setLineSize(7, 30);
    rows.setLineSize(8, 6);
    
    QCOMPARE(grid.size(), QSize(600, 136));
    
    Lines& columns = grid.columns();
    
    {
        std::vector<int> perm(12, 0);
        perm[0] = 11;
        perm[1] =10;
        perm[2] = 9;
        perm[3] = 8;
        perm[4] = 7;
        perm[5] = 6;
        perm[6] = 5;
        perm[7] = 4;
        perm[8] = 3;
        perm[9] = 2;
        perm[10] = 1;
        perm[11] = 0;
        columns.setPermutation(perm);
    }
    
    columns.setLineVisible(0, false);
    columns.setLineVisible(1, false);
    columns.setLineVisible(6, false);
    columns.setLineVisible(7, false);
    columns.setLineVisible(10, false);
    columns.setLineVisible(11, false);
    
    columns.setAllLinesSize(10);
    columns.setLineSize(2, 40);
    columns.setLineSize(4, 43);
    columns.setLineSize(8, 32);
    columns.setLineSize(9, 8);

    QCOMPARE(grid.size(), QSize(143, 136));
    
    QCOMPARE(grid.visibleDim(), QSize(6, 9));
    QCOMPARE(grid.toVisible(ItemID(0, 0)), ItemID(2, Invalid));
    QCOMPARE(grid.toVisible(ItemID(1, 3)), ItemID(4, 4));
    QCOMPARE(grid.toVisible(ItemID(4, 2)), ItemID(8, 5));
    QCOMPARE(grid.toVisible(ItemID(6, 6)), ItemID());
    QCOMPARE(grid.toVisibleSafe(ItemID(18, 62)), ItemID());
    
    QCOMPARE(grid.toAbsolute(ItemID(0, 0)), ItemID(2, 9));
    QCOMPARE(grid.toAbsolute(ItemID(1, 1)), ItemID(8, 8));
    QCOMPARE(grid.toAbsolute(ItemID(8, 5)), ItemID(4, 2));
    QCOMPARE(grid.toAbsoluteSafe(ItemID(12, 12)), ItemID());
    
    QCOMPARE(grid.findVisItemExact(QPoint(-1, -1)), ItemID());
    QCOMPARE(grid.findVisItem(QPoint(-1, -1)), ItemID(0, 0));
    QCOMPARE(grid.findVisItemExact(QPoint(-1, 40)), ItemID(2, Invalid));
    QCOMPARE(grid.findVisItem(QPoint(-1, 40)), ItemID(2, 0));
    QCOMPARE(grid.findVisItemExact(QPoint(40, -10)), ItemID(Invalid, 2));
    QCOMPARE(grid.findVisItem(QPoint(40, -10)), ItemID(0, 2));
    QCOMPARE(grid.findVisItemExact(QPoint(90, 110)), ItemID(7, 3));
    QCOMPARE(grid.findVisItem(QPoint(90, 110)), ItemID(7, 3));
    QCOMPARE(grid.findVisItemExact(QPoint(150, 80)), ItemID(4, Invalid));
    QCOMPARE(grid.findVisItem(QPoint(150, 80)), ItemID(4, 5));
    QCOMPARE(grid.findVisItemExact(QPoint(45, 150)), ItemID(Invalid, 2));
    QCOMPARE(grid.findVisItem(QPoint(45, 150)), ItemID(8, 2));
    QCOMPARE(grid.findVisItemExact(QPoint(145, 150)), ItemID());
    QCOMPARE(grid.findVisItem(QPoint(145, 150)), ItemID(8, 5));
    
    SpaceGrid grid2(grid.rowsPtr(), grid.columnsPtr());
    QCOMPARE(grid2.findVisItem(QPoint(145, 150)), ItemID(8, 5));
    grid.rows().setCount(2);
    QCOMPARE(grid2.rows().count(), 2u);

    QCOMPARE(signalSpy.size(), 26);
    */
}
