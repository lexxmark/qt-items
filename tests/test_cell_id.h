#ifndef TEST_CELL_ID_H
#define TEST_CELL_ID_H

#include <QObject>

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

#endif // TEST_CELL_ID_H
