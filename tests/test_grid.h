#ifndef TEST_GRID_H
#define TEST_GRID_H

#include <QObject>

class TestGrid: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestGrid() {}

private slots:

    void test();
};

#endif // TEST_GRID_H
