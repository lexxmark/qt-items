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

#include "Selection.h"
#include "space/grid/CacheSpaceGrid.h"
#include "space/grid/RangeGrid.h"
#include "widgets/core/SpaceWidgetCore.h"
#include <QStyleOptionViewItem>

namespace Qi
{

ModelSelection::ModelSelection(SharedPtr<SpaceGrid> space)
    : m_space(std::move(space)),
      m_selectionOperations(0)
{
    Q_ASSERT(m_space);
}

ModelSelection::~ModelSelection()
{
    Q_ASSERT(m_selectionOperations == 0);
}

bool ModelSelection::isVisibleItemSelected(GridID visibleId) const
{
    if (!visibleId.isValid())
        return false;

    if (!m_space)
        return false;

    auto absId = m_space.data()->toGridAbsolute(visibleId);
    if (!absId.isValid())
        return false;

    return isItemSelected(absId);
}

void ModelSelection::addSelection(SharedPtr<Range> range, bool exclude)
{
    m_selection.addRange(std::move(range), exclude);
    emitChangedSignals(ChangeReasonSelection);
}

void ModelSelection::setSelection(SharedPtr<Range> range)
{
    m_selection.clear();
    m_selection.addRange(std::move(range), false);
    emitChangedSignals(ChangeReasonSelection);
}

void ModelSelection::clearSelection()
{
    m_selection.clear();
    emitChangedSignals(ChangeReasonSelection);
}

void ModelSelection::applySelection(const RangeSelection& selection)
{
    m_selection = selection;
    emitChangedSignals(ChangeReasonSelection);
}

GridID ModelSelection::activeVisibleId() const
{
    return m_space ? m_space.data()->toGridVisible(m_activeId) : GridID();
}

void ModelSelection::setActiveId(GridID id)
{
    if (m_activeId != id)
    {
        m_activeId = id;
        emitChangedSignals(ChangeReasonActiveItem);
    }
}

void ModelSelection::setActiveVisibleId(GridID visibleId)
{
    Q_ASSERT(m_space);
    if (!m_space)
        return;

    setActiveId(m_space.data()->toGridAbsolute(visibleId));
}

void ModelSelection::startSelectionOperation()
{
    ++m_selectionOperations;
    if (m_selectionOperations == 1)
    {
        // invoke only for the 1st time
        emit selectionOperationPerformed(this, true);
    }
}

void ModelSelection::stopSelectionOperation()
{
    Q_ASSERT(m_selectionOperations > 0);
    --m_selectionOperations;
    if (m_selectionOperations == 0)
    {
        // invoke only for the last time
        emit selectionOperationPerformed(this, false);
    }
}

int ModelSelection::compareImpl(ID left, ID right) const
{
    return (int)isItemSelected(left.as<GridID>()) - (int)isItemSelected(right.as<GridID>());
}

void ModelSelection::emitChangedSignals(ChangeReason changeReason)
{
    emit modelChanged(this);

    startSelectionOperation();
    emit selectionChanged(this, changeReason);
    stopSelectionOperation();
}

bool ModelSelectionRows::isRowSelected(int row) const
{
    return m_selection.hasItem(makeID<GridID>(row, InvalidIndex));
}

void ModelSelectionRows::selectRows(const QSet<int>& rows)
{
    setSelection(makeShared<RangeGridRows>(rows));
}

void ModelSelectionColumns::selectColumns(const QSet<int>& columns)
{
    setSelection(makeShared<RangeGridColumns>(columns));
}

ViewSelectionClient::ViewSelectionClient(const SharedPtr<ModelSelection> &model, bool useDefaultController)
    : ViewModeled<ModelSelection>(model)
{
    if (useDefaultController)
    {
        setController(makeShared<ControllerMouseSelectionClient>(model));
    }
}

ViewSelectionClient::~ViewSelectionClient()
{
}

void ViewSelectionClient::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    m_painterState.save(painter);

    auto style = ctx.style();
    auto id = cache.id.as<GridID>();

