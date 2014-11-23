#include "test_lines.h"
#include "space/Lines.h"
#include "SignalSpy.h"
#include <QtTest/QtTest>

using namespace Qi;

void TestLines::testCount()
{
    Lines lines;

    lines.setCount(10);
    QCOMPARE(lines.count(), 10);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);
    
    lines.setCount(92);
    QCOMPARE(lines.count(), 92);
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCount|ChangeReasonLinesCountWeak);

    lines.setCount(92);
    QCOMPARE(lines.count(), 92);
    QCOMPARE(signalSpy.size(), 2);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCountWeak);

    lines.setCount(0);
    QCOMPARE(lines.count(), 0);
    QCOMPARE(signalSpy.size(), 3);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesCount|ChangeReasonLinesCountWeak);

    signalSpy.disconnect();
    lines.setCount(3);
    QCOMPARE(lines.count(), 3);
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
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesVisibility);
    QCOMPARE(signalSpy.getLast<0>(), &lines);

    lines.setLineVisibleAll(false);
    QCOMPARE(lines.isLineVisible(0), false);
    QCOMPARE(lines.isLineVisible(1), false);
    QCOMPARE(lines.isLineVisible(2), false);
    QCOMPARE(lines.isLineVisible(9), false);
    QCOMPARE(signalSpy.size(), 2);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesVisibility);

    lines.setLineVisibleAll(true);
    QCOMPARE(lines.isLineVisible(0), true);
    QCOMPARE(lines.isLineVisible(1), true);
    QCOMPARE(lines.isLineVisible(2), true);
    QCOMPARE(lines.isLineVisible(9), true);
    QCOMPARE(signalSpy.size(), 3);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesVisibility);

    lines.setLineVisible(2, true);
    QCOMPARE(lines.isLineVisible(2), true);
    QCOMPARE(signalSpy.size(), 3);

    lines.setLineVisible(2, false);
    QCOMPARE(lines.isLineVisible(2), false);
    QCOMPARE(signalSpy.size(), 4);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesVisibility);

    lines.setCount(5);
    QCOMPARE(lines.count(), 5);
    QCOMPARE(lines.isLineVisible(2), false);
    
    lines.setCount(20);
    QCOMPARE(lines.isLineVisible(19), true);
}

void TestLines::testSizes()
{
    Lines lines;
    lines.setCount(10);
    
    int defaultLineSize = lines.lineSize(0);

    QCOMPARE(lines.lineSize(5), defaultLineSize);
    QCOMPARE(lines.lineSize(9), defaultLineSize);

    lines.setLineSize(0, 16);
    QCOMPARE(lines.lineSize(0), 16);
    QCOMPARE(lines.lineSize(1), defaultLineSize);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);

    lines.setLineSize(1, 28);
    QCOMPARE(lines.lineSize(0), 16);
    QCOMPARE(lines.lineSize(1), 28);
    QCOMPARE(lines.lineSize(2), defaultLineSize);
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesSize);

    lines.setLineSizeAll(15);
    QCOMPARE(lines.lineSize(0), 15);
    QCOMPARE(lines.lineSize(1), 15);
    QCOMPARE(lines.lineSize(2), 15);
    QCOMPARE(lines.lineSize(9), 15);
    QCOMPARE(signalSpy.size(), 2);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesSize);

    lines.setLineSizeAll(4);
    QCOMPARE(lines.lineSize(0), 4);
    QCOMPARE(lines.lineSize(1), 4);
    QCOMPARE(lines.lineSize(2), 4);
    QCOMPARE(lines.lineSize(9), 4);
    QCOMPARE(signalSpy.size(), 3);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesSize);

    lines.setLineSize(2, 4);
    QCOMPARE(lines.lineSize(2), 4);
    QCOMPARE(signalSpy.size(), 3);

    lines.setLineSize(2, 8);
    QCOMPARE(lines.lineSize(2), 8);
    QCOMPARE(signalSpy.size(), 4);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesSize);

    lines.setCount(5);
    QCOMPARE(lines.count(), 5);
    QCOMPARE(lines.lineSize(2), 8);
    
    lines.setCount(20);
    QCOMPARE(lines.lineSize(19), defaultLineSize);
}

