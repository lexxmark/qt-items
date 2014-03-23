#include "test_ranges.h"
#include "utils/RangesBasic.h"
#include <QtTest/QtTest>

using namespace Qi;

struct RangeChangedHandler
{
public:
    RangeChangedHandler()
        : changeCount(0),
          lastRange(nullptr)
    {}
    
    void operator ()(const Range& range)
    {
        ++changeCount;
        lastRange = &range;
    }
    
    quint32 changeCount;
    const Range* lastRange;
};

void TestRanges::testRangeNone()
{
    {
        RangeNone r;
        QVERIFY(!r.parent());
        QVERIFY(!r.hasCell(CellID()));
        QVERIFY(!r.hasCell(1, 1));
    }
    
    {
        QScopedPointer<RangeNone> r(makeRangeNone(this));
        QVERIFY(r.data());
        QVERIFY(r->parent() == this);
        QVERIFY(!r->hasCell(3, 4));
        QVERIFY(!r->hasCell(0, 0));
    }
}

void TestRanges::testRangeAll()
{
    {
        RangeAll r;
        QVERIFY(!r.parent());
        QVERIFY(r.hasCell(CellID()));
        QVERIFY(r.hasCell(1, 1));
    }
    
    {
        QScopedPointer<RangeAll> r(makeRangeAll(this));
        QVERIFY(r.data());
        QVERIFY(r->parent() == this);
        QVERIFY(r->hasCell(3, 4));
        QVERIFY(r->hasCell(0, 0));
    }
}

void TestRanges::testRangeColumn()
{
    {
        RangeColumn r(0);
        QVERIFY(!r.parent());
        QVERIFY(r.hasCell(0, 0));
        QVERIFY(!r.hasCell(0, 1));
        QVERIFY(r.hasCell(1, 0));
        QVERIFY(!r.hasCell(1, 1));
    }
    
    {
        QScopedPointer<RangeColumn> r(makeRangeColumn(4, this));
        QVERIFY(r.data());
        QVERIFY(r->parent() == this);
        QCOMPARE(r->column(), 4u);
        QCOMPARE(r->hasCell(2, 2), false);
        QCOMPARE(r->hasCell(923, 4), true);
        
        RangeChangedHandler rh;
        connect(r.data(), &Range::rangeChanged, std::ref(rh));
        QCOMPARE(rh.changeCount, 0u);
        
        r->setColumn(4);
        QCOMPARE(rh.changeCount, 0u);
        
        r->setColumn(0);
        QCOMPARE(rh.changeCount, 1u);
        QCOMPARE(rh.lastRange, r.data());
        QVERIFY(r->hasCell(23, 0));
        QVERIFY(!r->hasCell(23, 23));
        QVERIFY(!r->hasCell(23, 4));
    }
}

void TestRanges::testRangeColumns()
{
    {
        QSet<quint32> columns;
        RangeColumns r(columns);
        QVERIFY(!r.parent());
        QCOMPARE(r.columns(), QSet<quint32>());
        QVERIFY(!r.hasCell(0, 0));
    }
    
    {
        QSet<quint32> columns;
        columns << 1 << 3;
        RangeColumns r(columns);
        QVERIFY(r.hasCell(1, 1));
        QVERIFY(!r.hasCell(2, 2));
    }
    
    {
        RangeColumns r(3, 6, this);
        QVERIFY(r.parent() == this);
        QVERIFY(!r.hasCell(0, 2));
        QVERIFY(r.hasCell(0, 3));
        QVERIFY(r.hasCell(0, 4));
        QVERIFY(r.hasCell(0, 5));
        QVERIFY(!r.hasCell(0, 6));
    }
    
    {
        QScopedPointer<RangeColumns> r(makeRangeColumns(0, 10));
        QVERIFY(r.data());
        QSet<quint32> columns;
        columns << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
        QVERIFY(r->columns() == columns);
        
        RangeChangedHandler rh;
        connect(r.data(), &Range::rangeChanged, std::ref(rh));
        QCOMPARE(rh.changeCount, 0u);
        
        QVERIFY(r->hasCell(10, 6));
        QVERIFY(!r->hasCell(0, 10));
        QCOMPARE(rh.changeCount, 0u);
        
        columns << 32;
        r->setColumns(columns);
        QCOMPARE(rh.changeCount, 1u);
        QCOMPARE(rh.lastRange, r.data());
        
        QVERIFY(r->hasCell(72, 32));
        QVERIFY(!r->hasCell(32, 72));
    }

    {
        QSet<quint32> columns;
        columns << 0 << 10;
        QScopedPointer<RangeColumns> r(makeRangeColumns(columns));
        QVERIFY(r.data());
        QVERIFY(r->columns() == columns);
        QVERIFY(r->hasCell(9, 10));
        QVERIFY(!r->hasCell(8, 8));
    }
}

