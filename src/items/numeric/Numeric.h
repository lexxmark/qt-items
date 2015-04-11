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

#ifndef QI_NUMERIC_H
#define QI_NUMERIC_H

#include "core/ext/ModelTyped.h"
#include <limits>

namespace Qi
{

namespace Private
{
    template<typename T> QString numericToText(const T& value)
    {
        QString text;
        text.setNum(value);
        return text;
    }

    template <typename T> T textToNumeric(const QString& text);
    template<> inline short textToNumeric<short>(const QString& text) { return text.toShort(); }
    template<> inline ushort textToNumeric<ushort>(const QString& text) { return text.toUShort(); }
    template<> inline int textToNumeric<int>(const QString& text) { return text.toInt(); }
    template<> inline uint textToNumeric<uint>(const QString& text) { return text.toUInt(); }
    template<> inline long textToNumeric<long>(const QString& text) { return text.toLong(); }
    template<> inline ulong textToNumeric<ulong>(const QString& text) { return text.toULong(); }
    template<> inline qlonglong textToNumeric<qlonglong>(const QString& text) { return text.toLongLong(); }
    template<> inline qulonglong textToNumeric<qulonglong>(const QString& text) { return text.toULongLong(); }
    template<> inline float textToNumeric<float>(const QString& text) { return text.toFloat(); }
    template<> inline double textToNumeric<double>(const QString& text) { return text.toDouble(); }
}

// If you need more customized text representation - use ModelCallback or ModelConversion classes
template <typename NumericType>
class ModelNumericText: public ModelTyped<QString>
{
    Q_DISABLE_COPY(ModelNumericText)

public:
    explicit ModelNumericText(const QSharedPointer<ModelTyped<NumericType>>& modelNumeric, bool ascendingDefault = true)
        : m_modelNumeric(modelNumeric)
    {
        static_assert(std::numeric_limits<NumericType>::is_specialized, "NumericType should be numeric.");
        Q_ASSERT(m_modelNumeric);
        m_ascendingDefault = ascendingDefault;
    }

protected:
    int compareImpl(const ItemID& left, const ItemID& right) const override
    {
        // compare numeric values
        return Private::compareValues(m_modelNumeric->value(left), m_modelNumeric->value(right));
    }

    ValueType_t valueImpl(const ItemID& item) const override
    {
        return Private::numericToText<NumericType>(m_modelNumeric->value(item));
    }

    bool setValueImpl(const ItemID& item, ValueType_t value) override
    {
        return m_modelNumeric->setValue(item, Private::textToNumeric<NumericType>(value));
    }

    bool setValueMultipleImpl(ItemsIterator& itemsIterator, ValueType_t value) override
    {
        return m_modelNumeric->setValueMultiple(itemsIterator, Private::textToNumeric<NumericType>(value));
    }

private:
    QSharedPointer<ModelTyped<NumericType>> m_modelNumeric;
};

class QI_EXPORT ModelRowNumber: public ModelTyped<int>
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelRowNumber)

public:
    explicit ModelRowNumber(bool ascendingDefault = true);

protected:
    int valueImpl(const ItemID& item) const override;
    bool setValueImpl(const ItemID& item, int value) override;
};

class QI_EXPORT ModelColumnNumber: public ModelTyped<int>
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelColumnNumber)

public:
    explicit ModelColumnNumber(bool ascendingDefault = true);

protected:
    ValueType_t valueImpl(const ItemID& item) const override;
    bool setValueImpl(const ItemID& item, ValueType_t value) override;
};

} // end namespace Qi

#endif // QI_NUMERIC_H
