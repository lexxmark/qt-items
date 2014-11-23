#include "test_item_id.h"

#include <set>

using namespace Qi;

void TestItemID::testClass()
{
    ItemID item;
    QCOMPARE(item.row, InvalidIndex);
    QCOMPARE(item.column, InvalidIndex);
    QVERIFY(!item.isValid());
    QCOMPARE(item, ItemID());

    item.row = 3;
    QVERIFY(!item.isValid());
    item.column = 5;
    QVERIFY(item.isValid());
    QCOMPARE(item, ItemID(3, 5));

    QVERIFY(item < ItemID(3, 6));
    QVERIFY(item < ItemID(4, 2));
    QVERIFY(!(item < ItemID(2, 2)));
}

void TestItemID::testSet()
{
    {
        std::set<ItemID> items;

        items.insert(ItemID(1, 1));
        items.insert(ItemID(2, 2));

        QVERIFY(items.size() == 2);

        items.insert(ItemID(2, 2));
        QVERIFY(items.size() == 2);

        auto it = items.find(ItemID(23, 7));
        QVERIFY(it == items.end());
        it = items.find(ItemID(1, 1));
        QVERIFY(it != items.end());

        items.erase(ItemID(12, 1));
        QVERIFY(items.size() == 2);

        items.erase(ItemID(1, 1));
        QVERIFY(items.size() == 1);

        for (const ItemID& item: items)
        {
            QVERIFY(item != ItemID(32, 22));
        }
    }

    {
        QSet<ItemID> items;

        items.insert(ItemID(12, 3));
        items.insert(ItemID(3, 3));
        items.insert(ItemID(1, 87));
        QVERIFY(items.size() == 3);

        items.insert(ItemID(3, 3));
        QVERIFY(items.size() == 3);

        auto it = items.find(ItemID(23, 3));
        QVERIFY(it == items.end());

        it = items.find(ItemID(1, 87));
        QVERIFY(it != items.end());

        items.remove(ItemID(54, 3));
        QVERIFY(items.size() == 3);

        items.remove(ItemID(12, 3));
        QVERIFY(items.size() == 2);

        foreach(const ItemID& item, items)
        {
            QVERIFY(item != ItemID());
        }
    }
}

void TestItemID::testMap()
{
    {
        std::map<ItemID, int> m;
        m[ItemID(12, 12)] = 4;
        m.insert(std::make_pair(ItemID(1, 8), 8));

        QVERIFY(m[ItemID(12, 12)] == 4);
        QVERIFY(m[ItemID(1, 8)] == 8);

        auto it = m.find(ItemID(1, 2));
        QVERIFY(it == m.end());

        it = m.find(ItemID(1, 8));
        QVERIFY(it != m.end());
        QVERIFY((*it).second == 8);

        m.erase(ItemID());
        QVERIFY(m.size() == 2);

        m.erase(ItemID(12, 12));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }

    {
        QMap<ItemID, QString> m;
        m[ItemID()] = tr("one");
        m.insert(ItemID(), tr("two"));

        QVERIFY(m.size() == 1);

        m.insert(ItemID(1, 1), tr("three"));
        QVERIFY(m.size() == 2);

        QVERIFY(m.contains(ItemID(1, 1)));

        auto it = m.find(ItemID(2, 2));
        QVERIFY(it == m.end());

        it = m.find(ItemID());
        QVERIFY(it != m.end());
        QVERIFY((*it) == tr("two"));

        m.remove(ItemID(2, 2));
        QVERIFY(m.size() == 2);

        m.remove(ItemID(1, 1));
        QVERIFY(m.size() == 1);

        m.erase(it);
        QVERIFY(m.empty());
    }
}
