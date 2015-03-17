#include "GridColumnsResizer.h"
#include "cache/CacheItemFactory.h"
#include "utils/CallLater.h"
#include <QEvent>

namespace Qi
{

static const int FitSizeCacheInvalid = -1;

int calculateColumnFitWidth(const SpaceGrid& grid, int visibleColumn, const GuiContext& ctx)
{
    int fitWidth = 0;

    auto factory = grid.createCacheItemFactory();
    ViewSizeMode sizeMode = (grid.rowsVisibleCount() <= 1000) ? ViewSizeModeExact : ViewSizeModeFastAverage;
    for (ItemID item(0, visibleColumn); item.row < grid.rowsVisibleCount(); ++item.row)
    {
        CacheItemInfo cacheItemInfo = factory->create(item);
        CacheItem cacheItem(cacheItemInfo);
        cacheItem.initializeCacheView(ctx);
        fitWidth = qMax(fitWidth, cacheItem.calculateItemSize(ctx, sizeMode).width());
    }

    return fitWidth;
}

int calculateGridColumnFitWidth(const GridWidget& gridWidget, int columnsId, int visibleColumn)
{
    int fitWidth = 0;
    fitWidth = qMax(fitWidth, calculateColumnFitWidth(*gridWidget.subGrid(ItemID(0, columnsId)),
                                                      visibleColumn,
                                                      gridWidget.guiContext()));

    fitWidth = qMax(fitWidth, calculateColumnFitWidth(*gridWidget.subGrid(ItemID(1, columnsId)),
                                                      visibleColumn,
                                                      gridWidget.guiContext()));

    fitWidth = qMax(fitWidth, calculateColumnFitWidth(*gridWidget.subGrid(ItemID(2, columnsId)),
                                                      visibleColumn,
                                                      gridWidget.guiContext()));

    return fitWidth;
}

static const int ToleranceZone = 3;

GridColumnsResizer::GridColumnsResizer(GridWidget* gridWidget)
    : m_gridWidget(gridWidget)
{
    Q_ASSERT(!m_gridWidget.isNull());
    m_gridWidget->installEventFilter(this);

    for (int id = 0; id < 3; ++id)
    {
        connect(m_gridWidget->rows(id).data(), &Lines::linesChanged, this, &GridColumnsResizer::onRowsChanged);
        connect(m_gridWidget->columns(id).data(), &Lines::linesChanged, this, &GridColumnsResizer::onColumnsChanged);
        initColumns(id, m_gridWidget->columns(id)->count());
    }
}

GridColumnsResizer::~GridColumnsResizer()
{
    if (!m_gridWidget.isNull())
    {
        m_gridWidget->removeEventFilter(this);

        for (int id = 0; id < 3; ++id)
        {
            disconnect(m_gridWidget->rows(id).data(), &Lines::linesChanged, this, &GridColumnsResizer::onRowsChanged);
            disconnect(m_gridWidget->columns(id).data(), &Lines::linesChanged, this, &GridColumnsResizer::onColumnsChanged);
        }
    }
}

void GridColumnsResizer::setColumnResizeModeNone(int column, const ItemID& subGridId)
{
    auto& info = m_columns[subGridId.column][column];
    info.mode = ColumnResizeModeNone;
}

void GridColumnsResizer::setColumnResizeModeFit(int column, const ItemID& subGridId)
{
    auto& info = m_columns[subGridId.column][column];
    info.mode = ColumnResizeModeFit;
    info.param.fitSizeCache = FitSizeCacheInvalid;
}

void GridColumnsResizer::setColumnResizeModeFixed(int column, int size, const ItemID& subGridId)
{
    Q_ASSERT(size >= 0);

    auto& info = m_columns[subGridId.column][column];
    info.mode = ColumnResizeModeFixed;
    info.param.fixedSize = size;
}

void GridColumnsResizer::setColumnResizeModeFraction(int column, float fraction, const ItemID& subGridId)
{
    Q_ASSERT(fraction >= 0);

    auto& info = m_columns[subGridId.column][column];
    info.mode = ColumnResizeModeFraction;
    info.param.fraction = fraction;
}

void GridColumnsResizer::setColumnResizeModeFractionN(int column, float fractionN, const ItemID& subGridId)
{
    Q_ASSERT(fractionN >= 0);

    auto& info = m_columns[subGridId.column][column];
    info.mode = ColumnResizeModeFractionN;
    info.param.fractionN = fractionN;
}

void GridColumnsResizer::setColumnResizeModeResidue(int column, const ItemID& subGridId)
{
    auto& info = m_columns[subGridId.column][column];
    info.mode = ColumnResizeModeResidue;
}

void GridColumnsResizer::setAllColumnResizeModeFit(const ItemID& subGridId)
{
    for (auto& info : m_columns[subGridId.column])
    {
        info.mode = ColumnResizeModeFit;
        info.param.fitSizeCache = FitSizeCacheInvalid;
    }
}

int GridColumnsResizer::doResize()
{
    int remainsWidth = m_gridWidget->viewport()->width();
    if (remainsWidth <= 0)
        return 0;

    remainsWidth = doResizeColumns(0, remainsWidth);
    remainsWidth = doResizeColumns(2, remainsWidth);
    remainsWidth = doResizeColumns(1, remainsWidth);

    return remainsWidth;
}

void GridColumnsResizer::doResizeLater()
{
    callLater(this, [this]() { doResize(); });
}

void GridColumnsResizer::invalidateFitCache()
{
    bool isResizingRequired = false;

    for (auto& columns : m_columns)
        for (auto& info : columns)
        {
            if (info.mode == ColumnResizeModeFit &&
                info.param.fitSizeCache != FitSizeCacheInvalid)
            {
                info.param.fitSizeCache = FitSizeCacheInvalid;
                isResizingRequired = true;
            }
        }

    if (isResizingRequired)
        doResizeLater();
}

bool GridColumnsResizer::eventFilter(QObject* object, QEvent* event)
{
    Q_ASSERT(object == m_gridWidget.data());

    if (event->type() == QEvent::Resize)
    {
        doResizeLater();
    }

    return QObject::eventFilter(object, event);
}

void GridColumnsResizer::onRowsChanged(const Lines* /*lines*/, ChangeReason reason)
{
    if (reason & (ChangeReasonLinesCount | ChangeReasonLinesCountWeak))
        invalidateFitCache();
}

void GridColumnsResizer::onColumnsChanged(const Lines* lines, ChangeReason reason)
{
    if (reason & ChangeReasonLinesCount)
    {
        for (int id = 0; id < 3; ++id)
        {
            if (m_gridWidget->columns(id).data() == lines)
            {
                initColumns(id, m_gridWidget->columns(id)->count());
                break;
            }
        }
    }
}

void GridColumnsResizer::initColumns(int columnsId, int count)
{
    auto& columns = m_columns[columnsId];
    if (columns.size() == count)
        return;

    if (count == 0)
    {
        columns.clear();
        return;
    }

    if (columnsId == 1)
    {
        ColumnResizeModeInfo info;
        info.mode = ColumnResizeModeFractionN;
        info.param.fractionN = 1.f;
        columns.fill(info, count);
        columns.back().mode = ColumnResizeModeResidue;
    }
    else
    {
        columns.fill(ColumnResizeModeInfo(), count);
    }
}

int GridColumnsResizer::doResizeColumns(int columnsId, int remainsWidth)
{
    if (remainsWidth <= 0)
        return remainsWidth;

    auto& columns = *m_gridWidget->columns(columnsId);
    auto& columnsInfo = m_columns[columnsId];
    Q_ASSERT(columns.count() == columnsInfo.size());

    if (columns.isEmptyVisible())
        return remainsWidth;

    // process ColumnResizeModeNone
    {
        int widthProcessed = 0;
        for (int column = 0; column < columns.count(); ++column)
        {
            if (!columns.isLineVisible(column))
                continue;

            const auto& info = columnsInfo[column];
            if (info.mode == ColumnResizeModeNone)
            {
                widthProcessed += columns.lineSize(column);
            }
        }
        remainsWidth -= widthProcessed;

        if (remainsWidth <= 0)
            return 0;
    }

    // process ColumnResizeModeFixed
    {
        int widthProcessed = 0;
        for (int column = 0; column < columns.count(); ++column)
        {
            if (!columns.isLineVisible(column))
                continue;

            const auto& info = columnsInfo[column];
            if (info.mode == ColumnResizeModeFixed)
            {
                columns.setLineSize(column, info.param.fixedSize);
                widthProcessed += columns.lineSize(column);
            }
        }
        remainsWidth -= widthProcessed;

        if (remainsWidth <= 0)
            return 0;
    }

    // process ColumnResizeModeFit
    {
        int widthProcessed = 0;
        for (int column = 0; column < columns.count(); ++column)
        {
            if (!columns.isLineVisible(column))
                continue;

            auto& info = columnsInfo[column];
            if (info.mode == ColumnResizeModeFit)
            {
                columns.setLineSize(column, columnFitWidth(columnsId, columns.toVisible(column), info));
                widthProcessed += columns.lineSize(column);
            }
        }
        remainsWidth -= widthProcessed;

        if (remainsWidth <= 0)
            return 0;
    }

    // process ColumnResizeModeFraction and ColumnResizeModeFractionN
    {
        int totalColumnsCount = m_gridWidget->columns(0)->visibleCount();
        totalColumnsCount += m_gridWidget->columns(1)->visibleCount();
        totalColumnsCount += m_gridWidget->columns(2)->visibleCount();

        int widthProcessed = 0;
        for (int column = 0; column < columns.count(); ++column)
        {
            if (!columns.isLineVisible(column))
                continue;

            const auto& info = columnsInfo[column];
            if (info.mode == ColumnResizeModeFraction)
            {
                columns.setLineSize(column, int((float)remainsWidth*info.param.fraction));
                widthProcessed += columns.lineSize(column);
            }
            else if (info.mode == ColumnResizeModeFractionN)
            {
                columns.setLineSize(column, int((float)remainsWidth*info.param.fractionN/(float)totalColumnsCount));
                widthProcessed += columns.lineSize(column);
            }
        }
        remainsWidth -= widthProcessed;

        if (remainsWidth <= 0)
            return 0;
    }

    // process ColumnResizeModeResidue
    {
        for (int column = 0; column < columns.count(); ++column)
        {
            if (!columns.isLineVisible(column))
                continue;

            const auto& info = columnsInfo[column];
            if (info.mode == ColumnResizeModeResidue)
            {
                // first ColumnResizeModeResidue will occupy all free space
                // all other would got 0
                columns.setLineSize(column, remainsWidth);
                remainsWidth = 0;
            }
        }
    }

    return remainsWidth;
}

int GridColumnsResizer::columnFitWidth(int columnsId, int visibleColumn, GridColumnsResizer::ColumnResizeModeInfo& info)
{
    Q_ASSERT(info.mode == ColumnResizeModeFit);

    if (info.param.fitSizeCache == FitSizeCacheInvalid)
        info.param.fitSizeCache = calculateGridColumnFitWidth(*m_gridWidget, columnsId, visibleColumn);

    return info.param.fitSizeCache;
}

GridColumnsResizer::ColumnResizeModeInfo::ColumnResizeModeInfo()
    : mode(ColumnResizeModeNone)
{
}

ControllerMouseColumnsAutoFit::ControllerMouseColumnsAutoFit(GridWidget* gridWidget, int columnsID, ControllerMousePriority priority)
    : ControllerMouse(priority),
      m_gridWidget(gridWidget),
      m_columnsID(columnsID)
{}

bool ControllerMouseColumnsAutoFit::processLButtonDblClick(QMouseEvent* /*event*/)
{
    int fitWidth = calculateGridColumnFitWidth(*m_gridWidget, m_columnsID, activationState().visibleItem().column);
    if (fitWidth > 0)
        m_gridWidget->columns(m_columnsID)->setLineSize(activationState().item.column, fitWidth);
    return true;
}

void ControllerMouseColumnsAutoFit::activateImpl(const ActivationInfo& activationInfo)
{
    ControllerMouse::activateImpl(activationInfo);
    activationState().context.pushCursor(Qt::SplitHCursor, this);
}

void ControllerMouseColumnsAutoFit::deactivateImpl()
{
    activationState().context.popCursor(this);
    ControllerMouse::deactivateImpl();
}

} // end namespace Qi

