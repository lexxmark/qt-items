#ifndef QI_GRID_H
#define QI_GRID_H

#include "Lines.h"
#include "CellsSchema.h"
#include <QSize>
#include <QPoint>

namespace Qi
{

class Range;
class View;
class Layout;

class QI_EXPORT Grid: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Grid)

public:
    Grid();
    Grid(const QSharedPointer<Lines>& rows, const QSharedPointer<Lines>& columns);
    virtual ~Grid();
    
    const Lines& rows() const;
    const Lines& columns() const;

    Lines& rows();
    Lines& columns();

    const QSharedPointer<Lines>& rowsPtr() const;
    const QSharedPointer<Lines>& columnsPtr() const;
    
    QSize dim() const;
    QSize visibleDim() const;
    
    QSize visibleSize() const;
    
    CellID toVisible(CellID absoluteCell) const;
    CellID toVisibleSafe(CellID absoluteCell) const;

    CellID toAbsolute(CellID visibleCell) const;
    CellID toAbsoluteSafe(CellID visibleCell) const;
    
    CellID findVisCell(QPoint point) const;
    CellID findVisCellExact(QPoint point) const;
    
    const QVector<CellsSchema>& cellsSchemas() const { return m_cellsSchemas; }

    void addCellsSchema(Range* range, Layout* layout, View* view, ControllerMouse* controller = nullptr);
    void clearCellsSchemas();
    
signals:
    void gridChanged(const Grid*, ChangeReason);
    
private slots:
    void onLinesChanged(const Lines* lines, ChangeReason reason);
    
private:
    void connectLinesSignal();
    void disconnectLinesSignal();
    
    QSharedPointer<Lines> m_rows;
    QSharedPointer<Lines> m_columns;
    QVector<CellsSchema> m_cellsSchemas;
};

} // end namespace Qi 

#endif // QI_GRID_H