void TestLines::testAbsVsVis()
{
    Lines lines;
    lines.setCount(10);
    
    QCOMPARE(lines.toVisible(0), 0);
    QCOMPARE(lines.toVisible(1), 1);
    QCOMPARE(lines.toVisible(2), 2);
    QCOMPARE(lines.toVisible(3), 3);
    QCOMPARE(lines.toVisible(4), 4);
    QCOMPARE(lines.toVisible(5), 5);
    QCOMPARE(lines.toVisible(6), 6);
    QCOMPARE(lines.toVisible(7), 7);
    QCOMPARE(lines.toVisible(8), 8);
    QCOMPARE(lines.toVisible(9), 9);

    QCOMPARE(lines.visibleCount(), 10);

    QCOMPARE(lines.toAbsolute(0), 0);
    QCOMPARE(lines.toAbsolute(1), 1);
    QCOMPARE(lines.toAbsolute(2), 2);
    QCOMPARE(lines.toAbsolute(3), 3);
    QCOMPARE(lines.toAbsolute(4), 4);
    QCOMPARE(lines.toAbsolute(5), 5);
    QCOMPARE(lines.toAbsolute(6), 6);
    QCOMPARE(lines.toAbsolute(7), 7);
    QCOMPARE(lines.toAbsolute(8), 8);
    QCOMPARE(lines.toAbsolute(9), 9);
    
    auto signalSpy = createSignalSpy(&lines, &Lines::linesChanged);

    QVector<int> permutation(10);
    /*
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
    */

    permutation[0] = 7;
    permutation[1] = 2;
    permutation[2] = 6;
    permutation[3] = 0;
    permutation[4] = 4;
    permutation[5] = 8;
    permutation[6] = 3;
    permutation[7] = 9;
    permutation[8] = 1;
    permutation[9] = 5;

    lines.setPermutation(permutation);
    
    QCOMPARE(signalSpy.size(), 1);
    QCOMPARE(signalSpy.getLast<1>(), ChangeReasonLinesOrder);

    QCOMPARE(lines.toVisible(0), 3);
    QCOMPARE(lines.toVisible(1), 8);
    QCOMPARE(lines.toVisible(2), 1);
    QCOMPARE(lines.toVisible(3), 6);
    QCOMPARE(lines.toVisible(4), 4);
    QCOMPARE(lines.toVisible(5), 9);
    QCOMPARE(lines.toVisible(6), 2);
    QCOMPARE(lines.toVisible(7), 0);
    QCOMPARE(lines.toVisible(8), 5);
    QCOMPARE(lines.toVisible(9), 7);

    QCOMPARE(lines.visibleCount(), 10);

    QCOMPARE(lines.toAbsolute(0), 7);
    QCOMPARE(lines.toAbsolute(1), 2);
    QCOMPARE(lines.toAbsolute(2), 6);
    QCOMPARE(lines.toAbsolute(3), 0);
    QCOMPARE(lines.toAbsolute(4), 4);
    QCOMPARE(lines.toAbsolute(5), 8);
    QCOMPARE(lines.toAbsolute(6), 3);
    QCOMPARE(lines.toAbsolute(7), 9);
    QCOMPARE(lines.toAbsolute(8), 1);
    QCOMPARE(lines.toAbsolute(9), 5);
    
    lines.setLineVisible(1, false);
    lines.setLineVisible(2, false);
    lines.setLineVisible(5, false);
    lines.setLineVisible(9, false);
    
    QCOMPARE(lines.visibleCount(), 6);
    
    QCOMPARE(lines.toVisible(0), 2);
    QCOMPARE(lines.toVisible(1), InvalidIndex);
    QCOMPARE(lines.toVisible(2), InvalidIndex);
    QCOMPARE(lines.toVisible(3), 5);
    QCOMPARE(lines.toVisible(4), 3);
    QCOMPARE(lines.toVisible(5), InvalidIndex);
    QCOMPARE(lines.toVisible(6), 1);
    QCOMPARE(lines.toVisible(7), 0);
    QCOMPARE(lines.toVisible(8), 4);
    QCOMPARE(lines.toVisible(9), InvalidIndex);

    QCOMPARE(lines.toAbsolute(0), 7);
    QCOMPARE(lines.toAbsolute(1), 6);
    QCOMPARE(lines.toAbsolute(2), 0);
    QCOMPARE(lines.toAbsolute(3), 4);
    QCOMPARE(lines.toAbsolute(4), 8);
    QCOMPARE(lines.toAbsolute(5), 3);
}