    // cannot use drawPrimitive(QStyle::PE_PanelItemViewItem) from QWindowsVistaStyle class
    if (style->inherits("QWindowsVistaStyle"))
    {
        QPalette::ColorGroup cg = ctx.colorGroup();

        if (theModel()->isItemSelected(id))
        {
            painter->setPen(ctx.widget->palette().color(cg, QPalette::HighlightedText));
            painter->fillRect(cache.cacheView.rect(), ctx.widget->palette().brush(cg, QPalette::Highlight));
        }

        // draw focus rect for active item
        if ((cg == QPalette::Active) && (theModel()->activeId() == id))
        {
            QStyleOptionFocusRect focusOption;
            ctx.initStyleOption(focusOption);
            focusOption.state |= QStyle::State_KeyboardFocusChange;
            focusOption.rect = cache.cacheView.rect();
            style->drawPrimitive(QStyle::PE_FrameFocusRect, &focusOption, painter, ctx.widget);
        }

        return;
    }

    QStyleOptionViewItem option;
    ctx.initStyleOption(option);
    option.rect = cache.cacheView.rect();
    option.widget = ctx.widget;
    option.showDecorationSelected = true;

    if (theModel()->isItemSelected(id))
    {
        option.state |= QStyle::State_Selected;
        painter->setPen(ctx.widget->palette().highlightedText().color());

        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, ctx.widget);
    }

    // draw focus rect for active item
    if (theModel()->activeId() == id)
    {
        QStyleOptionFocusRect focusOption;
        focusOption.QStyleOption::operator=(option);
        focusOption.state |= QStyle::State_KeyboardFocusChange;
        style->drawPrimitive(QStyle::PE_FrameFocusRect, &focusOption, painter, ctx.widget);
    }
}

void ViewSelectionClient::cleanupDrawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& /*cache*/) const
{
    m_painterState.restore(painter);
}

ViewSelectionHeader::ViewSelectionHeader(const SharedPtr<ModelSelection> &model, SelectionHeaderType type, bool useDefaultController)
    : ViewModeled<ModelSelection>(model),
      m_type(type),
      m_pushableTracker(this)
{
    if (useDefaultController)
    {
        setController(makeShared<ControllerMouseSelectionHeader>(model, type));
    }
}

void ViewSelectionHeader::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QStyleOptionHeader option;
    ctx.initStyleOption(option);
    option.state |= m_pushableTracker.styleStateByItem(cache.id);
    option.rect = cache.cacheView.rect();

    auto activeId = theModel()->activeId();
    auto id = cache.id.as<GridID>();
    if (    ((m_type == SelectionColumnsHeader) && (activeId.row == id.row)) ||
            ((m_type == SelectionRowsHeader) && (activeId.column == id.column)))
    {
        option.state |= QStyle::State_Sunken;
    }

    if (m_type == SelectionColumnsHeader)
    {
        option.orientation = Qt::Vertical;
    }

    ctx.widget->style()->drawControl(QStyle::CE_HeaderSection, &option, painter, ctx.widget);
}

ControllerMouseSelectionClient::ControllerMouseSelectionClient(SharedPtr<ModelSelection> model)
    : ControllerMouseCaptured(ControllerMousePriorityBackground, true),
      m_model(std::move(model)),
      m_exclude(false)
{
    Q_ASSERT(m_model);
}

void ControllerMouseSelectionClient::startCapturingImpl()
{
    ControllerMouseCaptured::startCapturingImpl();

    // space of the selection model should be the same as controller's area
    Q_ASSERT(&activationState().cacheSpace.space() == &m_model->space());

    m_model->startSelectionOperation();

    m_selection = m_model->selection();
    m_startId = activationState().visibleId().as<GridID>();
    m_trackId = m_startId;
    m_exclude = m_model->isItemSelected(activationState().id.as<GridID>());
}

void ControllerMouseSelectionClient::stopCapturingImpl()
{
    m_selection.clear();
    m_startId = GridID();
    m_trackId = GridID();
    m_exclude = false;

    m_model->stopSelectionOperation();

    ControllerMouseCaptured::stopCapturingImpl();
}

bool ControllerMouseSelectionClient::processLButtonDown(QMouseEvent* event)
{
    ControllerMouseCaptured::processLButtonDown(event);

    // Ctrl key was not pressed -> clear selection
    if (!(event->modifiers() & Qt::ControlModifier))
    {
        m_selection.clear();
        m_exclude = false;
    }

    // Shift key was pressed -> make continuous selection
    // from active item to the current item
    if (event->modifiers() & Qt::ShiftModifier)
    {
        m_startId = m_model->activeVisibleId();
        applySelection(false);
    }
    else
    {
        applySelection(true);
    }

    return true;
}

