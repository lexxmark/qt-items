#ifndef TEST_SIGNAL_H
#define TEST_SIGNAL_H

#include <QObject>

class TestSignal: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestSignal() {}

private slots:

    void testVoid();
    void testConnectDisconnect();
    
#ifdef ENABLE_SIGNAL_BENCHMARK
    void testConnectLambdaBenchmark();
    void testConnectStaticBenchmark();
    void testConnectSlotBenchmark();
    void testConnectQObjectBenchmark();
    void testEmitBenchmark();
    void testEmitQObjectBenchmark();
#endif
};

class MyObj
{
public:
    int value;

    MyObj()
        : value(0)
    {}

    void onSignal(const std::string& str, float val, MyObj& obj) const;
};

class MyQObject: public QObject
{
    Q_OBJECT

public:
    void invoke(const std::string& str, float val, MyObj& obj)
    {
        mySignal(str, val, obj);
    }

public slots:
    void mySlot(const std::string& str, float val, MyObj& obj);

signals:
    void mySignal(const std::string& str, float val, MyObj& obj);
};

#endif // TEST_SIGNAL_H
