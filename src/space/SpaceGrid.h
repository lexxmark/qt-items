#ifndef QI_SPACE_GRID_H
#define QI_SPACE_GRID_H

#include "Space.h"
#include "Lines.h"
#include <QPoint>

namespace Qi
{

class ModelComparable;

class QI_EXPORT SpaceGrid: public Space
{
public:
    SpaceGrid(SpaceHints hints = SpaceHintNone);
    SpaceGrid(const QSharedPointer<Lines>& rows, const QSharedPointer<Lines>& columns, SpaceHints hints = SpaceHintNone);
    ~SpaceGrid();

    QSize size() const override;
    ItemID toAbsolute(const ItemID& visibleItem) const override { return ItemID(m_rows->toAbsolute(visibleItem.row), m_columns->toAbsolute(visibleItem.column)); }
    ItemID toVisible(const ItemID& absoluteItem) const override { return ItemID(m_rows->toVisible(absoluteItem.row), m_columns->toVisible(absoluteItem.column)); }
    QRect itemRect(const ItemID& visibleItem) const override;

    bool isEmpty() const { return m_rows->isEmpty() || m_columns->isEmpty(); }
    bool isEmptyVisible() const { return m_rows->isEmptyVisible() || m_columns->isEmptyVisible(); }

    const QSharedPointer<Lines>& rows() const { return m_rows; }
    const QSharedPointer<Lines>& columns() const { return m_columns; }

    void setDimensions(quint32 rows, quint32 columns);
    void setRowsCount(quint32 rows) { m_rows->setCount(rows); }
    void setColumnsCount(quint32 columns) { m_columns->setCount(columns); }

    QSize dimensions() const;
    quint32 rowsCount() const { return m_rows->count(); }
    quint32 columnsCount() const { return m_columns->count(); }

    quint32 rowsVisibleCount() const { return m_rows->visibleCount(); }
    quint32 columnsVisibleCount() const { return m_columns->visibleCount(); }

    void shareRows(const QSharedPointer<Lines>& rows);
    void shareColumns(const QSharedPointer<Lines>& columns);

    void unshareRows();
    void unshareColumns();

    QSize itemSize(const ItemID& item) const;

    bool checkItem(const ItemID& item) const;
    bool checkVisibleItem(const ItemID& item) const;
    bool isItemVisible(const ItemID& item) const;

    void sortColumnByModel(quint32 column, const QSharedPointer<ModelComparable>& model, bool ascending, bool stable);
    void sortRowByModel(quint32 row, const QSharedPointer<ModelComparable>& model, bool ascending, bool stable);

    void sortColumnByRangedModel(quint32 column, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool ascending, bool stable, bool outOfRangeIsSmall);
    void sortRowByRangedModel(quint32 row, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool ascending, bool stable, bool outOfRangeIsSmall);

private slots:
    void onLinesChanged(const Lines* lines, ChangeReason reason);

private:
    void connectLines(const QSharedPointer<Lines>& lines);
    void disconnectLines(const QSharedPointer<Lines>& lines);

    ItemID trimItem(const ItemID& item) const;
    ItemID trimVisibleItem(const ItemID& item) const;

    QSharedPointer<Lines> m_rows;
    QSharedPointer<Lines> m_columns;
};

QI_EXPORT QSharedPointer<Range> createItemRangeRect(const SpaceGrid& grid, const ItemID& displayCorner1, const ItemID& displayCorner2);

} // end namespace Qi 

#endif // QI_SPACE_GRID_H
