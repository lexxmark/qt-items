#include "test_lines.h"
#include "utils/Lines.h"
#include "SignalSpy.h"
#include <QtTest/QtTest>

using namespace Qi;

void TestLines::testCount()
{
    Lines lines;

    lines.setCount(10);
    QCOMPARE(lines.count(), 10u);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);
    
    lines.setCount(92);
    QCOMPARE(lines.count(), 92u);
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCount|ChangeReasonLinesCountWeak);

    lines.setCount(92);
    QCOMPARE(lines.count(), 92u);
    QCOMPARE(signalSpy.size(), 2);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCountWeak);

    lines.setCount(0);
    QCOMPARE(lines.count(), 0u);
    QCOMPARE(signalSpy.size(), 3);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCount|ChangeReasonLinesCountWeak);

    signalSpy.disconnect();
    lines.setCount(3);
    QCOMPARE(lines.count(), 3u);
    QCOMPARE(signalSpy.size(), 3);
}
    
void TestLines::testVisibility()
{
    Lines lines;
    lines.setCount(10);
    
    QCOMPARE(lines.isLineVisible(0), true);
    QCOMPARE(lines.isLineVisible(5), true);
    QCOMPARE(lines.isLineVisible(9), true);

    lines.setLineVisible(0, false);
    QCOMPARE(lines.isLineVisible(0), false);
    QCOMPARE(lines.isLineVisible(1), true);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);

    lines.setLineVisible(1, false);
    QCOMPARE(lines.isLineVisible(0), false);
    QCOMPARE(lines.isLineVisible(1), false);
    QCOMPARE(lines.isLineVisible(2), true);
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineVisibility);
    QCOMPARE(signalSpy.getLast<0>(), &lines);

    lines.setAllLinesVisible(false);
    QCOMPARE(lines.isLineVisible(0), false);
    QCOMPARE(lines.isLineVisible(1), false);
    QCOMPARE(lines.isLineVisible(2), false);
    QCOMPARE(lines.isLineVisible(9), false);
    QCOMPARE(signalSpy.size(), 2);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineVisibility);

    lines.setAllLinesVisible(true);
    QCOMPARE(lines.isLineVisible(0), true);
    QCOMPARE(lines.isLineVisible(1), true);
    QCOMPARE(lines.isLineVisible(2), true);
    QCOMPARE(lines.isLineVisible(9), true);
    QCOMPARE(signalSpy.size(), 3);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineVisibility);

    lines.setLineVisible(2, true);
    QCOMPARE(lines.isLineVisible(2), true);
    QCOMPARE(signalSpy.size(), 3);

    lines.setLineVisible(2, false);
    QCOMPARE(lines.isLineVisible(2), false);
    QCOMPARE(signalSpy.size(), 4);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineVisibility);

    lines.setCount(5);
    QCOMPARE(lines.count(), 5u);
    QCOMPARE(lines.isLineVisible(2), false);
    
    lines.setCount(20);
    QCOMPARE(lines.isLineVisible(19), true);
}

void TestLines::testSizes()
{
    Lines lines;
    lines.setCount(10);
    
    quint32 defaultLineSize = lines.lineSize(0);

    QCOMPARE(lines.lineSize(5), defaultLineSize);
    QCOMPARE(lines.lineSize(9), defaultLineSize);

    lines.setLineSize(0, 16);
    QCOMPARE(lines.lineSize(0), 16u);
    QCOMPARE(lines.lineSize(1), defaultLineSize);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);

    lines.setLineSize(1, 28);
    QCOMPARE(lines.lineSize(0), 16u);
    QCOMPARE(lines.lineSize(1), 28u);
    QCOMPARE(lines.lineSize(2), defaultLineSize);
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineSize);

    lines.setAllLinesSize(15);
    QCOMPARE(lines.lineSize(0), 15u);
    QCOMPARE(lines.lineSize(1), 15u);
    QCOMPARE(lines.lineSize(2), 15u);
    QCOMPARE(lines.lineSize(9), 15u);
    QCOMPARE(signalSpy.size(), 2);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineSize);

    lines.setAllLinesSize(4);
    QCOMPARE(lines.lineSize(0), 4u);
    QCOMPARE(lines.lineSize(1), 4u);
    QCOMPARE(lines.lineSize(2), 4u);
    QCOMPARE(lines.lineSize(9), 4u);
    QCOMPARE(signalSpy.size(), 3);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineSize);

    lines.setLineSize(2, 4);
    QCOMPARE(lines.lineSize(2), 4u);
    QCOMPARE(signalSpy.size(), 3);

    lines.setLineSize(2, 8);
    QCOMPARE(lines.lineSize(2), 8u);
    QCOMPARE(signalSpy.size(), 4);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineSize);

    lines.setCount(5);
    QCOMPARE(lines.count(), 5u);
    QCOMPARE(lines.lineSize(2), 8u);
    
    lines.setCount(20);
    QCOMPARE(lines.lineSize(19), defaultLineSize);
}

