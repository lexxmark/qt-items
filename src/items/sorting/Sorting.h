#ifndef QI_SORTING_H
#define QI_SORTING_H

#include "core/Model.h"
#include "core/Range.h"
#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMouseCaptured.h"

namespace Qi
{

class SpaceGrid;
class ModelComparable;
class Range;

class QI_EXPORT ModelGridSortingBase: public Model
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelGridSortingBase)

public:
    ModelGridSortingBase(const QSharedPointer<SpaceGrid>& grid);

    QSharedPointer<ModelComparable> sortingModel(const ItemID& item) const { return sortingModelImpl(item); }

    ItemID activeSortingItem() const { return !m_sortingExpired ? m_activeSortingItem : ItemID(); }
    void clearActiveSortingItem();
    bool isAscending() const { return m_ascending; }
    void setSorting(const ItemID& item, bool ascending);

    bool sort();
    bool sortByItem(const ItemID& item);
    bool defaultSortByItem(const ItemID& item);
    bool sortByItem(const ItemID& item, bool ascending);

signals:
    void willSortItems(const ModelGridSortingBase*);
    void didSortItems(const ModelGridSortingBase*);

protected:
    virtual QSharedPointer<ModelComparable> sortingModelImpl(const ItemID& /*item*/) const = 0;

    void connectModel(const Model* model);
    void disconnectModel(const Model* model);

private:
    void onSortingModelChanged(const Model* model);

    QSharedPointer<SpaceGrid> m_grid;
    bool m_ascending;
    ItemID m_activeSortingItem;
    bool m_sortingExpired;
};

class QI_EXPORT ModelGridSorting: public ModelGridSortingBase
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelGridSorting)

public:
    ModelGridSorting(const QSharedPointer<SpaceGrid>& grid);
    virtual ~ModelGridSorting();

    void addSortingModel(const ItemID& item, const QSharedPointer<ModelComparable>& model);
    void addSortingModel(int column, const QSharedPointer<ModelComparable>& model);
    void clear();

protected:
    QSharedPointer<ModelComparable> sortingModelImpl(const ItemID& item) const override;

private:
    QMap<ItemID, QSharedPointer<ModelComparable>> m_modelsToSort;
};

class QI_EXPORT ModelGridSortingByRanges: public ModelGridSortingBase
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelGridSortingByRanges)

public:
    ModelGridSortingByRanges(const QSharedPointer<SpaceGrid>& grid);
    virtual ~ModelGridSortingByRanges();

    void addSortingModel(const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range);
    void clear();

protected:
    QSharedPointer<ModelComparable> sortingModelImpl(const ItemID& item) const override;

private:
    struct SortingInfo
    {
        QSharedPointer<Range> range;
        QSharedPointer<ModelComparable> model;
    };
    QVector<SortingInfo> m_modelsToSort;
};

class QI_EXPORT SortingHub
{
public:
    SortingHub();
    SortingHub(const QSharedPointer<ModelGridSortingBase>& sorting1, const QSharedPointer<ModelGridSortingBase>& sorting2);
    ~SortingHub();

    void addSorting(const QSharedPointer<ModelGridSortingBase>& sorting);

    void clearActiveSortingItem();
    void sort();

private:
    void onDidSortItems(const ModelGridSortingBase* activeSorting);

    struct Info
    {
        QSharedPointer<ModelGridSortingBase> sorting;
        QMetaObject::Connection connection;
    };

    QVector<Info> m_sortings;
};

class QI_EXPORT RangeGridSorting: public Range
{
    Q_OBJECT
    Q_DISABLE_COPY(RangeGridSorting)

public:
    RangeGridSorting(const QSharedPointer<ModelGridSortingBase>& model, int row = 0);

protected:
    bool hasItemImpl(const ItemID &item) const override;
    bool hasRowImpl(int row) const override;
    bool hasColumnImpl(int column) const override;

private:
    QSharedPointer<ModelGridSortingBase> m_model;
    int m_row;
};

inline QSharedPointer<RangeGridSorting> makeRangeGridSorter(const QSharedPointer<ModelGridSortingBase>& model, int row = 0)
{
    return QSharedPointer<RangeGridSorting>::create(model, row);
}

class QI_EXPORT ViewGridSorting: public ViewModeled<ModelGridSortingBase>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewGridSorting)

public:
    ViewGridSorting(const QSharedPointer<ModelGridSortingBase>& model, bool useDefaultController = true);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    bool tooltipTextImpl(const ItemID& item, QString& txt) const override;
};

class QI_EXPORT ControllerMouseGridSorting: public ControllerMouseCaptured
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseGridSorting)

public:
    ControllerMouseGridSorting(const QSharedPointer<ModelGridSortingBase>& model);

protected:
    void applyImpl() override;

private:
    QSharedPointer<ModelGridSortingBase> m_model;
};

} // end namespace Qi

#endif // QI_SORTING_H
