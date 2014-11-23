#include "test_ranges.h"
#include "core/ext/Ranges.h"
#include "SignalSpy.h"
#include <QtTest/QtTest>

using namespace Qi;

void TestRanges::testRangeNone()
{
    {
        RangeNone r;
        QVERIFY(!r.parent());
        QVERIFY(!r.hasItem(ItemID()));
        QVERIFY(!r.hasItem(1, 1));
    }
    
    {
        QSharedPointer<Range> r = makeRangeNone();
        QVERIFY(r.data());
        QVERIFY(!r->parent());
        QVERIFY(!r->hasItem(3, 4));
        QVERIFY(!r->hasItem(0, 0));
    }
}

void TestRanges::testRangeAll()
{
    {
        RangeAll r;
        QVERIFY(!r.parent());
        QVERIFY(r.hasItem(ItemID()));
        QVERIFY(r.hasItem(1, 1));
    }
    
    {
        QSharedPointer<RangeAll> r(makeRangeAll());
        QVERIFY(r.data());
        QVERIFY(!r->parent());
        QVERIFY(r->hasItem(3, 4));
        QVERIFY(r->hasItem(0, 0));
    }
}

void TestRanges::testRangeColumn()
{
    {
        RangeColumn r(0);
        QVERIFY(!r.parent());
        QVERIFY(r.hasItem(0, 0));
        QVERIFY(!r.hasItem(0, 1));
        QVERIFY(r.hasItem(1, 0));
        QVERIFY(!r.hasItem(1, 1));
    }
    
    {
        QSharedPointer<RangeColumn> r = makeRangeColumn(4);
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
        RangeColumns r(columns);
        QVERIFY(!r.parent());
        QCOMPARE(r.columns(), QSet<int>());
        QVERIFY(!r.hasItem(0, 0));
    }
    
    {
        QSet<int> columns;
        columns << 1 << 3;
        RangeColumns r(columns);
        QVERIFY(r.hasItem(1, 1));
        QVERIFY(!r.hasItem(2, 2));
    }
    
    {
        RangeColumns r(3, 6);
        QVERIFY(!r.parent());
        QVERIFY(!r.hasItem(0, 2));
        QVERIFY(r.hasItem(0, 3));
        QVERIFY(r.hasItem(0, 4));
        QVERIFY(r.hasItem(0, 5));
        QVERIFY(!r.hasItem(0, 6));
    }
    
    {
        QSharedPointer<RangeColumns> r(makeRangeColumns(0, 10));
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
        QSharedPointer<RangeColumns> r(makeRangeColumns(columns));
        QVERIFY(r.data());
        QVERIFY(r->columns() == columns);
        QVERIFY(r->hasItem(9, 10));
        QVERIFY(!r->hasItem(8, 8));
    }
}

void TestRanges::testRangeRow()
{
    {
        RangeRow r(0);
        QVERIFY(!r.parent());
        QVERIFY(r.hasItem(0, 0));
        QVERIFY(r.hasItem(0, 1));
        QVERIFY(!r.hasItem(1, 0));
        QVERIFY(!r.hasItem(1, 1));
    }
    
    {
        QSharedPointer<RangeRow> r(makeRangeRow(4));
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
        RangeRows r(rows);
        QVERIFY(!r.parent());
        QCOMPARE(r.rows(), QSet<int>());
        QVERIFY(!r.hasItem(0, 0));
    }
    
    {
        QSet<int> rows;
        rows << 1 << 3;
        RangeRows r(rows);
        QVERIFY(r.hasItem(1, 1));
        QVERIFY(!r.hasItem(2, 2));
    }
    
    {
        RangeRows r(3, 6);
        QVERIFY(!r.parent());
        QVERIFY(!r.hasItem(2, 0));
        QVERIFY(r.hasItem(3, 0));
        QVERIFY(r.hasItem(4, 0));
        QVERIFY(r.hasItem(5, 0));
        QVERIFY(!r.hasItem(6, 0));
    }
    
    {
        QSharedPointer<RangeRows> r(makeRangeRows(0, 10));
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
        QSharedPointer<RangeRows> r(makeRangeRows(rows));
        QVERIFY(r.data());
        QVERIFY(r->rows() == rows);
        QVERIFY(r->hasItem(10, 9));
        QVERIFY(!r->hasItem(8, 8));
    }
}
