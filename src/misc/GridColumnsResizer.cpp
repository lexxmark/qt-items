/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "GridColumnsResizer.h"
#include "widgets/GridWidget.h"
#include "widgets/ListWidget.h"
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
        CacheItem cacheItem(factory->create(item));
        cacheItem.validateCacheView(ctx);
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

namespace Impl
{

ColumnResizeModeInfo::ColumnResizeModeInfo()
    : mode(ColumnResizeModeNone)
{
}

} // end namepace Impl

using namespace Impl;

static const int ToleranceZone = 3;
const ItemID GridColumnsResizer::clientID = Qi::clientID;

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

int GridColumnsResizer::columnFitWidth(int columnsId, int visibleColumn, Impl::ColumnResizeModeInfo& info)
{
    Q_ASSERT(info.mode == ColumnResizeModeFit);

    if (info.param.fitSizeCache == FitSizeCacheInvalid)
        info.param.fitSizeCache = calculateGridColumnFitWidth(*m_gridWidget, columnsId, visibleColumn);

    return info.param.fitSizeCache;
}


ListColumnsResizer::ListColumnsResizer(ListWidget* listWidget)
    : m_listWidget(listWidget)
{
    Q_ASSERT(!m_listWidget.isNull());
    m_listWidget->viewport()->installEventFilter(this);

    connect(m_listWidget->rows().data(), &Lines::linesChanged, this, &ListColumnsResizer::onRowsChanged);
    connect(m_listWidget->columns().data(), &Lines::linesChanged, this, &ListColumnsResizer::onColumnsChanged);
    initColumns(m_listWidget->columns()->count());
}

ListColumnsResizer::~ListColumnsResizer()
{
    if (!m_listWidget.isNull())
    {
        m_listWidget->viewport()->removeEventFilter(this);

        disconnect(m_listWidget->rows().data(), &Lines::linesChanged, this, &ListColumnsResizer::onRowsChanged);
        disconnect(m_listWidget->columns().data(), &Lines::linesChanged, this, &ListColumnsResizer::onColumnsChanged);
    }
}

void ListColumnsResizer::setColumnResizeModeNone(int column)
{
    auto& info = m_columns[column];
    info.mode = ColumnResizeModeNone;
}

void ListColumnsResizer::setColumnResizeModeFit(int column)
{
    auto& info = m_columns[column];
    info.mode = ColumnResizeModeFit;
    info.param.fitSizeCache = FitSizeCacheInvalid;
}

void ListColumnsResizer::setColumnResizeModeFixed(int column, int size)
{
    Q_ASSERT(size >= 0);

    auto& info = m_columns[column];
    info.mode = ColumnResizeModeFixed;
    info.param.fixedSize = size;
}

void ListColumnsResizer::setColumnResizeModeFraction(int column, float fraction)
{
    Q_ASSERT(fraction >= 0);

    auto& info = m_columns[column];
    info.mode = ColumnResizeModeFraction;
    info.param.fraction = fraction;
}

void ListColumnsResizer::setColumnResizeModeFractionN(int column, float fractionN)
{
    Q_ASSERT(fractionN >= 0);

    auto& info = m_columns[column];
    info.mode = ColumnResizeModeFractionN;
    info.param.fractionN = fractionN;
}

void ListColumnsResizer::setColumnResizeModeResidue(int column)
{
    auto& info = m_columns[column];
    info.mode = ColumnResizeModeResidue;
}

void ListColumnsResizer::setAllColumnResizeModeFit()
{
    for (auto& info : m_columns)
    {
        info.mode = ColumnResizeModeFit;
        info.param.fitSizeCache = FitSizeCacheInvalid;
    }
}

int ListColumnsResizer::doResize()
{
    int remainsWidth = m_listWidget->viewport()->width();
    if (remainsWidth <= 0)
        return 0;

    return doResizeColumns(remainsWidth);
}

void ListColumnsResizer::doResizeLater()
{
    callLater(this, [this]() { doResize(); });
}

void ListColumnsResizer::invalidateFitCache()
{
    bool isResizingRequired = false;

    for (auto& info : m_columns)
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

bool ListColumnsResizer::eventFilter(QObject* object, QEvent* event)
{
    Q_ASSERT(object == m_listWidget->viewport());

    if (event->type() == QEvent::Resize)
    {
        doResize();
        //doResizeLater();
    }

    return QObject::eventFilter(object, event);
}

void ListColumnsResizer::onRowsChanged(const Lines* /*lines*/, ChangeReason reason)
{
    if (reason & (ChangeReasonLinesCount | ChangeReasonLinesCountWeak))
        invalidateFitCache();
}

void ListColumnsResizer::onColumnsChanged(const Lines* /*lines*/, ChangeReason reason)
{
    if (reason & ChangeReasonLinesCount)
    {
        initColumns(m_listWidget->columns()->count());
    }
}

void ListColumnsResizer::initColumns(int count)
{
    auto& columns = m_columns;
    if (columns.size() == count)
        return;

    if (count == 0)
    {
        columns.clear();
        return;
    }

    ColumnResizeModeInfo info;
    info.mode = ColumnResizeModeFractionN;
    info.param.fractionN = 1.f;
    columns.fill(info, count);
    columns.back().mode = ColumnResizeModeResidue;
}

int ListColumnsResizer::doResizeColumns(int remainsWidth)
{
    if (remainsWidth <= 0)
        return remainsWidth;

    auto& columns = *m_listWidget->columns();
    auto& columnsInfo = m_columns;
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
                columns.setLineSize(column, columnFitWidth(columns.toVisible(column), info));
                widthProcessed += columns.lineSize(column);
            }
        }
        remainsWidth -= widthProcessed;

        if (remainsWidth <= 0)
            return 0;
    }

    // process ColumnResizeModeFraction and ColumnResizeModeFractionN
    {
        int totalColumnsCount = m_listWidget->columns()->visibleCount();

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
                columns.setLineSize(column, remainsWidth-2);
                remainsWidth = 0;
            }
        }
    }

    return remainsWidth;
}

int ListColumnsResizer::columnFitWidth(int visibleColumn, Impl::ColumnResizeModeInfo& info)
{
    Q_ASSERT(info.mode == ColumnResizeModeFit);

    if (info.param.fitSizeCache == FitSizeCacheInvalid)
        info.param.fitSizeCache = calculateColumnFitWidth(*m_listWidget->grid(), visibleColumn, m_listWidget->guiContext());

    return info.param.fitSizeCache;
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

