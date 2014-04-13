#include "test_grid.h"
#include "test_cell_id.h"
#include "utils/Grid.h"
#include <QtTest/QtTest>

using namespace Qi;

void TestGrid::test()
{
    Grid grid;
    
    QCOMPARE(grid.dim(), QSize(0, 0));
    QCOMPARE(grid.visibleDim(), QSize(0, 0));
    QCOMPARE(grid.visibleSize(), QSize(0, 0));
    
    grid.rows().setCount(10);
    grid.columns().setCount(12);
    
    QCOMPARE(grid.dim(), QSize(12, 10));

    int emitCount = 0;
    auto lSlot = [&emitCount, &grid](const Grid* _grid, ChangeReason reason) {
        Q_ASSERT(&grid == _grid);
        ++emitCount;
    };
    connect(&grid, &Grid::gridChanged, lSlot);
    
    grid.rows().setCount(10);
    // ChangeReasonLinesCountWeak should be fired
    QCOMPARE(emitCount, 1);
    
    grid.rows().setLineVisible(2, true);
    QCOMPARE(emitCount, 1);

    grid.rows().setCount(12);
    QCOMPARE(emitCount, 2);
    
    QCOMPARE(grid.dim(), QSize(12, 12));
    QCOMPARE(grid.visibleDim(), QSize(12, 12));
    QCOMPARE(grid.visibleSize(), QSize(600, 600));
    QCOMPARE(grid.findVisCell(QPoint(-2, -2)), CellID(0, 0));
    QCOMPARE(grid.findVisCell(QPoint(-2, 3)), CellID(0, 0));
    QCOMPARE(grid.findVisCell(QPoint(-2, 53)), CellID(1, 0));
    
    Lines& rows = grid.rows();
    
    {
        std::vector<quint32> perm(12, 0);
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
    
    QCOMPARE(grid.visibleSize(), QSize(600, 136));
    
    Lines& columns = grid.columns();
    
    {
        std::vector<quint32> perm(12, 0);
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

    QCOMPARE(grid.visibleSize(), QSize(143, 136));
    
    QCOMPARE(grid.visibleDim(), QSize(6, 9));
    QCOMPARE(grid.toVisible(CellID(0, 0)), CellID(2, Invalid));
    QCOMPARE(grid.toVisible(CellID(1, 3)), CellID(4, 4));
    QCOMPARE(grid.toVisible(CellID(4, 2)), CellID(8, 5));
    QCOMPARE(grid.toVisible(CellID(6, 6)), CellID());
    QCOMPARE(grid.toVisibleSafe(CellID(18, 62)), CellID());
    
    QCOMPARE(grid.toAbsolute(CellID(0, 0)), CellID(2, 9));
    QCOMPARE(grid.toAbsolute(CellID(1, 1)), CellID(8, 8));
    QCOMPARE(grid.toAbsolute(CellID(8, 5)), CellID(4, 2));
    QCOMPARE(grid.toAbsoluteSafe(CellID(12, 12)), CellID());
    
    QCOMPARE(grid.findVisCellExact(QPoint(-1, -1)), CellID());
    QCOMPARE(grid.findVisCell(QPoint(-1, -1)), CellID(0, 0));
    QCOMPARE(grid.findVisCellExact(QPoint(-1, 40)), CellID(2, Invalid));
    QCOMPARE(grid.findVisCell(QPoint(-1, 40)), CellID(2, 0));
    QCOMPARE(grid.findVisCellExact(QPoint(40, -10)), CellID(Invalid, 2));
    QCOMPARE(grid.findVisCell(QPoint(40, -10)), CellID(0, 2));
    QCOMPARE(grid.findVisCellExact(QPoint(90, 110)), CellID(7, 3));
    QCOMPARE(grid.findVisCell(QPoint(90, 110)), CellID(7, 3));
    QCOMPARE(grid.findVisCellExact(QPoint(150, 80)), CellID(4, Invalid));
    QCOMPARE(grid.findVisCell(QPoint(150, 80)), CellID(4, 5));
    QCOMPARE(grid.findVisCellExact(QPoint(45, 150)), CellID(Invalid, 2));
    QCOMPARE(grid.findVisCell(QPoint(45, 150)), CellID(8, 2));
    QCOMPARE(grid.findVisCellExact(QPoint(145, 150)), CellID());
    QCOMPARE(grid.findVisCell(QPoint(145, 150)), CellID(8, 5));
    
    Grid grid2(grid.rowsPtr(), grid.columnsPtr());
    QCOMPARE(grid2.findVisCell(QPoint(145, 150)), CellID(8, 5));
    grid.rows().setCount(2);
    QCOMPARE(grid2.rows().count(), 2u);
}
