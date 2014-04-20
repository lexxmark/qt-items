#ifndef QI_CACHE_GRID_H
#define QI_CACHE_GRID_H

#include "Grid.h"
#include "Cache.h"
#include <QRect>

namespace Qi
{

class QI_EXPORT CacheGrid: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheGrid)

public:
    CacheGrid(const Grid& grid);
    virtual ~CacheGrid();
    
    const QRect& frame() const;
    bool setFrame(const QRect& frame);
    
    const CellID& startVisibleCell() const { return m_startVisibleCell; }
    const CellID& endVisibleCell() const { return m_endVisibleCell; }

    const CacheCell* cacheCell(const CellID& visibleCell) const;

    void draw(QPainter* painter, const QWidget* widget) const;
   
private slots:
    void onGridChanged(const Grid* grid, ChangeReason reason);
    
private:
    void validateFrame() const;
    
    const Grid& m_grid;
    
    QRect m_frame;
    mutable bool m_isFrameValid;
    CellID m_startVisibleCell;
    CellID m_endVisibleCell;

    mutable std::vector<CacheCell> m_visibleCells;
};

} // end namespace Qi 

#endif // QI_CACHE_GRID_H
