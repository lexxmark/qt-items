#ifndef TEST_CELL_ID_H
#define TEST_CELL_ID_H

#include "utils/CellID.h"
#include <QObject>
#include <QtTest/QtTest>

class TestCellID: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestCellID() {}
    
private slots:

    void testClass();
    void testSet();
    void testMap();
};

namespace QTest {
    template<>
    inline char *toString(const Qi::CellID &cell)
    {
        QByteArray ba = "CellID(";
        ba += QByteArray::number(cell.row) + ", " + QByteArray::number(cell.column);
        ba += ")";
        return qstrdup(ba.data());
    }
}

#endif // TEST_CELL_ID_H
