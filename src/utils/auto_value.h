#ifndef AUTO_VALUE_H
#define AUTO_VALUE_H

template <typename T>
struct auto_value
{
    auto_value(T& value, T newValue)
        : m_value(value),
          m_oldValue(value)
    {
        m_value = newValue;
    }

    ~auto_value()
    {
        m_value = m_oldValue;
    }

private:
    T& m_value;
    T m_oldValue;
};

#endif // AUTO_VALUE_H