bool ControllerMouseSelectionClient::processLButtonDblClick(QMouseEvent* event)
{
    ControllerMouseCaptured::processLButtonDblClick(event);

    // Ctrl key was not pressed -> clear selection
    if (!(event->modifiers() & Qt::ControlModifier))
    {
        m_selection.clear();
        m_exclude = false;
    }

    if (event->modifiers() & Qt::ShiftModifier)
    {
        m_startId = m_model->activeVisibleId();
        applySelection(false);
    }
    else
    {
        applySelection(true);
    }

    return true;
}

bool ControllerMouseSelectionClient::processMouseMove(QMouseEvent* event)
{
    ControllerMouseCaptured::processMouseMove(event);

    if (isCapturing())
    {
        auto cacheSpaceGrid = qobject_cast<const CacheSpaceGrid*>(&activationState().cacheSpace);
        GridID itemUnderPoint = cacheSpaceGrid->visibleItemByPosition(activationState().context.point);

        if (!cacheSpaceGrid->isItemInFrameStrict(itemUnderPoint))
        {
            // user moved mouse out of frame => try to scroll
            activationState().context.widgetCore->ensureVisible(ID(itemUnderPoint), cacheSpaceGrid, false);
        }

        if (m_trackId != itemUnderPoint)
        {
            // update selection if track item has changed
            m_trackId = itemUnderPoint;
            applySelection(false);
        }
    }

    return true;
}

bool ControllerMouseSelectionClient::processContextMenu(QContextMenuEvent* /*event*/)
{
    if (isCapturing())
        return false;

    auto id = activationState().id.as<GridID>();
    m_model->setActiveId(id);

    if (!m_model->isItemSelected(id))
        m_model->setSelection(makeRangeID(ID(id)));

    return false;
}

void ControllerMouseSelectionClient::applySelection(bool makeStartItemAsActive)
{
    // this should be more abstract
    auto spaceGrid = &m_model->space();
    Q_ASSERT(spaceGrid);
    if (!spaceGrid)
        return;

    auto range = makeRangeGridRect(*spaceGrid, m_startId, m_trackId);
    if (range)
    {
        // perform selection
        RangeSelection selection = m_selection;
        selection.addRange(range, m_exclude);

        m_model->applySelection(selection);
        if (makeStartItemAsActive)
            m_model->setActiveVisibleId(m_startId);
    }
}

ControllerMouseSelectionHeader::ControllerMouseSelectionHeader(SharedPtr<ModelSelection> model, SelectionHeaderType type)
    : ControllerMouseCaptured(ControllerMousePriorityBackground, true),
      m_model(std::move(model)),
      m_type(type),
      m_startLine(InvalidIndex),
      m_trackLine(InvalidIndex)
{
    Q_ASSERT(m_model);
}

static int lineIndexByHeaderType(GridID id, SelectionHeaderType type)
{
    switch (type)
    {
    case SelectionRowsHeader:
        return id.column;

    case SelectionColumnsHeader:
        return id.row;

    default:
        Q_ASSERT(false);
        return InvalidIndex;
    }
}

void ControllerMouseSelectionHeader::startCapturingImpl()
{
    ControllerMouseCaptured::startCapturingImpl();

    m_model->startSelectionOperation();

    m_selection = m_model->selection();
    if (m_type != SelectionCornerHeader)
        m_startLine = m_trackLine = lineIndexByHeaderType(activationState().visibleId().as<GridID>(), m_type);
}

void ControllerMouseSelectionHeader::stopCapturingImpl()
{
    m_selection.clear();
    m_startLine = m_trackLine = InvalidIndex;

    m_model->stopSelectionOperation();

    ControllerMouseCaptured::stopCapturingImpl();
}

bool ControllerMouseSelectionHeader::processLButtonDown(QMouseEvent* event)
{
    ControllerMouseCaptured::processLButtonDown(event);

    // Ctrl key was not pressed -> clear selection
    if (!(event->modifiers() & Qt::ControlModifier))
        m_selection.clear();

    if ((event->modifiers() & Qt::ShiftModifier))
    {
        m_startLine = lineIndexByHeaderType(m_model->activeVisibleId(), m_type);
        applySelection(false);
    }
    else
    {
        applySelection(true);
    }

    return true;
}

