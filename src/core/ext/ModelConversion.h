#ifndef QI_MODEL_CONVERSION_H
#define QI_MODEL_CONVERSION_H

#include "ModelTyped.h"
#include <functional>

namespace Qi
{

template <typename Source_t, typename Target_t>
class ModelConversion: public ModelTyped<Target_t>
{
public:
    ModelConversion(const QSharedPointer<ModelTyped<Source_t>>& sourceModel)
        : m_sourceModel(sourceModel)
    {
        Q_ASSERT(m_sourceModel);
    }

    std::function<Target_t(Source_t)> s2tFunction;
    std::function<Source_t(Target_t)> t2sFunction;

protected:
    Target_t valueImpl(const ItemID& item) const override
    {
        if (!s2tFunction)
            throw std::logic_error("Conversion delegate is not set");

        return s2tFunction(m_sourceModel->value(item));
    }

    bool setValueImpl(const ItemID& item, Target_t value) override
    {
        if (t2sFunction)
            return m_sourceModel->setValue(item, t2sFunction(value));
        else
            return false;
    }

    bool setValueMultipleImpl(ItemsIterator& itemsIterator, ValueType_t value) override
    {
        if (t2sFunction)
            return m_sourceModel->setValueMultiple(itemsIterator, t2sFunction(value));
        else
            return false;
    }

private:
    QSharedPointer<ModelTyped<Source_t>> m_sourceModel;
};

} // end namespace Qi

#endif // QI_MODEL_CONVERSION_H
