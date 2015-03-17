#ifndef QI_SPACE_WIDGET_SCROLL_ABSTRACT_H
#define QI_SPACE_WIDGET_SCROLL_ABSTRACT_H

#include "SpaceWidgetCore.h"
#include <QAbstractScrollArea>

namespace Qi
{

class QI_EXPORT SpaceWidgetScrollAbstract: public QAbstractScrollArea, public SpaceWidgetCore
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceWidgetScrollAbstract)

public:
    virtual ~SpaceWidgetScrollAbstract();

protected:
    explicit SpaceWidgetScrollAbstract(QWidget *parent = nullptr);

    bool viewportEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;
    void scrollContentsBy(int dx, int dy) override;
    QSize viewportSizeHint() const override;

    // SpaceWidgetCore implementation
    void ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;

    void updateScrollbars();
    void invalidateCacheItemsLayout();
    void validateCacheItemsLayout();

    virtual void validateCacheItemsLayoutImpl();
    virtual QSize calculateVirtualSizeImpl() const;
    virtual QSize calculateScrollableSizeImpl() const;
    virtual void updateCacheScrollOffsetImpl();

private:
    bool m_isCacheItemsLayoutValid;
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_SCROLL_ABSTRACT_H