void TestLines::testAbsVsVis()
{
    Lines lines;
    lines.setCount(10);
    
    QCOMPARE(lines.toVisible(0), 0u);
    QCOMPARE(lines.toVisible(1), 1u);
    QCOMPARE(lines.toVisible(2), 2u);
    QCOMPARE(lines.toVisible(3), 3u);
    QCOMPARE(lines.toVisible(4), 4u);
    QCOMPARE(lines.toVisible(5), 5u);
    QCOMPARE(lines.toVisible(6), 6u);
    QCOMPARE(lines.toVisible(7), 7u);
    QCOMPARE(lines.toVisible(8), 8u);
    QCOMPARE(lines.toVisible(9), 9u);

    QCOMPARE(lines.visibleCount(), 10u);

    QCOMPARE(lines.toAbsolute(0), 0u);
    QCOMPARE(lines.toAbsolute(1), 1u);
    QCOMPARE(lines.toAbsolute(2), 2u);
    QCOMPARE(lines.toAbsolute(3), 3u);
    QCOMPARE(lines.toAbsolute(4), 4u);
    QCOMPARE(lines.toAbsolute(5), 5u);
    QCOMPARE(lines.toAbsolute(6), 6u);
    QCOMPARE(lines.toAbsolute(7), 7u);
    QCOMPARE(lines.toAbsolute(8), 8u);
    QCOMPARE(lines.toAbsolute(9), 9u);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);

    std::vector<quint32> permutation(10);
    permutation[0] = 3;
    permutation[1] = 8;
    permutation[2] = 1;
    permutation[3] = 6;
    permutation[4] = 4;
    permutation[5] = 9;
    permutation[6] = 2;
    permutation[7] = 0;
    permutation[8] = 5;
    permutation[9] = 7;
    
    lines.setPermutation(permutation);
    
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLineOrder);

    QCOMPARE(lines.toVisible(0), 3u);
    QCOMPARE(lines.toVisible(1), 8u);
    QCOMPARE(lines.toVisible(2), 1u);
    QCOMPARE(lines.toVisible(3), 6u);
    QCOMPARE(lines.toVisible(4), 4u);
    QCOMPARE(lines.toVisible(5), 9u);
    QCOMPARE(lines.toVisible(6), 2u);
    QCOMPARE(lines.toVisible(7), 0u);
    QCOMPARE(lines.toVisible(8), 5u);
    QCOMPARE(lines.toVisible(9), 7u);

    QCOMPARE(lines.visibleCount(), 10u);

    QCOMPARE(lines.toAbsolute(0), 7u);
    QCOMPARE(lines.toAbsolute(1), 2u);
    QCOMPARE(lines.toAbsolute(2), 6u);
    QCOMPARE(lines.toAbsolute(3), 0u);
    QCOMPARE(lines.toAbsolute(4), 4u);
    QCOMPARE(lines.toAbsolute(5), 8u);
    QCOMPARE(lines.toAbsolute(6), 3u);
    QCOMPARE(lines.toAbsolute(7), 9u);
    QCOMPARE(lines.toAbsolute(8), 1u);
    QCOMPARE(lines.toAbsolute(9), 5u);
    
    lines.setLineVisible(1, false);
    lines.setLineVisible(2, false);
    lines.setLineVisible(5, false);
    lines.setLineVisible(9, false);
    
    QCOMPARE(lines.visibleCount(), 6u);
    
    QCOMPARE(lines.toVisible(0), 2u);
    QCOMPARE(lines.toVisible(1), Invalid);
    QCOMPARE(lines.toVisible(2), Invalid);
    QCOMPARE(lines.toVisible(3), 5u);
    QCOMPARE(lines.toVisible(4), 3u);
    QCOMPARE(lines.toVisible(5), Invalid);
    QCOMPARE(lines.toVisible(6), 1u);
    QCOMPARE(lines.toVisible(7), 0u);
    QCOMPARE(lines.toVisible(8), 4u);
    QCOMPARE(lines.toVisible(9), Invalid);

    QCOMPARE(lines.toAbsolute(0), 7u);
    QCOMPARE(lines.toAbsolute(1), 6u);
    QCOMPARE(lines.toAbsolute(2), 0u);
    QCOMPARE(lines.toAbsolute(3), 4u);
    QCOMPARE(lines.toAbsolute(4), 8u);
    QCOMPARE(lines.toAbsolute(5), 3u);
}