void TestRanges::testRangeRow()
{
    {
        RangeRow r(0);
        QVERIFY(!r.parent());
        QVERIFY(r.hasCell(0, 0));
        QVERIFY(r.hasCell(0, 1));
        QVERIFY(!r.hasCell(1, 0));
        QVERIFY(!r.hasCell(1, 1));
    }
    
    {
        QScopedPointer<RangeRow> r(makeRangeRow(4, this));
        QVERIFY(r.data());
        QVERIFY(r->parent() == this);
        QCOMPARE(r->row(), 4u);
        QCOMPARE(r->hasCell(2, 2), false);
        QCOMPARE(r->hasCell(4, 923), true);
        
        RangeChangedHandler rh;
        connect(r.data(), &Range::rangeChanged, std::ref(rh));
        QCOMPARE(rh.changeCount, 0u);
        
        r->setRow(4);
        QCOMPARE(rh.changeCount, 0u);
        
        r->setRow(0);
        QCOMPARE(rh.changeCount, 1u);
        QCOMPARE(rh.lastRange, r.data());
        QVERIFY(r->hasCell(0, 23));
        QVERIFY(!r->hasCell(23, 23));
        QVERIFY(!r->hasCell(4, 23));
    }
}

void TestRanges::testRangeRows()
{
    {
        QSet<quint32> rows;
        RangeRows r(rows);
        QVERIFY(!r.parent());
        QCOMPARE(r.rows(), QSet<quint32>());
        QVERIFY(!r.hasCell(0, 0));
    }
    
    {
        QSet<quint32> rows;
        rows << 1 << 3;
        RangeRows r(rows);
        QVERIFY(r.hasCell(1, 1));
        QVERIFY(!r.hasCell(2, 2));
    }
    
    {
        RangeRows r(3, 6, this);
        QVERIFY(r.parent() == this);
        QVERIFY(!r.hasCell(2, 0));
        QVERIFY(r.hasCell(3, 0));
        QVERIFY(r.hasCell(4, 0));
        QVERIFY(r.hasCell(5, 0));
        QVERIFY(!r.hasCell(6, 0));
    }
    
    {
        QScopedPointer<RangeRows> r(makeRangeRows(0, 10));
        QVERIFY(r.data());
        QSet<quint32> rows;
        rows << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
        QVERIFY(r->rows() == rows);
        
        RangeChangedHandler rh;
        connect(r.data(), &Range::rangeChanged, std::ref(rh));
        QCOMPARE(rh.changeCount, 0u);
        
        QVERIFY(r->hasCell(6, 10));
        QVERIFY(!r->hasCell(10, 0));
        QCOMPARE(rh.changeCount, 0u);
        
        rows << 32;
        r->setRows(rows);
        QCOMPARE(rh.changeCount, 1u);
        QCOMPARE(rh.lastRange, r.data());
        
        QVERIFY(!r->hasCell(72, 32));
        QVERIFY(r->hasCell(32, 72));
    }
    
    {
        QSet<quint32> rows;
        rows << 0 << 10;
        QScopedPointer<RangeRows> r(makeRangeRows(rows));
        QVERIFY(r.data());
        QVERIFY(r->rows() == rows);
        QVERIFY(r->hasCell(10, 9));
        QVERIFY(!r->hasCell(8, 8));
    }
}
