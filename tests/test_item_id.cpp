#include "test_item_id.h"

#include <set>
#include <type_traits>
#include <cstring>

using namespace Qi;

void TestItemID::testClass()
{
    GridID item;
    QCOMPARE(item.row, InvalidIndex);
    QCOMPARE(item.column, InvalidIndex);
    QVERIFY(!item.isValid());
    QCOMPARE(item, GridID());

    item.row = 3;
    QVERIFY(!item.isValid());
    item.column = 5;
    QVERIFY(item.isValid());
    QCOMPARE(item, GridID(3, 5));

    QVERIFY(item < GridID(3, 6));
    QVERIFY(item < GridID(4, 2));
    QVERIFY(!(item < GridID(2, 2)));
}

void TestItemID::testSet()
{
    {
        std::set<GridID> items;

        items.insert(GridID(1, 1));
        items.insert(GridID(2, 2));

        QVERIFY(items.size() == 2);

        items.insert(GridID(2, 2));
        QVERIFY(items.size() == 2);

        auto it = items.find(GridID(23, 7));
        QVERIFY(it == items.end());
        it = items.find(GridID(1, 1));
        QVERIFY(it != items.end());

        items.erase(GridID(12, 1));
        QVERIFY(items.size() == 2);

        items.erase(GridID(1, 1));
        QVERIFY(items.size() == 1);

        for (GridID id: items)
        {
            QVERIFY(id != GridID(32, 22));
        }
    }

    {
        QSet<GridID> items;

        items.insert(GridID(12, 3));
        items.insert(GridID(3, 3));
        items.insert(GridID(1, 87));
        QVERIFY(items.size() == 3);

        items.insert(GridID(3, 3));
        QVERIFY(items.size() == 3);

        auto it = items.find(GridID(23, 3));
        QVERIFY(it == items.end());

        it = items.find(GridID(1, 87));
        QVERIFY(it != items.end());

        items.remove(GridID(54, 3));
        QVERIFY(items.size() == 3);

        items.remove(GridID(12, 3));
        QVERIFY(items.size() == 2);

        foreach(GridID id, items)
        {
            QVERIFY(id != GridID());
        }
    }
}

void TestItemID::testMap()
{
    {
        std::map<GridID, int> m;
        m[GridID(12, 12)] = 4;
        m.insert(std::make_pair(GridID(1, 8), 8));

        QVERIFY(m[GridID(12, 12)] == 4);
        QVERIFY(m[GridID(1, 8)] == 8);

        auto it = m.find(GridID(1, 2));
        QVERIFY(it == m.end());

        it = m.find(GridID(1, 8));
        QVERIFY(it != m.end());
        QVERIFY((*it).second == 8);

        m.erase(GridID());
        QVERIFY(m.size() == 2);

        m.erase(GridID(12, 12));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }

    {
        QMap<GridID, QString> m;
        m[GridID()] = tr("one");
        m.insert(GridID(), tr("two"));

        QVERIFY(m.size() == 1);

        m.insert(GridID(1, 1), tr("three"));
        QVERIFY(m.size() == 2);

        QVERIFY(m.contains(GridID(1, 1)));

        auto it = m.find(GridID(2, 2));
        QVERIFY(it == m.end());

        it = m.find(GridID());
        QVERIFY(it != m.end());
        QVERIFY((*it) == tr("two"));

        m.remove(GridID(2, 2));
        QVERIFY(m.size() == 2);

        m.remove(GridID(1, 1));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }
}
