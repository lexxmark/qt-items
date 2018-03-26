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

#ifndef QI_MODEL_TYPED_H
#define QI_MODEL_TYPED_H

#include "core/Model.h"
#include "core/ItemsIterator.h"

namespace Qi
{

namespace Private
{
    template<typename T>
    int compareValues(const T& leftValue, const T& rightValue)
    {
        if (leftValue < rightValue)
            return -1;
        else if (rightValue < leftValue)
            return 1;
        else
            return 0;
    }
}

// typed Model - represents T values
template <typename T>
class ModelTyped : public ModelComparable
{
protected:
    ModelTyped():
         m_ascendingDefault(true)
    {}

public:
    typedef T ValueType_t;

    ValueType_t value(ID id) const { return valueImpl(id); }

    bool setValue(ID id, ValueType_t value)
    {
        if (setValueImpl(id, value))
        {
            emit modelChanged(this);
            return true;
        }
        return false;
    }

    bool setValueMultiple(IdIterator& itemsIterator, ValueType_t value)
    {
        if (setValueMultipleImpl(itemsIterator, value))
        {
            emit modelChanged(this);
            return true;
        }
        return false;
    }

protected:
    int compareImpl(ID left, ID right) const override { return Private::compareValues(value(left), value(right)); }
    bool isAscendingDefaultImpl(ID /*item*/) const override { return m_ascendingDefault; }

    virtual ValueType_t valueImpl(ID id) const = 0;
    virtual bool setValueImpl(ID id, ValueType_t value) = 0;
    virtual bool setValueMultipleImpl(IdIterator& it, ValueType_t value)
    {
        bool result = false;

        for (it.atFirst(); it.isValid(); it.toNext())
        {
            bool itemResult = setValueImpl(it.id(), value);
            result |= itemResult;
        }

        return result;
    }

    bool m_ascendingDefault;
};

template <typename T, typename ID_t>
class ModelIdTyped : public ModelTyped<T>
{
public:
    ValueType_t valueId(ID_t id) const { return valueIdImpl(id); }

    bool setValueId(ID_t id, ValueType_t value)
    {
        if (setValueIdImpl(id, value))
        {
            emit modelChanged(this);
            return true;
        }
        return false;
    }

protected:
    ModelIdTyped() = default;

    ValueType_t valueImpl(ID id) const final { return valueIdImpl(id.as<ID_t>()); }
    bool setValueImpl(ID id, ValueType_t value) final { return setValueIdImpl(id.as<ID_t>(), value); }

    virtual ValueType_t valueIdImpl(ID_t id) const = 0;
    virtual bool setValueIdImpl(ID_t id, ValueType_t value) = 0;
};

// export already specialized ModelTyped classes
//QI_EXPORT_TEMPLATE template class QI_EXPORT ModelTyped<int>;

} // end namespace Qi

#endif // QI_MODEL_TYPED_H
