/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef QI_ITEM_ID_H
#define QI_ITEM_ID_H

#include "QiAPI.h"
#include <array>
#include <memory>

#if !defined(QT_NO_DEBUG)
    #define QI_CHECK_ID_TYPES
#endif

#if defined(QI_CHECK_ID_TYPES)
    #include <typeinfo>
#endif

namespace Qi
{

class QI_EXPORT NoID {};

class QI_EXPORT ID
{
public:
    ID()
    {
        Adopt(NoID());
    }

    template <typename T>
    explicit ID(const T& other)
    {
        Adopt(other);
    }


    ID(const ID& other)
        : m_data(other.m_data)
#if defined(QI_CHECK_ID_TYPES)
          , m_typeHashCode(other.m_typeHashCode)
          , m_typeName(other.m_typeName)
#endif
    {
    }


    ID(ID&& other)
        : m_data(std::move(other.m_data))
#if defined(QI_CHECK_ID_TYPES)
          , m_typeHashCode(std::move(other.m_typeHashCode))
          , m_typeName(std::move(other.m_typeName))
#endif
    {
    }

    template <typename T>
    ID& operator=(const T& other)
    {
        Adopt(other);
        return *this;
    }

    ID& operator=(const ID& other)
    {
        CheckIDTypes(other);

        if (this != &other)
        {
            m_data = other.m_data;

#if defined(QI_CHECK_ID_TYPES)
            m_typeHashCode = other.m_typeHashCode;
            m_typeName= other.m_typeName;
#endif
        }
        return *this;
    }

    ID& operator=(ID&& other)
    {
        CheckIDTypes(other);

        if (this != &other)
        {
            m_data = std::move(other.m_data);

#if defined(QI_CHECK_ID_TYPES)
            m_typeHashCode = std::move(other.m_typeHashCode);
            m_typeName = std::move(other.m_typeName);
#endif
        }
        return *this;
    }

    template <typename T>
    T& as()
    {
        CheckType<T>();
        return *reinterpret_cast<T*>(&m_data.front());
    }

    template <typename T>
    const T& as() const
    {
        CheckType<T>();
        return *reinterpret_cast<const T*>(&m_data.front());
    }

    bool operator==(const ID& other) const
    {
        CheckIDTypes(other);
        return m_data == other.m_data;
    }

    bool operator!=(const ID& other) const
    {
        CheckIDTypes(other);
        return m_data != other.m_data;
    }

protected:
    template <typename T>
    void CheckType() const
    {
        static_assert(sizeof(T) <= sizeof(ID), "Should fit in size");
        static_assert(std::is_standard_layout<T>::value, "Should be standard layout");
        static_assert(std::is_trivially_copyable<T>::value, "Should be trivial copyable");
        static_assert(std::is_trivially_destructible<T>::value, "Should be trivial destructable");
#if defined(QI_CHECK_ID_TYPES)
        Q_ASSERT(!m_typeHashCode || m_typeHashCode == typeid(T).hash_code());
#endif
    }

    void CheckIDTypes(const ID& other) const
    {
        Q_UNUSED(other);
#if defined(QI_CHECK_ID_TYPES)
        Q_ASSERT(!m_typeHashCode || !other.m_typeHashCode || m_typeHashCode == other.m_typeHashCode);
#endif
    }

    template <typename T>
    void Adopt(const T& other)
    {
        CheckType<T>();
        m_data.fill(0);
        std::memcpy(&m_data.front(), &other, sizeof(T));

#if defined(QI_CHECK_ID_TYPES)
        if (typeid(T).hash_code() != typeid(NoID).hash_code())
        {
            m_typeHashCode = typeid(T).hash_code();
            m_typeName = typeid(T).name();
        }
        else
        {
            m_typeHashCode = 0;
            m_typeName = nullptr;
        }
#endif
    }

private:
    std::array<qintptr, 3> m_data;

#if defined(QI_CHECK_ID_TYPES)
    size_t m_typeHashCode = 0;
    const char* m_typeName = nullptr;
#endif
};

inline int index(ID id)
{
    return id.as<int>();
}

template <typename T, typename... Args>
inline ID makeID(Args&&... args)
{
    return ID(T(std::forward<Args>(args)...));
}

template <typename T>
bool operator==(ID left, const T& right)
{
    return left.as<T>() == right;
}

template <typename T>
bool operator==(const T& left, ID right)
{
    return left == right.as<T>();
}

template <typename T>
bool operator!=(ID left, const T& right)
{
    return left.as<T>() != right;
}

template <typename T>
bool operator!=(const T& left, ID right)
{
    return left != right.as<T>();
}

} // end namespace Qi

#endif // QI_ITEM_ID_H
