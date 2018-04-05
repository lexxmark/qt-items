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

#ifndef QI_ENUM_H
#define QI_ENUM_H

#include <QSet>
#include "items/text/Text.h"

namespace Qi
{

template <typename EnumType = int>
class EnumTraits
{
    Q_DISABLE_COPY(EnumTraits)

public:
    QVector<EnumType> uniqueValues() const { return uniqueValuesImpl(); }
    QString valueText(EnumType value) const { return valueTextImpl(value); }
    int compareValues(EnumType left, EnumType right) const
    {
        if (m_sortedUniqueValues.isEmpty())
        {
            m_sortedUniqueValues = uniqueValues();
            sortUniqueValuesImpl(m_sortedUniqueValues);
        }

        auto leftIndex = std::distance(m_sortedUniqueValues.begin(), std::lower_bound(m_sortedUniqueValues.begin(),
                                                                                      m_sortedUniqueValues.end(),
                                                                                      left));
        auto rightIndex = std::distance(m_sortedUniqueValues.begin(), std::lower_bound(m_sortedUniqueValues.begin(),
                                                                                       m_sortedUniqueValues.end(),
                                                                                       right));
        return Private::compareValues(leftIndex, rightIndex);
    }

protected:
    EnumTraits() {}
    virtual ~EnumTraits() {}

    virtual QVector<EnumType> uniqueValuesImpl() const = 0;
    virtual QString valueTextImpl(EnumType value) const = 0;
    virtual void sortUniqueValuesImpl(QVector<EnumType>& uniqueValues) const
    {
        std::sort(uniqueValues.begin(), uniqueValues.end(), [this](EnumType left, EnumType right)->bool {
            return valueText(left) < valueText(right);
        });
    }

private:
    mutable QVector<EnumType> m_sortedUniqueValues;
};

template <typename EnumType = int>
class ModelEnum: public ModelTyped<EnumType>
{
    Q_DISABLE_COPY(ModelEnum)

public:
    typedef typename ModelTyped<EnumType>::ValueType_t ValueType_t;

    explicit ModelEnum(SharedPtr<EnumTraits<EnumType>> enumTraits, SharedPtr<ModelTyped<EnumType>> enumValues, bool ascendingDefault = true)
        : m_enumTraits(std::move(enumTraits)),
          m_enumValues(std::move(enumValues))
    {
        Q_ASSERT(m_enumTraits && m_enumValues);
        ModelTyped<EnumType>::m_ascendingDefault = ascendingDefault;
    }

    const EnumTraits<EnumType>& enumTraits() const { return *m_enumTraits; }

protected:
    int compareImpl(ID left, ID right) const override
    {
        // compare enum values
        return m_enumTraits->compareValues(m_enumValues->value(left), m_enumValues->value(right));
    }

    ValueType_t valueImpl(ID id) const override
    {
        return m_enumValues->value(id);
    }

    bool setValueImpl(ID id, ValueType_t value) override
    {
        return m_enumValues->setValue(id, value);
    }

    bool setValueMultipleImpl(IdIterator& itemsIterator, ValueType_t value) override
    {
        return m_enumValues->setValueMultiple(itemsIterator, value);
    }

private:
    SharedPtr<EnumTraits<EnumType>> m_enumTraits;
    SharedPtr<ModelTyped<EnumType>> m_enumValues;
};


template <typename EnumType = int>
class ModelEnumText: public ModelTyped<QString>
{
    Q_DISABLE_COPY(ModelEnumText)

public:
    explicit ModelEnumText(SharedPtr<ModelEnum<EnumType>> modelEnum)
        : m_modelEnum(std::move(modelEnum))
    {
        Q_ASSERT(m_modelEnum);
    }

protected:
    int compareImpl(ID left, ID right) const override
    {
        return m_modelEnum->compare(left, right);
    }
    bool isAscendingDefaultImpl(ID id) const override
    {
        return m_modelEnum->isAscendingDefault(id);
    }

    ValueType_t valueImpl(ID id) const override
    {
        return m_modelEnum->enumTraits().valueText(m_modelEnum->value(id));
    }

    bool setValueImpl(ID id, ValueType_t value) override
    {
        validateText2Enum();
        return m_modelEnum->setValue(id, m_text2Enum[value]);
    }

    bool setValueMultipleImpl(IdIterator& itemsIterator, ValueType_t value) override
    {
        validateText2Enum();
        return m_modelEnum->setValueMultiple(itemsIterator, m_text2Enum[value]);
    }

private:
    void validateText2Enum()
    {
        if (!m_text2Enum.isEmpty())
            return;

        for (auto enumValue : m_modelEnum->enumTraits().uniqueValues())
            m_text2Enum[m_modelEnum->enumTraits().valueText(enumValue)] = enumValue;
    }

    SharedPtr<ModelEnum<EnumType>> m_modelEnum;
    QMap<QString, EnumType> m_text2Enum;
};

template <typename EnumType = int>
class ViewEnumText: public ViewText
{
    Q_DISABLE_COPY(ViewEnumText)

public:
    ViewEnumText(SharedPtr<ModelEnum<EnumType>> model, ViewDefaultController createDefaultController = ViewDefaultControllerNone, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone)
        : ViewText(makeShared<ModelEnumText<EnumType>>(model), createDefaultController, alignment, textElideMode),
          m_model(std::move(model))
    {
    }

protected:
    QSize sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const override
    {
        EnumType value = m_model->value(id);
        auto it = m_sizes.find(value);
        if (it != m_sizes.end())
            return it.value();

        return m_sizes[value] = ViewText::sizeImpl(ctx, id, sizeMode);
    }

private:
    SharedPtr<ModelEnum<EnumType>> m_model;
    mutable QMap<EnumType, QSize> m_sizes;
};

} // end namespace Qi

#endif // QI_ENUM_H