void TestLines::testSizeAtLine()
{
    Lines lines;
    lines.setCount(10);
    
    lines.setAllLinesSize(10);
    
    QCOMPARE(lines.sizeAtVisLine(0), 10u);
    QCOMPARE(lines.sizeAtVisLine(1), 20u);
    QCOMPARE(lines.sizeAtVisLine(2), 30u);
    QCOMPARE(lines.sizeAtVisLine(3), 40u);
    QCOMPARE(lines.sizeAtVisLine(4), 50u);
    QCOMPARE(lines.sizeAtVisLine(5), 60u);
    QCOMPARE(lines.sizeAtVisLine(6), 70u);
    QCOMPARE(lines.sizeAtVisLine(7), 80u);
    QCOMPARE(lines.sizeAtVisLine(8), 90u);
    QCOMPARE(lines.sizeAtVisLine(9), 100u);
    
    QCOMPARE(lines.findVisLine(0), 0u);
    QCOMPARE(lines.findVisLine(10), 1u);
    QCOMPARE(lines.findVisLine(37), 3u);
    
    QCOMPARE(lines.visibleSize(), 100u);
    
    lines.setLineVisible(0, false);
    lines.setLineVisible(2, false);
    lines.setLineVisible(3, false);
    lines.setLineVisible(5, false);
    
    std::vector<quint32> permutation(10);
    permutation[0] = 6;
    permutation[1] = 4;
    permutation[2] = 1;
    permutation[3] = 5;
    permutation[4] = 0;
    permutation[5] = 2;
    permutation[6] = 9;
    permutation[7] = 3;
    permutation[8] = 7;
    permutation[9] = 8;
    
    lines.setPermutation(permutation);
    
    lines.setLineSize(4, 30);
    lines.setLineSize(7, 20);
    lines.setLineSize(1, 0);
    lines.setLineSize(8, 8);
    lines.setLineSize(9, 1);
    lines.setLineSize(6, 3);
    
    QCOMPARE(lines.sizeAtVisLine(0), 30u);
    QCOMPARE(lines.sizeAtVisLine(1), 50u);
    QCOMPARE(lines.sizeAtVisLine(2), 50u);
    QCOMPARE(lines.sizeAtVisLine(3), 58u);
    QCOMPARE(lines.sizeAtVisLine(4), 59u);
    QCOMPARE(lines.sizeAtVisLine(5), 62u);
    
    QCOMPARE(lines.findVisLine(0), 0u);
    QCOMPARE(lines.findVisLine(10), 0u);
    QCOMPARE(lines.findVisLine(30), 1u);
    QCOMPARE(lines.findVisLine(40), 1u);
    QCOMPARE(lines.findVisLine(50), 3u);
    QCOMPARE(lines.findVisLine(55), 3u);
    QCOMPARE(lines.findVisLine(58), 4u);
    QCOMPARE(lines.findVisLine(59), 5u);
    QCOMPARE(lines.findVisLine(60), 5u);
    QCOMPARE(lines.findVisLine(62), 5u);
    QCOMPARE(lines.findVisLine(159), 5u);
    
    QCOMPARE(lines.visibleSize(), 62u);
}

