#include "test_signal.h"
#include "test_item_id.h"
#include "test_ranges.h"
#include "test_lines.h"
#include "test_grid.h"

#include <QtTest/QtTest>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    int result = 0;

    QList<const QMetaObject*> tests;

    // register tests
    tests.append(&TestSignal::staticMetaObject);
    tests.append(&TestItemID::staticMetaObject);
    tests.append(&TestRanges::staticMetaObject);
    tests.append(&TestLines::staticMetaObject);
    tests.append(&TestGrid::staticMetaObject);

    // run tests
    foreach (const QMetaObject* testMetaObject, tests)
    {
        QScopedPointer<QObject> test(testMetaObject->newInstance());
        Q_ASSERT(test);

        if (test)
        {
            result |= QTest::qExec(test.data(), argc, argv);
        }
    }

    return result;
}

