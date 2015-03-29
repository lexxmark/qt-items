#ifndef QI_LIST_WIDGET_H
#define QI_LIST_WIDGET_H

#include "QiAPI.h"
#include "space/SpaceGrid.h"
#include "core/SpaceWidgetScrollAbstract.h"

namespace Qi
{

class CacheSpaceGrid;

class QI_EXPORT ListWidget: public SpaceWidgetScrollAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(ListWidget)

public:
    explicit ListWidget(QWidget *parent = nullptr);
    virtual ~ListWidget();

    const QSharedPointer<SpaceGrid>& grid() const { return m_grid; }

    const QSharedPointer<Lines>& rows() const { return m_grid->rows(); }
    const QSharedPointer<Lines>& columns() const { return m_grid->columns(); }

    const QSharedPointer<CacheSpaceGrid>& cacheGrid() const { return m_cacheGrid;}

private:
    QSharedPointer<SpaceGrid> m_grid;
    QSharedPointer<CacheSpaceGrid> m_cacheGrid;
};

} // end namespace Qi

#endif // QI_LIST_WIDGET_H
