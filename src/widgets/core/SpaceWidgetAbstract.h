#ifndef QI_SPACE_WIDGET_ABSTRACT_H
#define QI_SPACE_WIDGET_ABSTRACT_H

#include "SpaceWidgetCore.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT SpaceWidgetAbstract: public QWidget, public SpaceWidgetCore
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceWidgetAbstract)

public:
    virtual ~SpaceWidgetAbstract();

protected:
    explicit SpaceWidgetAbstract(QWidget *parent = nullptr);

    bool event(QEvent* event) override;

    // SpaceWidgetCore implementation
    void ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_ABSTRACT_H