void TestLines::testSizeAtLine()
{
    Lines lines;
    lines.setCount(10);
    
    lines.setLineSizeAll(10);
    
    QCOMPARE(lines.startPos(0), 0);
    QCOMPARE(lines.startPos(1), 10);
    QCOMPARE(lines.startPos(2), 20);
    QCOMPARE(lines.startPos(3), 30);
    QCOMPARE(lines.startPos(4), 40);
    QCOMPARE(lines.startPos(5), 50);
    QCOMPARE(lines.startPos(6), 60);
    QCOMPARE(lines.startPos(7), 70);
    QCOMPARE(lines.startPos(8), 80);
    QCOMPARE(lines.startPos(9), 90);
    
    QCOMPARE(lines.findVisibleIDByPos(0), 0);
    QCOMPARE(lines.findVisibleIDByPos(10), 1);
    QCOMPARE(lines.findVisibleIDByPos(37), 3);
    
    QCOMPARE(lines.visibleSize(), 100);
    
    lines.setLineVisible(0, false);
    lines.setLineVisible(2, false);
    lines.setLineVisible(3, false);
    lines.setLineVisible(5, false);
    
    QVector<int> permutation(10);
    /*
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
    */
    permutation[0] = 4;
    permutation[1] = 2;
    permutation[2] = 5;
    permutation[3] = 7;
    permutation[4] = 1;
    permutation[5] = 3;
    permutation[6] = 0;
    permutation[7] = 8;
    permutation[8] = 9;
    permutation[9] = 6;

    lines.setPermutation(permutation);
    
    lines.setLineSize(4, 30);
    lines.setLineSize(7, 20);
    lines.setLineSize(1, 0);
    lines.setLineSize(8, 8);
    lines.setLineSize(9, 1);
    lines.setLineSize(6, 3);
    
    QCOMPARE(lines.startPos(1), 30);
    QCOMPARE(lines.startPos(2), 50);
    QCOMPARE(lines.startPos(3), 50);
    QCOMPARE(lines.startPos(4), 58);
    QCOMPARE(lines.startPos(5), 59);
    QCOMPARE(lines.startPos(6), 62);
    
    QCOMPARE(lines.findVisibleIDByPos(0), 0);
    QCOMPARE(lines.findVisibleIDByPos(10), 0);
    QCOMPARE(lines.findVisibleIDByPos(30), 1);
    QCOMPARE(lines.findVisibleIDByPos(40), 1);
    QCOMPARE(lines.findVisibleIDByPos(50), 3);
    QCOMPARE(lines.findVisibleIDByPos(55), 3);
    QCOMPARE(lines.findVisibleIDByPos(58), 4);
    QCOMPARE(lines.findVisibleIDByPos(59), 5);
    QCOMPARE(lines.findVisibleIDByPos(60), 5);
    QCOMPARE(lines.findVisibleIDByPos(62), 5);
    QCOMPARE(lines.findVisibleIDByPos(159), 5);
    
    QCOMPARE(lines.visibleSize(), 62);
}

