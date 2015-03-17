#ifndef QI_GRID_COLUMNS_RESIZER_H
#define QI_GRID_COLUMNS_RESIZER_H

#include "widgets/GridWidget.h"
#include "core/ControllerMouse.h"

namespace Qi
{

QI_EXPORT int calculateColumnFitWidth(const SpaceGrid& grid, int visibleColumn, const GuiContext& ctx);
QI_EXPORT int calculateGridColumnFitWidth(const GridWidget& gridWidget, int columnsId, int visibleColumn);

enum ColumnResizeMode
{
    ColumnResizeModeNone = 0x000,
    ColumnResizeModeFit = 0x001,
    ColumnResizeModeFixed = 0x002,
    ColumnResizeModeFraction = 0x004,
    ColumnResizeModeFractionN = 0x008,
    ColumnResizeModeResidue = 0x010
};

class QI_EXPORT GridColumnsResizer: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GridColumnsResizer)

public:
    explicit GridColumnsResizer(GridWidget* gridWidget);
    virtual ~GridColumnsResizer();

    void setColumnResizeModeNone(int column, const ItemID& subGridId = clientID);
    void setColumnResizeModeFit(int column, const ItemID& subGridId = clientID);
    void setColumnResizeModeFixed(int column, int size, const ItemID& subGridId = clientID);
    void setColumnResizeModeFraction(int column, float fraction, const ItemID& subGridId = clientID);
    void setColumnResizeModeFractionN(int column, float fractionN, const ItemID& subGridId = clientID);
    void setColumnResizeModeResidue(int column, const ItemID& subGridId = clientID);

    void setAllColumnResizeModeFit(const ItemID& subGridId = clientID);

    int doResize();
    void doResizeLater();
    void invalidateFitCache();

    bool eventFilter(QObject* object, QEvent* event) override;

private:
    struct ColumnResizeModeInfo
    {
        ColumnResizeMode mode;
        union
        {
            int fitSizeCache;
            int fixedSize;
            float fraction;
            float fractionN;
        } param;

        ColumnResizeModeInfo();
    };

    void onRowsChanged(const Lines* lines, ChangeReason reason);
    void onColumnsChanged(const Lines* lines, ChangeReason reason);
    void initColumns(int columnsId, int count);
    int doResizeColumns(int columnsId, int remainsWidth);
    int columnFitWidth(int columnsId, int visibleColumn, ColumnResizeModeInfo& info);

    QPointer<GridWidget> m_gridWidget;
    QVector<ColumnResizeModeInfo> m_columns[3];
};

class QI_EXPORT ControllerMouseColumnsAutoFit: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseColumnsAutoFit)

public:
    ControllerMouseColumnsAutoFit(GridWidget* gridWidget, int columnsID, ControllerMousePriority priority = ControllerMousePriorityOverlay);

    bool processLButtonDblClick(QMouseEvent* /*event*/) override;
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

private:
    GridWidget* m_gridWidget;
    int m_columnsID;
};

} // end namespace Qi

#endif // QI_GRID_COLUMNS_RESIZER_H
