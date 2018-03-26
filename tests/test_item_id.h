#ifndef TEST_ITEM_ID_H
#define TEST_ITEM_ID_H

#include <space/grid/GridID.h>
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
    inline char *toString(const Qi::GridID& id)
    {
        QByteArray ba = "ID(";
        ba += QByteArray::number(id.row) + ", " + QByteArray::number(id.column);
        ba += ")";
        return qstrdup(ba.data());
    }
}

#endif // TEST_ITEM_ID_H
