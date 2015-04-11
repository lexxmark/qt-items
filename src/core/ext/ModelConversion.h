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
    ModelConversion(const QSharedPointer<ModelTyped<Source_t>>& sourceModel, bool compareBySource = true)
        : m_sourceModel(sourceModel),
          m_compareBySource(compareBySource)
    {
        Q_ASSERT(m_sourceModel);
    }

    std::function<Target_t(Source_t)> s2tFunction;
    std::function<Source_t(Target_t)> t2sFunction;

protected:
    int compareImpl(const ItemID& left, const ItemID& right) const override
    {
        if (m_compareBySource)
            return m_sourceModel->compare(left, right);
        else
            return ModelTyped<Target_t>::compareImpl(left, right);
    }

    bool isAscendingDefaultImpl(const ItemID& item) const override
    {
        if (m_compareBySource)
            return m_sourceModel->isAscendingDefault(item);
        else
            return ModelTyped<Target_t>::isAscendingDefaultImpl(item);
    }

    Target_t valueImpl(const ItemID& item) const override
    {
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
    bool m_compareBySource;
};

} // end namespace Qi

#endif // QI_MODEL_CONVERSION_H
