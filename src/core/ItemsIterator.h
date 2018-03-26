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

#ifndef QI_ITEMS_ITERATOR_H
#define QI_ITEMS_ITERATOR_H

#include "ID.h"

namespace Qi
{

// interface class to iterate through items
// it can be used right after creation (no need call atFirst)
// the common usage is
// for (IdIteratorDerived cit(...); cit.isValid(); cit.toNext())
// {
//    cit.item();
// }

class QI_EXPORT IdIterator
{
    Q_DISABLE_COPY(IdIterator)

public:
    virtual ~IdIterator() = default;

    ID id() const { return idImpl(); }
    bool atFirst() { return atFirstImpl(); }
    bool toNext() { return toNextImpl(); }

    bool isValid() const { return isValidImpl(); }

protected:
    IdIterator() = default;

    virtual ID idImpl() const = 0;
    virtual bool atFirstImpl() = 0;
    virtual bool toNextImpl() = 0;
    virtual bool isValidImpl() const = 0;
};

/*
template <typename Iterator>
class ItemsIteratorRange: public IdIterator
{
public:
    ItemsIteratorRange(Iterator begin, Iterator end)
        : m_begin(begin), m_end(end), m_current(begin)
    {}

    ItemsIteratorRange(const ItemsIteratorRange& other)
        : m_begin(other.m_begin), m_end(other.m_end), m_current(other.m_current)
    {}

protected:
    ID itemImpl() const override
    {
        if (m_current == m_end)
            return ID();
        else
            return *m_current;
    }

    bool atFirstImpl() override
    {
        m_current = m_begin;
        return m_current != m_end;
    }

    bool toNextImpl() override
    {
        if (m_current != m_end)
        {
            ++m_current;
            return true;
        }
        else
            return false;
    }

private:
    Iterator m_begin;
    Iterator m_end;
    Iterator m_current;
};

template <typename Container>
ItemsIteratorRange<typename Container::const_iterator> createItemsIteratorFromContainer(const Container& container)
{
    return ItemsIteratorRange<typename Container::const_iterator>(container.begin(), container.end());
}
*/
} // end namespace Qi

#endif // QI_ITEMS_ITERATOR_H
