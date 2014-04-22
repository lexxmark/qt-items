#include <QObject>

#if defined (Q_COMPILER_VARIADIC_TEMPLATES)

template<typename T1, typename T2, typename... ParamT>
class SignalSpy
{
public :
    SignalSpy(T1* signalSource, void (T2::*Method)(ParamT...))
    {
        m_connection = QObject::connect(signalSource, Method, [this](ParamT... args) {
            m_calls.push_back(std::make_tuple(args...));
        });
    }

    SignalSpy(SignalSpy&& other)
    {
        *this = std::move(other);
    }

    ~SignalSpy()
    {
        disconnect();
    }

    SignalSpy& operator=(SignalSpy&& other)
    {
        m_calls = std::move(other.m_calls);
        m_connection = std::move(other.m_connection);
        return *this;
    }

    void disconnect()
    {
        QObject::disconnect(m_connection);
    }

    bool empty() const { return m_calls.empty(); }
    int size() const { return m_calls.size(); }
    const std::tuple<ParamT...>& last() const { return m_calls.last(); }
    template <size_t N>
    typename std::tuple_element<N, std::tuple<ParamT...>>::type getLast() const { return std::get<N>(last()); }

    QList<std::tuple<ParamT...>> m_calls;

private:
    QMetaObject::Connection m_connection;
};

template<typename T1, typename T2, typename... ParamT>
SignalSpy<T1, T2, ParamT...> createSignalSpy(T1* signalSource, void (T2::*Method)(ParamT...))
{
    return SignalSpy<T1, T2, ParamT...>(signalSource, Method);
}

#else

template<typename T1, typename T2, typename ParamT>
class SignalSpy
{
public :
    SignalSpy(T1* signalSource, void (T2::*Method)(ParamT))
    {
        QObject::connect(signalSource,
                         Method,
                         [this](ParamT arg)
        {
            m_calls.push_back(std::make_tuple(arg));
        });
    }

    SignalSpy(SignalSpy&& other)
    {
        *this = std::move(other);
    }

    ~SignalSpy()
    {
        QObject::disconnect(m_connection);
    }

    SignalSpy& operator=(SignalSpy&& other)
    {
        m_calls = std::move(other.m_calls);
        m_connection = std::move(other.m_connection);
        return *this;
    }

    QList<std::tuple<ParamT>> m_calls;

private:
    QMetaObject::Connection m_connection;
};

template<typename T1, typename T2, typename ParamT>
SignalSpy<T1, T2, ParamT> createSignalSpy(T1* signalSource, void (T2::*Method)(ParamT))
{
    return SignalSpy<T1, T2, ParamT>(signalSource, Method);
}

template<typename T1, typename T2, typename ParamT1, typename ParamT2>
class SignalSpy
{
public :
    SignalSpy(T1* signalSource, void (T2::*Method)(ParamT1, ParamT2))
    {
        QObject::connect(signalSource,
                         Method,
                         [this](ParamT1 arg1, ParamT2 arg2)
        {
            m_calls.push_back(std::make_tuple(arg1, arg2));
        });
    }

    ~SignalSpy()
    {
        QObject::disconnect(m_connection);
    }

    QList<std::tuple<ParamT1, ParamT2>> m_calls;

private:
    QMetaObject::Connection m_connection;
};

template<typename T1, typename T2, typename ParamT1, typename ParamT2>
SignalSpy<T1, T2, ParamT1, ParamT2> createSignalSpy(T1* signalSource, void (T2::*Method)(ParamT1, ParamT2))
{
    return SignalSpy<T1, T2, ParamT1, ParamT2>(signalSource, Method);
}

#endif
