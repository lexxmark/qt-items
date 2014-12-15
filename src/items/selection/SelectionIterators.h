#ifndef QI_SELECTION_ITERATORS_H
#define QI_SELECTION_ITERATORS_H

#include "core/ItemsIterator.h"
#include "Selection.h"

namespace Qi
{

class Lines;

class QI_EXPORT ItemsIteratorSelectedVisible: public ItemsIterator
{
public:
    explicit ItemsIteratorSelectedVisible(const ModelSelection& selection);

    ItemID visibleItem() const { return m_currentVisibleItem; }

protected:
    ItemID itemImpl() const override { return m_currentAbsItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const ModelSelection& m_selection;
    const Lines* m_rows;
    const Lines* m_columns;
    ItemID m_currentVisibleItem;
    ItemID m_currentAbsItem;
};

class QI_EXPORT ItemsIteratorSelectedVisibleByColumn: public ItemsIterator
{
public:
    explicit ItemsIteratorSelectedVisibleByColumn(const ModelSelection& selection, int absColumn = 0);

    ItemID visibleItem() const { return m_currentVisibleItem; }

protected:
    ItemID itemImpl() const override { return m_currentAbsItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const ModelSelection& m_selection;
    const Lines* m_rows;
    ItemID m_currentVisibleItem;
    ItemID m_currentAbsItem;
};


} // end namespace Qi

#endif // QI_SELECTION_ITERATORS_H
