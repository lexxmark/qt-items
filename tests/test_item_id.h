#ifndef TEST_ITEM_ID_H
#define TEST_ITEM_ID_H

#include "core/ItemID.h"
#include <QObject>
#include <QtTest/QtTest>

class TestItemID: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestItemID() {}
    
private slots:

    void testClass();
    void testSet();
    void testMap();
};

namespace QTest {
    template<>
    inline char *toString(const Qi::ItemID &item)
    {
        QByteArray ba = "ItemID(";
        ba += QByteArray::number(item.row) + ", " + QByteArray::number(item.column);
        ba += ")";
        return qstrdup(ba.data());
    }
}

#endif // TEST_ITEM_ID_H
