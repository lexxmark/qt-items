#include "test_grid.h"
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
    auto lSlot = [&emitCount, &grid](const Grid& _grid, ChangeReason reason) {
        Q_ASSERT(&grid == &_grid);
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
}
