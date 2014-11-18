#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "cache/space/CacheSpaceItem.h"
#include <QWidget>

namespace Qi
{

class SpaceWidgetPrivate;

class QI_EXPORT ItemWidget: public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemWidget)

public:
    explicit ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();

    const SpaceItem& space() const { return *m_space; }
    SpaceItem& space() { return *m_space; }

    void syncSpaceSizeWithContent(bool enable);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool event(QEvent* e) override;

private:
    void onSpaceChanged(const Space* space, ChangeReason reason);

    QSharedPointer<SpaceItem> m_space;
    QScopedPointer<SpaceWidgetPrivate> m_impl;
    bool m_syncSpaceSizeWithContent;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
