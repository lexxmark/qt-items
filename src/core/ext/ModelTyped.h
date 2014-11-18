#ifndef QI_MODEL_TYPED_H
#define QI_MODEL_TYPED_H

#include "core/Model.h"
#include "core/ItemsIterator.h"

namespace Qi
{

namespace Private
{
    template<typename T> int compareValues(const T& leftValue, const T& rightValue)
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
template <typename T> class ModelTyped : public ModelComparable
{
protected:
    ModelTyped():
         m_ascendingDefault(true)
    {}

public:
    typedef T ValueType_t;

    ValueType_t value(const ItemID& item) const { return valueImpl(item); }
    ValueType_t value(quint32 row, quint32 column) const { return value(ItemID(row, column)); }

    bool setValue(const ItemID& item, ValueType_t value)
    {
        if (setValueImpl(item, value))
        {
            emit modelChanged(this);
            return true;
        }
        return false;
    }
    bool setValue(quint32 row, quint32 column, ValueType_t value) { return setValue(ItemID(row, column), value); }

    bool setValueMultiple(ItemsIterator& itemsIterator, ValueType_t value)
    {
        if (setValueMultipleImpl(itemsIterator, value))
        {
            emit modelChanged(this);
            return true;
        }
        return false;
    }

protected:
    int compareImpl(const ItemID &left, const ItemID &right) const override { return Private::compareValues(value(left), value(right)); }
    bool isAscendingDefaultImpl(const ItemID &item) const override { return m_ascendingDefault; }

    virtual ValueType_t valueImpl(const ItemID& item) const = 0;
    virtual bool setValueImpl(const ItemID& item, ValueType_t value) = 0;
    virtual bool setValueMultipleImpl(ItemsIterator& itemsIterator, ValueType_t value)
    {
        bool result = false;

        for (itemsIterator.atFirst(); itemsIterator.isValid(); itemsIterator.toNext())
        {
            bool itemResult = setValueImpl(itemsIterator.item(), value);
            result |= itemResult;
        }

        return result;
    }

    bool m_ascendingDefault;
};

// export already specialized ModelTyped classes
QI_EXPORT_TEMPLATE template class QI_EXPORT ModelTyped<quint32>;
QI_EXPORT_TEMPLATE template class QI_EXPORT ModelTyped<int>;

} // end namespace Qi

#endif // QI_MODEL_TYPED_H
