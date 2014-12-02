#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "cache/space/CacheSpaceItem.h"
#include "WidgetDriver.h"
#include <QWidget>

namespace Qi
{

class SpaceWidgetPrivate;

class QI_EXPORT ItemWidget: public QWidget, private WidgetDriver
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemWidget)

public:
    explicit ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();

    const SpaceItem& space() const { return *m_space; }
    SpaceItem& space() { return *m_space; }

    // there are two ways to synchronize space size:
    // 1. sync with widget size (minimumSizeHint will work by default)
    // 2. sync with item content (minimumSizeHint will return desired item size)
    void syncSpaceSizeWithContent(bool enable);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool event(QEvent* e) override;

private:
    // WidgetDriver implementation
    void ensureVisible(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;
    bool doEdit(const ItemID& visibleItem, const CacheSpace *cacheSpace, const QKeyEvent *event) override;

    void onSpaceChanged(const Space* space, ChangeReason reason);

    QSharedPointer<SpaceItem> m_space;
    QScopedPointer<SpaceWidgetPrivate> m_impl;
    bool m_syncSpaceSizeWithContent;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
