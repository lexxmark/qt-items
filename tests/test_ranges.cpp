#include "test_ranges.h"
#include "core/ext/Ranges.h"
#include "SignalSpy.h"
#include <QtTest/QtTest>
#include "space/grid/RangeGrid.h"

using namespace Qi;

void TestRanges::testRangeNone()
{
    {
        RangeNone r;
        QVERIFY(!r.parent());
        QVERIFY(!r.hasItem(ID(0)));
        QVERIFY(!r.hasItem(makeID<GridID>(1, 1)));
    }
    
    {
        SharedPtr<Range> r = makeRangeNone();
        QVERIFY(r.data());
        QVERIFY(!r->parent());
        QVERIFY(!r->hasItem(makeID<GridID>(3, 4)));
        QVERIFY(!r->hasItem(makeID<GridID>(0, 0)));
    }
}

void TestRanges::testRangeAll()
{
    {
        RangeAll r;
        QVERIFY(!r.parent());
        QVERIFY(r.hasItem(ID(0)));
        QVERIFY(r.hasItem(makeID<GridID>(1, 1)));
    }
    
    {
        auto r = makeRangeAll();
        QVERIFY(r.data());
        QVERIFY(!r->parent());
        QVERIFY(r->hasItem(makeID<GridID>(3, 4)));
        QVERIFY(r->hasItem(makeID<GridID>(0, 0)));
    }
}

void TestRanges::testRangeColumn()
{
    {
        RangeGridColumn r(0);
        QVERIFY(!r.parent());
        QVERIFY(r.hasItem(0, 0));
        QVERIFY(!r.hasItem(0, 1));
        QVERIFY(r.hasItem(1, 0));
        QVERIFY(!r.hasItem(1, 1));
    }
    
    {
        auto r = makeRangeGridColumn(4);
        QVERIFY(r.data());
        QVERIFY(!r->parent());
        QCOMPARE(r->column(), 4);
        QCOMPARE(r->hasItem(2, 2), false);
        QCOMPARE(r->hasItem(923, 4), true);
        
        auto signalSpy = createSignalSpy(r.data(), &Range::rangeChanged);
        QCOMPARE(signalSpy.empty(), true);
        
        r->setColumn(4);
        QCOMPARE(signalSpy.empty(), true);

        r->setColumn(0);
        QCOMPARE(signalSpy.size(), 1);
        QVERIFY(r->hasItem(23, 0));
        QVERIFY(!r->hasItem(23, 23));
        QVERIFY(!r->hasItem(23, 4));
    }
}

void TestRanges::testRangeColumns()
{
    {
        QSet<int> columns;
        RangeGridColumns r(columns);
        QVERIFY(!r.parent());
        QCOMPARE(r.columns(), QSet<int>());
        QVERIFY(!r.hasItem(0, 0));
    }
    
    {
        QSet<int> columns;
        columns << 1 << 3;
        RangeGridColumns r(columns);
        QVERIFY(r.hasItem(1, 1));
        QVERIFY(!r.hasItem(2, 2));
    }
    
    {
        RangeGridColumns r(3, 6);
        QVERIFY(!r.parent());
        QVERIFY(!r.hasItem(0, 2));
        QVERIFY(r.hasItem(0, 3));
        QVERIFY(r.hasItem(0, 4));
        QVERIFY(r.hasItem(0, 5));
        QVERIFY(!r.hasItem(0, 6));
    }
    
    {
        SharedPtr<RangeGridColumns> r(makeRangeGridColumns(0, 10));
        QVERIFY(r.data());
        QSet<int> columns;
        columns << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
        QVERIFY(r->columns() == columns);
        
        auto signalSpy = createSignalSpy(r.data(), &Range::rangeChanged);
        QCOMPARE(signalSpy.empty(), true);
        
        QVERIFY(r->hasItem(10, 6));
        QVERIFY(!r->hasItem(0, 10));
        QCOMPARE(signalSpy.empty(), true);
        
        columns << 32;
        r->setColumns(columns);
        QCOMPARE(signalSpy.size(), 1);
        
        QVERIFY(r->hasItem(72, 32));
        QVERIFY(!r->hasItem(32, 72));
    }

    {
        QSet<int> columns;
        columns << 0 << 10;
        SharedPtr<RangeGridColumns> r(makeRangeGridColumns(columns));
        QVERIFY(r.data());
        QVERIFY(r->columns() == columns);
        QVERIFY(r->hasItem(9, 10));
        QVERIFY(!r->hasItem(8, 8));
    }
}

void TestRanges::testRangeRow()
{
    {
        RangeGridRow r(0);
        QVERIFY(!r.parent());
        QVERIFY(r.hasItem(0, 0));
        QVERIFY(r.hasItem(0, 1));
        QVERIFY(!r.hasItem(1, 0));
        QVERIFY(!r.hasItem(1, 1));
    }
    
    {
        SharedPtr<RangeGridRow> r(makeRangeGridRow(4));
        QVERIFY(r.data());
        QVERIFY(!r->parent());
        QCOMPARE(r->row(), 4);
        QCOMPARE(r->hasItem(2, 2), false);
        QCOMPARE(r->hasItem(4, 923), true);
        
        auto signalSpy = createSignalSpy(r.data(), &Range::rangeChanged);
        QCOMPARE(signalSpy.empty(), true);
        
        r->setRow(4);
        QCOMPARE(signalSpy.empty(), true);
        
        r->setRow(0);
        QCOMPARE(signalSpy.size(), 1);
        QVERIFY(r->hasItem(0, 23));
        QVERIFY(!r->hasItem(23, 23));
        QVERIFY(!r->hasItem(4, 23));
    }
}

void TestRanges::testRangeRows()
{
    {
        QSet<int> rows;
        RangeGridRows r(rows);
        QVERIFY(!r.parent());
        QCOMPARE(r.rows(), QSet<int>());
        QVERIFY(!r.hasItem(0, 0));
    }
    
    {
        QSet<int> rows;
        rows << 1 << 3;
        RangeGridRows r(rows);
        QVERIFY(r.hasItem(1, 1));
        QVERIFY(!r.hasItem(2, 2));
    }
    
    {
        RangeGridRows r(3, 6);
        QVERIFY(!r.parent());
        QVERIFY(!r.hasItem(2, 0));
        QVERIFY(r.hasItem(3, 0));
        QVERIFY(r.hasItem(4, 0));
        QVERIFY(r.hasItem(5, 0));
        QVERIFY(!r.hasItem(6, 0));
    }
    
    {
        SharedPtr<RangeGridRows> r(makeRangeGridRows(0, 10));
        QVERIFY(r.data());
        QSet<int> rows;
        rows << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
        QVERIFY(r->rows() == rows);
        
        auto signalSpy = createSignalSpy(r.data(), &Range::rangeChanged);
        QCOMPARE(signalSpy.size(), 0);
        
        QVERIFY(r->hasItem(6, 10));
        QVERIFY(!r->hasItem(10, 0));
        QCOMPARE(signalSpy.size(), 0);
        
        rows << 32;
        r->setRows(rows);
        QCOMPARE(signalSpy.size(), 1);
        
        QVERIFY(!r->hasItem(72, 32));
        QVERIFY(r->hasItem(32, 72));
    }
    
    {
        QSet<int> rows;
        rows << 0 << 10;
        SharedPtr<RangeGridRows> r(makeRangeGridRows(rows));
        QVERIFY(r.data());
        QVERIFY(r->rows() == rows);
        QVERIFY(r->hasItem(10, 9));
        QVERIFY(!r->hasItem(8, 8));
    }
}
