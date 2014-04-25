#ifndef QI_VIEW_GRID_H
#define QI_VIEW_GRID_H

#include "View.h"
#include "Grid.h"
#include <QPointer>

namespace Qi
{

class CacheGrid;

class QI_EXPORT ViewGrid: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewGrid)

public:
    ViewGrid(Grid* grid);
    virtual ~ViewGrid();
    
    const Grid& grid() const { return *m_grid; }
    void setGrid(Grid* grid);

    const QRect& frame() const;
    void setFrame(const QRect& frame);

protected:
    void drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const override;
    QSize sizeHintImpl(const QWidget* widget, const CellID& cell) const override;
    bool textImpl(const CellID& cell, QString& text) const override;

private:
    QPointer<Grid> m_grid;
    QScopedPointer<CacheGrid> m_cache;
};

} // end namespace Qi

#endif // QI_VIEW_GRID_H
