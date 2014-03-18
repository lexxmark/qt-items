#ifndef TEST_RANGES_H
#define TEST_RANGES_H

#include <QObject>

class TestRanges: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestRanges() {}

private slots:

    void testRangeNone();
    void testRangeAll();
    void testRangeColumn();
    void testRangeColumns();
    void testRangeRow();
    void testRangeRows();
};

#endif // TEST_RANGES_H