bool ControllerMouseSelectionHeader::processMouseMove(QMouseEvent* /*event*/)
{
    if (isCapturing() && m_type != SelectionCornerHeader)
    {
        auto cacheSpaceGrid = qobject_cast<const CacheSpaceGrid*>(&activationState().cacheSpace);
        auto itemUnderPoint = cacheSpaceGrid->visibleItemByPosition(activationState().context.point);

        if (!cacheSpaceGrid->isItemInFrameStrict(itemUnderPoint))
        {
            // user moved mouse out of frame => try to scroll
            activationState().context.widgetCore->ensureVisible(ID(itemUnderPoint), cacheSpaceGrid, false);
        }

        int lineUnderPoint = lineIndexByHeaderType(itemUnderPoint, m_type);
        if (m_trackLine != lineUnderPoint)
        {
            // update selection if track line has changed
            m_trackLine = lineUnderPoint;
            applySelection(false);
        }
    }

    return true;
}

void ControllerMouseSelectionHeader::applySelection(bool makeStartItemAsActive)
{
    Q_ASSERT(isCapturing());
    if (!isCapturing())
        return;

    RangeSelection selection = m_selection;
    auto activeId = m_model->activeId();
    const auto& space = m_model->space();

    switch (m_type)
    {
    case SelectionColumnsHeader:
    {
        QSet<int> rows;
        for (GridID id(qMin(m_startLine, m_trackLine), 0); id.row <= qMax(m_startLine, m_trackLine); ++id.row)
        {
            rows.insert(space.toGridAbsolute(id).row);
        }

        selection.addRange(makeRangeGridRows(rows), false);
        activeId.row = space.toGridAbsolute(GridID(m_startLine, 0)).row;
    }
        break;

    case SelectionRowsHeader:
    {
        QSet<int> columns;
        for (GridID id(0, qMin(m_startLine, m_trackLine)); id.column <= qMax(m_startLine, m_trackLine); ++id.column)
        {
            columns.insert(space.toGridAbsolute(id).column);
        }

        selection.addRange(makeRangeGridColumns(columns), false);
        activeId.column = space.toGridAbsolute(GridID(0, m_startLine)).column;
    }
        break;

    case SelectionCornerHeader:
    {
        selection.clear();
        selection.addRange(makeRangeAll(), false);
    }
        break;
    }

    m_model->applySelection(selection);

    if (makeStartItemAsActive && activeId.isValid())
    {
        m_model->setActiveId(activeId);
    }
}

ControllerMouseSelectionNonItems::ControllerMouseSelectionNonItems(SharedPtr<ModelSelection> model)
    : ControllerMouse(ControllerMousePriorityBackground),
      m_model(std::move(model))
{
    Q_ASSERT(m_model);
}

bool ControllerMouseSelectionNonItems::processLButtonDown(QMouseEvent* event)
{
    ControllerMouse::processLButtonDown(event);
    // clear selection if user clicks out of items
    m_model->clearSelection();
    return true;
}

ControllerKeyboardSelection::ControllerKeyboardSelection(SharedPtr<ModelSelection> model, const CacheSpace *cacheSpace, SpaceWidgetCore* widgetCore)
    : m_model(std::move(model)),
      m_cacheSpace(cacheSpace),
      m_widgetCore(widgetCore)
{
    Q_ASSERT(m_model);
    Q_ASSERT(m_cacheSpace);
    Q_ASSERT(m_widgetCore);
    Q_ASSERT(&m_cacheSpace->space() == &m_model->space());

    m_trackId = m_model->activeId();

    connect(m_model.data(), &ModelSelection::selectionChanged, this, &ControllerKeyboardSelection::onSelectionChanged);
}

ControllerKeyboardSelection::~ControllerKeyboardSelection()
{
    disconnect(m_model.data(), &ModelSelection::selectionChanged, this, &ControllerKeyboardSelection::onSelectionChanged);
}

