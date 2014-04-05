#ifndef TEST_LINES_H
#define TEST_LINES_H

#include <QObject>

class TestLines: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestLines() {}

private slots:

    void testCount();
    void testVisibility();
    void testSizes();
    void testAbsVsVis();
    void testSizeAtLine();
};

#endif // TEST_LINES_H
