#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "space/SpaceItem.h"
#include "SpaceWidgetAbstract.h"

namespace Qi
{

class QI_EXPORT ItemWidget: public SpaceWidgetAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemWidget)

public:
    explicit ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();

    const SpaceItem& spaceItem() const { return *m_space; }
    SpaceItem& spaceItem() { return *m_space; }

    // there are two ways to synchronize space size:
    // 1. sync with widget size (minimumSizeHint will work by default)
    // 2. sync with item content (minimumSizeHint will return desired item size)
    void syncSpaceSizeWithContent(bool enable);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool event(QEvent* e) override;

private:
    void onSpaceChanged(const Space* space, ChangeReason reason);

    QSharedPointer<SpaceItem> m_space;
    bool m_syncSpaceSizeWithContent;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