bool ControllerKeyboardSelection::processKeyPress(QKeyEvent* event)
{
    auto spaceGrid = qobject_cast<const SpaceGrid*>(&m_model->space());
    if (!spaceGrid)
        return false;

    qDebug() << "ControllerKeyboardSelection::processKeyPress";

    if (m_pressedKeys.isEmpty())
        m_model->startSelectionOperation();
    m_pressedKeys.insert(event->key());

    GridID trackVisibleId(0, 0);

    if (m_trackId.isValid())
    {
        trackVisibleId = m_model->space().toGridVisible(m_trackId);
        // if invisible - let start from beginning
        if (!trackVisibleId.isValid())
            trackVisibleId = GridID(0, 0);
    }

    const auto& rows = spaceGrid->rows();
    const auto& columns = spaceGrid->columns();

    int rowOffset = 0;
    int columnOffset = 0;
    bool eventIsProcessed = true;

    switch (event->key())
    {
    case Qt::Key_Home:
    {
        rowOffset = 0 - trackVisibleId.row;
    } break;

    case Qt::Key_End:
    {
        rowOffset = (rows->visibleCount() - 1) - trackVisibleId.row;
    } break;

    case Qt::Key_PageUp:
    {
        rowOffset = -qMin(10, trackVisibleId.row - 0);
    } break;

    case Qt::Key_PageDown:
    {
        rowOffset = qMin(10, (rows->visibleCount() - 1) - trackVisibleId.row);
    } break;

    case Qt::Key_Up:
    {
        rowOffset = -qMin(1, trackVisibleId.row - 0);
    } break;

    case Qt::Key_Down:
    {
        rowOffset = qMin(1, (rows->visibleCount() - 1) - trackVisibleId.row);
    } break;

    case Qt::Key_Left:
    {
        columnOffset = -qMin(1, trackVisibleId.column - 0);
    } break;

    case Qt::Key_Right:
    {
        columnOffset = qMin(1, (columns->visibleCount() - 1) - trackVisibleId.column);
    } break;

    case 'a':
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            RangeSelection selection;
            selection.addRange(makeRangeAll(), false);
            m_model->applySelection(selection);
        }
        else
        {
            eventIsProcessed = false;
        }
    } break;

    case ' ':
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            m_model->addSelection(makeRangeID(ID(m_model->activeId())), m_model->isItemSelected(m_model->activeId()));
        }
        else
        {
            eventIsProcessed = false;
        }
    } break;

    default:
        eventIsProcessed = false;
    }

    if (rowOffset != 0 || columnOffset != 0)
    {
        trackVisibleId = GridID(trackVisibleId.row + rowOffset, trackVisibleId.column + columnOffset);

        if (event->modifiers() & Qt::ShiftModifier)
        {
            if (m_selection.isEmpty())
            {
                m_selection = m_model->selection();
            }

            RangeSelection selection(m_selection);
            selection.addRange(makeRangeGridRect(*spaceGrid, m_model->activeVisibleId(), trackVisibleId), false);
            m_model->applySelection(selection);
        }
        else if (event->modifiers() & Qt::ControlModifier)
        {
            m_model->setActiveVisibleId(trackVisibleId);
        }
        else
        {
            m_model->setActiveVisibleId(trackVisibleId);
            m_model->setSelection(makeRangeID(ID(m_model->activeId())));
        }

        m_widgetCore->ensureVisible(ID(trackVisibleId), m_cacheSpace, false);

        m_trackId = spaceGrid->toGridAbsolute(trackVisibleId);

        return true;
    }
    else if (!eventIsProcessed)
    {
        auto activeVisibleId = m_model->activeVisibleId();
        if (activeVisibleId.isValid())
        {
            // try to perform edit by keyboard on active cell
            return m_widgetCore->doInplaceEdit(ID(activeVisibleId), m_cacheSpace, event);
        }
    }

    return false;
}

bool ControllerKeyboardSelection::processKeyRelease(QKeyEvent* event)
{
    qDebug() << "ControllerKeyboardSelection::processKeyRelease";

    if (!m_pressedKeys.isEmpty())
    {
        m_pressedKeys.remove(event->key());
        // if all keys up -> make stop selection operation
        if (m_pressedKeys.isEmpty())
        {
            m_model->stopSelectionOperation();
        }
    }

    auto activeVisibleId = m_model->activeVisibleId();
    if (activeVisibleId.isValid())
    {
        // try to perform edit by keyboard on active cell
        return m_widgetCore->doInplaceEdit(ID(activeVisibleId), m_cacheSpace, event);
    }

    return false;
}

void ControllerKeyboardSelection::startCapturing()
{
    qDebug() << "ControllerKeyboardSelection::startCapturing";

    Q_ASSERT(m_pressedKeys.isEmpty());
}

void ControllerKeyboardSelection::stopCapturing()
{
    // number of key presses and releases may be different
    if (!m_pressedKeys.isEmpty())
    {
        m_model->stopSelectionOperation();
    }

    m_pressedKeys.clear();

    qDebug() << "ControllerKeyboardSelection::stopCapturing";
}

void ControllerKeyboardSelection::onSelectionChanged(const ModelSelection*, ModelSelection::ChangeReason reason)
{
    if (reason & ModelSelection::ChangeReasonActiveItem)
    {
        m_trackId = m_model->activeId();
        m_selection.clear();
    }
}

} // end namespace Qi
