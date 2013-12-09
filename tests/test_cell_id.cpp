#include "test_cell_id.h"
#include "utils/QiCellID.h"
#include <QtTest/QtTest>

#include <set>

void TestCellID::testClass()
{
    QiCellID cell;
    QCOMPARE(cell.row, QiInvalid);
    QCOMPARE(cell.column, QiInvalid);
    QVERIFY(!cell.isValid());
    QCOMPARE(cell, QiCellID());

    cell.row = 3;
    QVERIFY(!cell.isValid());
    cell.column = 5;
    QVERIFY(cell.isValid());
    QCOMPARE(cell, QiCellID(3, 5));

    QVERIFY(cell < QiCellID(3, 6));
    QVERIFY(cell < QiCellID(4, 2));
    QVERIFY(!(cell < QiCellID(2, 2)));
}

void TestCellID::testSet()
{
    {
        std::set<QiCellID> cells;

        cells.insert(QiCellID(1, 1));
        cells.insert(QiCellID(2, 2));

        QVERIFY(cells.size() == 2);

        cells.insert(QiCellID(2, 2));
        QVERIFY(cells.size() == 2);

        auto it = cells.find(QiCellID(23, 7));
        QVERIFY(it == cells.end());
        it = cells.find(QiCellID(1, 1));
        QVERIFY(it != cells.end());

        cells.erase(QiCellID(12, 1));
        QVERIFY(cells.size() == 2);

        cells.erase(QiCellID(1, 1));
        QVERIFY(cells.size() == 1);

        for (const QiCellID& cell: cells)
        {
            QVERIFY(cell != QiCellID(32, 22));
        }
    }

    {
        QSet<QiCellID> cells;

        cells.insert(QiCellID(12, 3));
        cells.insert(QiCellID(3, 3));
        cells.insert(QiCellID(1, 87));
        QVERIFY(cells.size() == 3);

        cells.insert(QiCellID(3, 3));
        QVERIFY(cells.size() == 3);

        auto it = cells.find(QiCellID(23, 3));
        QVERIFY(it == cells.end());

        it = cells.find(QiCellID(1, 87));
        QVERIFY(it != cells.end());

        cells.remove(QiCellID(54, 3));
        QVERIFY(cells.size() == 3);

        cells.remove(QiCellID(12, 3));
        QVERIFY(cells.size() == 2);

        foreach(const QiCellID& cell, cells)
        {
            QVERIFY(cell != QiCellID());
        }
    }
}

void TestCellID::testMap()
{
    {
        std::map<QiCellID, int> m;
        m[QiCellID(12, 12)] = 4;
        m.insert(std::make_pair(QiCellID(1, 8), 8));

        QVERIFY(m[QiCellID(12, 12)] == 4);
        QVERIFY(m[QiCellID(1, 8)] == 8);

        auto it = m.find(QiCellID(1, 2));
        QVERIFY(it == m.end());

        it = m.find(QiCellID(1, 8));
        QVERIFY(it != m.end());
        QVERIFY((*it).second == 8);

        m.erase(QiCellID());
        QVERIFY(m.size() == 2);

        m.erase(QiCellID(12, 12));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }

    {
        QMap<QiCellID, QString> m;
        m[QiCellID()] = tr("one");
        m.insert(QiCellID(), tr("two"));

        QVERIFY(m.size() == 1);

        m.insert(QiCellID(1, 1), tr("three"));
        QVERIFY(m.size() == 2);

        QVERIFY(m.contains(QiCellID(1, 1)));

        auto it = m.find(QiCellID(2, 2));
        QVERIFY(it == m.end());

        it = m.find(QiCellID());
        QVERIFY(it != m.end());
        QVERIFY((*it) == tr("two"));

        m.remove(QiCellID(2, 2));
        QVERIFY(m.size() == 2);

        m.remove(QiCellID(1, 1));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }
}
