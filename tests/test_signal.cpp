#include "test_signal.h"
#include "utils/Signal.h"
#include <QtTest/QtTest>

using namespace Qi;

struct Receiver
{
    Receiver()
        : received(false)
    {}

    void onRecieve()
    {
        received = true;
    }

    bool received;
};

static bool s_received = false;
static void onStaticReceive()
{
    s_received = true;
}

void TestSignal::testVoid()
{
    SignalVoid s;

    {
        bool lambdaCalled = false;
        s.connect([&lambdaCalled]() { lambdaCalled = true; }, 0);
        QVERIFY(!lambdaCalled);

        s.doEmit();
        QVERIFY(lambdaCalled);
        s.disconnect(0);
        lambdaCalled = false;
        s.doEmit();
        QVERIFY(!lambdaCalled);
    }

    {
        Receiver r;
        s.connectSlot(&r, &Receiver::onRecieve);
        QVERIFY(!r.received);

        s.doEmit();
        QVERIFY(r.received);
        s.disconnectSlot(&r);

        r.received = false;
        s.doEmit();
        QVERIFY(!r.received);
    }

    {
        s.connectStatic(&onStaticReceive);
        QVERIFY(!s_received);

        s.doEmit();
        QVERIFY(s_received);
        s.disconnectAll();

        s_received = false;
        s.doEmit();
        QVERIFY(!s_received);
    }

    {
        bool lambdaCalled = false;
        s.connect([&lambdaCalled]() { lambdaCalled = true; }, 0);
        QVERIFY(!lambdaCalled);

        Receiver r;
        s.connectSlot(&r, &Receiver::onRecieve);
        QVERIFY(!r.received);

        s.connectStatic(&onStaticReceive);
        QVERIFY(!s_received);

        s.doEmit();

        QVERIFY(lambdaCalled);
        QVERIFY(r.received);
        QVERIFY(s_received);

        lambdaCalled = r.received = s_received = false;

        s.disconnectStatic(&onStaticReceive);
        s.doEmit();

        QVERIFY(lambdaCalled);
        QVERIFY(r.received);
        QVERIFY(!s_received);

        lambdaCalled = r.received = s_received = false;

        s.disconnectSlot(&r);
        s.doEmit();

        QVERIFY(lambdaCalled);
        QVERIFY(!r.received);
        QVERIFY(!s_received);

        lambdaCalled = r.received = s_received = false;

        s.disconnectById(0);
        s.doEmit();

        QVERIFY(!lambdaCalled);
        QVERIFY(!r.received);
        QVERIFY(!s_received);
    }
}

void TestSignal::testConnectDisconnect()
{
    Signal<void(int)> s;
    int value = 0;
    quint32 c = s.connect([&value](int v) { value = v; });

    s.doEmit(10);
    QCOMPARE(value, 10);

    QVERIFY(s.disconnect(c));
    s.doEmit(11);
    QCOMPARE(value, 10);

    int value2 = 0;
    quint32 c2 = s.connect([&value2](int v) { value2 = v; });
    c = s.connect([&value](int v) { value = v; });

    s.doEmit(12);
    QCOMPARE(value, 12);
    QCOMPARE(value2, 12);

    QVERIFY(s.disconnect(c));
    s.doEmit(13);
    QCOMPARE(value, 12);
    QCOMPARE(value2, 13);

    QVERIFY(s.disconnect(c2));
    s.doEmit(14);
    QCOMPARE(value, 12);
    QCOMPARE(value2, 13);
}

static int static_sum = 0;

static void OnSignal(const std::string& /*str*/, float /*val*/, MyObj& obj)
{
    static_sum += obj.value;
}

void MyObj::onSignal(const std::string& /*str*/, float /*val*/, MyObj& obj) const
{
    static_sum += obj.value;
}

void MyQObject::mySlot(const std::string& /*str*/, float /*val*/, MyObj& obj)
{
    static_sum += obj.value;
}

#ifdef ENABLE_SIGNAL_BENCHMARK

void TestSignal::testConnectLambdaBenchmark()
{
    Signal<void(const std::string&, float, MyObj&)> s;

    QBENCHMARK
    {
        s.connect([](const std::string& str, float val, MyObj& obj) {
            static_sum += obj.value;
        });
    }

    s.disconnectAll();
}

void TestSignal::testConnectStaticBenchmark()
{
    Signal<void(const std::string&, float, MyObj&)> s;

    QBENCHMARK
    {
        s.connectStatic(&OnSignal);
    }

    s.disconnectAll();
}

void TestSignal::testConnectSlotBenchmark()
{
    Signal<void(const std::string&, float, MyObj&)> s;
    MyObj obj;

    QBENCHMARK
    {
        s.connectSlot(&obj, &MyObj::onSignal);
    }

    s.disconnectAll();
}

void TestSignal::testConnectQObjectBenchmark()
{
    QObject obj1;
    QObject obj2;

    QBENCHMARK
    {
        QObject::connect(&obj1, SIGNAL(destroyed()), &obj2, SLOT(deleteLater()));
    }
}


void TestSignal::testEmitBenchmark()
{
    Signal<void(const std::string&, float, MyObj&)> s;
    MyObj obj;
    obj.value = 1;
    std::string txt("Hello");

    for (quint32 i = 0; i < 100; ++i)
    {
        s.connect([](const std::string& str, float val, MyObj& obj) {
            static_sum += obj.value;
        });
    }

    for (quint32 i = 0; i < 100; ++i)
    {
        s.connectStatic(&OnSignal);
    }

    for (quint32 i = 0; i < 100; ++i)
    {
        s.connectSlot(&obj, &MyObj::onSignal);

    }

    QBENCHMARK
    {
        s.doEmit(txt, 2.f, obj);
    }

    s.disconnectAll();
}

void TestSignal::testEmitQObjectBenchmark()
{
    MyQObject obj1;
    MyQObject obj2;
    MyObj obj;
    obj.value = 1;
    std::string txt("Hello");


    for (quint32 i = 0; i < 300; ++i)
    {
        QObject::connect(&obj1, SIGNAL(mySignal(std::string,float,MyObj&)), &obj2, SLOT(mySlot(std::string,float,MyObj&)));
    }

    QBENCHMARK
    {
        obj1.invoke(txt, 2.f, obj);
    }
}
#endif
