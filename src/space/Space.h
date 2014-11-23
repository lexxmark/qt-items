#ifndef QI_SPACE_H
#define QI_SPACE_H

#include "core/ItemSchema.h"
#include "core/Layout.h"
#include "core/View.h"
#include <QSize>
#include <QPoint>

namespace Qi
{

// creates composite view for all views in the item
QI_EXPORT ViewSchema viewSchemaForItem(const ItemID& item, ViewApplicationMask viewApplicationMask, const QVector<ItemSchema>& itemSchemas);

enum SpaceHint
{
    SpaceHintNone = 0x0000,
    SpaceHintSimilarViewsByColumns = 0x0001
};

typedef quint32 SpaceHints;

class QI_EXPORT Space: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Space)

public:
    Space(SpaceHints hints = SpaceHintNone);
    virtual ~Space();

    virtual QSize size() const = 0;
    virtual ItemID toAbsolute(const ItemID& visibleItem) const = 0;
    virtual ItemID toVisible(const ItemID& absoluteItem) const = 0;
    virtual QRect itemRect(const ItemID& visibleItem) const = 0;

    const QVector<ItemSchema>& schemas() const { return m_schemas; }
    int addSchema(const ItemSchema& schema);
    int addSchema(const QSharedPointer<Range>& range, const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout = makeLayoutClient()) { return addSchema(ItemSchema(range, layout, view)); }
    int insertSchema(int index, const QSharedPointer<Range>& range, const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout = makeLayoutClient());
    void removeSchema(const QSharedPointer<View>& view);
    void clearSchemas();

    SpaceHints hints() const { return m_hints; }
    void setHints(SpaceHints hints) { m_hints = hints; }

    ViewApplicationMask viewApplicationMask() const { return m_viewApplicationMask; }
    void setViewApplicationMask(ViewApplicationMask viewApplicationMask);

    const QVector<ItemSchema>& schemasOrdered() const;

signals:
    void spaceChanged(const Space* space, ChangeReason reason);

private slots:
    void onRangeChanged(const Range* range, ChangeReason reason);
    void onLayoutChanged(const Layout* layout, ChangeReason reason);
    void onViewChanged(const View* view, ChangeReason reason);

private:
    void connectSchema(const ItemSchema& schema);
    void disconnectSchema(const ItemSchema& schema);

    QVector<ItemSchema> m_schemas;
    mutable QVector<ItemSchema> m_schemasOrdered;

    // hints
    SpaceHints m_hints;

    // views filtering
    ViewApplicationMask m_viewApplicationMask;
};

} // end namespace Qi 

#endif // QI_GRID_SPACE_H
