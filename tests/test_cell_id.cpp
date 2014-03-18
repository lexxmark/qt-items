#include "test_cell_id.h"
#include "utils/CellID.h"
#include <QtTest/QtTest>

#include <set>

using namespace Qi;

void TestCellID::testClass()
{
    CellID cell;
    QCOMPARE(cell.row, Invalid);
    QCOMPARE(cell.column, Invalid);
    QVERIFY(!cell.isValid());
    QCOMPARE(cell, CellID());

    cell.row = 3;
    QVERIFY(!cell.isValid());
    cell.column = 5;
    QVERIFY(cell.isValid());
    QCOMPARE(cell, CellID(3, 5));

    QVERIFY(cell < CellID(3, 6));
    QVERIFY(cell < CellID(4, 2));
    QVERIFY(!(cell < CellID(2, 2)));
}

void TestCellID::testSet()
{
    {
        std::set<CellID> cells;

        cells.insert(CellID(1, 1));
        cells.insert(CellID(2, 2));

        QVERIFY(cells.size() == 2);

        cells.insert(CellID(2, 2));
        QVERIFY(cells.size() == 2);

        auto it = cells.find(CellID(23, 7));
        QVERIFY(it == cells.end());
        it = cells.find(CellID(1, 1));
        QVERIFY(it != cells.end());

        cells.erase(CellID(12, 1));
        QVERIFY(cells.size() == 2);

        cells.erase(CellID(1, 1));
        QVERIFY(cells.size() == 1);

        for (const CellID& cell: cells)
        {
            QVERIFY(cell != CellID(32, 22));
        }
    }

    {
        QSet<CellID> cells;

        cells.insert(CellID(12, 3));
        cells.insert(CellID(3, 3));
        cells.insert(CellID(1, 87));
        QVERIFY(cells.size() == 3);

        cells.insert(CellID(3, 3));
        QVERIFY(cells.size() == 3);

        auto it = cells.find(CellID(23, 3));
        QVERIFY(it == cells.end());

        it = cells.find(CellID(1, 87));
        QVERIFY(it != cells.end());

        cells.remove(CellID(54, 3));
        QVERIFY(cells.size() == 3);

        cells.remove(CellID(12, 3));
        QVERIFY(cells.size() == 2);

        foreach(const CellID& cell, cells)
        {
            QVERIFY(cell != CellID());
        }
    }
}

void TestCellID::testMap()
{
    {
        std::map<CellID, int> m;
        m[CellID(12, 12)] = 4;
        m.insert(std::make_pair(CellID(1, 8), 8));

        QVERIFY(m[CellID(12, 12)] == 4);
        QVERIFY(m[CellID(1, 8)] == 8);

        auto it = m.find(CellID(1, 2));
        QVERIFY(it == m.end());

        it = m.find(CellID(1, 8));
        QVERIFY(it != m.end());
        QVERIFY((*it).second == 8);

        m.erase(CellID());
        QVERIFY(m.size() == 2);

        m.erase(CellID(12, 12));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }

    {
        QMap<CellID, QString> m;
        m[CellID()] = tr("one");
        m.insert(CellID(), tr("two"));

        QVERIFY(m.size() == 1);

        m.insert(CellID(1, 1), tr("three"));
        QVERIFY(m.size() == 2);

        QVERIFY(m.contains(CellID(1, 1)));

        auto it = m.find(CellID(2, 2));
        QVERIFY(it == m.end());

        it = m.find(CellID());
        QVERIFY(it != m.end());
        QVERIFY((*it) == tr("two"));

        m.remove(CellID(2, 2));
        QVERIFY(m.size() == 2);

        m.remove(CellID(1, 1));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }
}
