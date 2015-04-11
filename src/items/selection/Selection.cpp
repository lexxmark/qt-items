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
#include "cache/space/CacheSpaceGrid.h"
#include "widgets/core/SpaceWidgetCore.h"
#include <QStyleOptionViewItem>

namespace Qi
{

ModelSelection::ModelSelection(const QSharedPointer<Space>& space)
    : m_space(space),
      m_selectionOperations(0)
{
    Q_ASSERT(space);
}

ModelSelection::~ModelSelection()
{
    Q_ASSERT(m_selectionOperations == 0);
}

bool ModelSelection::isVisibleItemSelected(const ItemID& visibleItem) const
{
    if (!visibleItem.isValid())
        return false;

    if (!m_space)
        return false;

    ItemID absItem = m_space.data()->toAbsolute(visibleItem);
    if (!absItem.isValid())
        return false;

    return isItemSelected(absItem);
}

void ModelSelection::addSelection(const QSharedPointer<Range>& range, bool exclude)
{
    m_selection.addRange(range, exclude);
    emitChangedSignals(ChangeReasonSelection);
}

void ModelSelection::setSelection(const QSharedPointer<Range>& range)
{
    m_selection.clear();
    m_selection.addRange(range, false);
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

ItemID ModelSelection::activeVisibleItem() const
{
    return m_space ? m_space.data()->toVisible(m_activeItem) : ItemID();
}

void ModelSelection::setActiveItem(const ItemID& item)
{
    if (m_activeItem != item)
    {
        m_activeItem = item;
        emitChangedSignals(ChangeReasonActiveItem);
    }
}

void ModelSelection::setActiveVisibleItem(const ItemID& visibleItem)
{
    Q_ASSERT(m_space);
    if (!m_space)
        return;

    setActiveItem(m_space.data()->toAbsolute(visibleItem));
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

int ModelSelection::compareImpl(const ItemID &left, const ItemID &right) const
{
    return (int)isItemSelected(left) - (int)isItemSelected(right);
}

void ModelSelection::emitChangedSignals(ChangeReason changeReason)
{
    emit modelChanged(this);

    startSelectionOperation();
    emit selectionChanged(this, changeReason);
    stopSelectionOperation();
}

void ModelSelectionRows::selectRows(const QSet<int>& rows)
{
    setSelection(QSharedPointer<RangeRows>::create(rows));
}

void ModelSelectionColumns::selectColumns(const QSet<int>& columns)
{
    setSelection(QSharedPointer<RangeColumns>::create(columns));
}

ViewSelectionClient::ViewSelectionClient(const QSharedPointer<ModelSelection>& model, bool useDefaultController)
    : ViewModeled<ModelSelection>(model)
{
    // don't use this view in copy operations by default
    setExcludeApplicationMask(ViewApplicationCopy);

    if (useDefaultController)
    {
        setController(QSharedPointer<ControllerMouseSelectionClient>::create(model));
    }
}

ViewSelectionClient::~ViewSelectionClient()
{
}

void ViewSelectionClient::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    m_painterState.save(painter);

    auto style = ctx.style();

    // cannot use drawPrimitive(QStyle::PE_PanelItemViewItem) from QWindowsVistaStyle class
    if (style->inherits("QWindowsVistaStyle"))
    {
        QPalette::ColorGroup cg = ctx.colorGroup();

        if (theModel()->isItemSelected(cache.item))
        {
            painter->setPen(ctx.widget->palette().color(cg, QPalette::HighlightedText));
            painter->fillRect(cache.cacheView.rect(), ctx.widget->palette().brush(cg, QPalette::Highlight));
        }

        // draw focus rect for active item
        if ((cg == QPalette::Active) && (theModel()->activeItem() == cache.item))
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

    if (theModel()->isItemSelected(cache.item))
    {
        option.state |= QStyle::State_Selected;
        painter->setPen(ctx.widget->palette().highlightedText().color());

        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, ctx.widget);
    }

    // draw focus rect for active item
    if (theModel()->activeItem() == cache.item)
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

ViewSelectionHeader::ViewSelectionHeader(const QSharedPointer<ModelSelection>& model, SelectionHeaderType type, bool useDefaultController)
    : ViewModeled<ModelSelection>(model),
      m_type(type),
      m_pushableTracker(this)
{
    // don't use this view in copy operations by default
    setExcludeApplicationMask(ViewApplicationCopy);

    if (useDefaultController)
    {
        setController(QSharedPointer<ControllerMouseSelectionHeader>::create(model, type));
    }
}

void ViewSelectionHeader::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QStyleOptionHeader option;
    ctx.initStyleOption(option);
    option.state |= m_pushableTracker.styleStateByItem(cache.item);
    option.rect = cache.cacheView.rect();

    ItemID activeItem = theModel()->activeItem();
    if (    ((m_type == SelectionColumnsHeader) && (activeItem.row == cache.item.row)) ||
            ((m_type == SelectionRowsHeader) && (activeItem.column == cache.item.column)))
    {
        option.state |= QStyle::State_Sunken;
    }

    if (m_type == SelectionColumnsHeader)
    {
        option.orientation = Qt::Vertical;
    }

    ctx.widget->style()->drawControl(QStyle::CE_HeaderSection, &option, painter, ctx.widget);
}

ControllerMouseSelectionClient::ControllerMouseSelectionClient(const QSharedPointer<ModelSelection>& model)
    : ControllerMouseCaptured(ControllerMousePriorityBackground, true),
      m_model(model),
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
    m_startItem = activationState().visibleItem();
    m_trackItem = m_startItem;
    m_exclude = m_model->isItemSelected(activationState().item);
}

void ControllerMouseSelectionClient::stopCapturingImpl()
{
    m_selection.clear();
    m_startItem = ItemID();
    m_trackItem = ItemID();
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
        m_startItem = m_model->activeVisibleItem();
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
        m_startItem = m_model->activeVisibleItem();
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
        ItemID itemUnderPoint = cacheSpaceGrid->visibleItemByPosition(activationState().context.point);

        if (!cacheSpaceGrid->isItemInFrameStrict(itemUnderPoint))
        {
            // user moved mouse out of frame => try to scroll
            activationState().context.widgetCore->ensureVisible(itemUnderPoint, cacheSpaceGrid, false);
        }

        if (m_trackItem != itemUnderPoint)
        {
            // update selection if track item has changed
            m_trackItem = itemUnderPoint;
            applySelection(false);
        }
    }

    return true;
}

bool ControllerMouseSelectionClient::processContextMenu(QContextMenuEvent* /*event*/)
{
    if (isCapturing())
        return false;

    ItemID item = activationState().item;
    m_model->setActiveItem(item);

    if (!m_model->isItemSelected(item))
        m_model->setSelection(makeRangeItem(item));

    return false;
}

void ControllerMouseSelectionClient::applySelection(bool makeStartItemAsActive)
{
    // this should be more abstract
    auto spaceGrid = qobject_cast<const SpaceGrid*>(&m_model->space());
    Q_ASSERT(spaceGrid);
    if (!spaceGrid)
        return;

    auto range = createItemRangeRect(*spaceGrid, m_startItem, m_trackItem);
    if (range)
    {
        // perform selection
        RangeSelection selection = m_selection;
        selection.addRange(range, m_exclude);

        m_model->applySelection(selection);
        if (makeStartItemAsActive)
            m_model->setActiveVisibleItem(m_startItem);
    }
}

ControllerMouseSelectionHeader::ControllerMouseSelectionHeader(const QSharedPointer<ModelSelection>& model, SelectionHeaderType type)
    : ControllerMouseCaptured(ControllerMousePriorityBackground, true),
      m_model(model),
      m_type(type),
      m_startLine(InvalidIndex),
      m_trackLine(InvalidIndex)
{
    Q_ASSERT(m_model);
}

static int lineIndexByHeaderType(const ItemID& item, SelectionHeaderType type)
{
    switch (type)
    {
    case SelectionRowsHeader:
        return item.column;

    case SelectionColumnsHeader:
        return item.row;

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
        m_startLine = m_trackLine = lineIndexByHeaderType(activationState().visibleItem(), m_type);
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
        m_startLine = lineIndexByHeaderType(m_model->activeVisibleItem(), m_type);
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
        ItemID itemUnderPoint = cacheSpaceGrid->visibleItemByPosition(activationState().context.point);

        if (!cacheSpaceGrid->isItemInFrameStrict(itemUnderPoint))
        {
            // user moved mouse out of frame => try to scroll
            activationState().context.widgetCore->ensureVisible(itemUnderPoint, cacheSpaceGrid, false);
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
    ItemID activeItem = m_model->activeItem();
    const auto& space = m_model->space();

    switch (m_type)
    {
    case SelectionColumnsHeader:
    {
        QSet<int> rows;
        for (ItemID item(qMin(m_startLine, m_trackLine), 0); item.row <= qMax(m_startLine, m_trackLine); ++item.row)
        {
            rows.insert(space.toAbsolute(item).row);
        }

        selection.addRange(makeRangeRows(rows), false);
        activeItem.row = space.toAbsolute(ItemID(m_startLine, 0)).row;
    }
        break;

    case SelectionRowsHeader:
    {
        QSet<int> columns;
        for (ItemID item(0, qMin(m_startLine, m_trackLine)); item.column <= qMax(m_startLine, m_trackLine); ++item.column)
        {
            columns.insert(space.toAbsolute(item).column);
        }

        selection.addRange(makeRangeColumns(columns), false);
        activeItem.column = space.toAbsolute(ItemID(0, m_startLine)).column;
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

    if (makeStartItemAsActive && activeItem.isValid())
    {
        m_model->setActiveItem(activeItem);
    }
}

ControllerMouseSelectionNonItems::ControllerMouseSelectionNonItems(const QSharedPointer<ModelSelection>& model)
    : ControllerMouse(ControllerMousePriorityBackground),
      m_model(model)
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

ControllerKeyboardSelection::ControllerKeyboardSelection(const QSharedPointer<ModelSelection>& model, const CacheSpace *cacheSpace, SpaceWidgetCore* widgetCore)
    : m_model(model),
      m_cacheSpace(cacheSpace),
      m_widgetCore(widgetCore)
{
    Q_ASSERT(m_model);
    Q_ASSERT(m_cacheSpace);
    Q_ASSERT(m_widgetCore);
    Q_ASSERT(&m_cacheSpace->space() == &m_model->space());

    m_trackItem = m_model->activeItem();

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

    ItemID trackVisibleItem(0, 0);

    if (m_trackItem.isValid())
    {
        trackVisibleItem = m_model->space().toVisible(m_trackItem);
        // if invisible - let start from beginning
        if (!trackVisibleItem.isValid())
            trackVisibleItem = ItemID(0, 0);
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
        rowOffset = 0 - trackVisibleItem.row;
    } break;

    case Qt::Key_End:
    {
        rowOffset = (rows->visibleCount() - 1) - trackVisibleItem.row;
    } break;

    case Qt::Key_PageUp:
    {
        rowOffset = -qMin(10, trackVisibleItem.row - 0);
    } break;

    case Qt::Key_PageDown:
    {
        rowOffset = qMin(10, (rows->visibleCount() - 1) - trackVisibleItem.row);
    } break;

    case Qt::Key_Up:
    {
        rowOffset = -qMin(1, trackVisibleItem.row - 0);
    } break;

    case Qt::Key_Down:
    {
        rowOffset = qMin(1, (rows->visibleCount() - 1) - trackVisibleItem.row);
    } break;

    case Qt::Key_Left:
    {
        columnOffset = -qMin(1, trackVisibleItem.column - 0);
    } break;

    case Qt::Key_Right:
    {
        columnOffset = qMin(1, (columns->visibleCount() - 1) - trackVisibleItem.column);
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
            m_model->addSelection(makeRangeItem(m_model->activeItem()), m_model->isItemSelected(m_model->activeItem()));
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
        trackVisibleItem = ItemID(trackVisibleItem.row + rowOffset, trackVisibleItem.column + columnOffset);

        if (event->modifiers() & Qt::ShiftModifier)
        {
            if (m_selection.isEmpty())
            {
                m_selection = m_model->selection();
            }

            RangeSelection selection(m_selection);
            selection.addRange(createItemRangeRect(*spaceGrid, m_model->activeVisibleItem(), trackVisibleItem), false);
            m_model->applySelection(selection);
        }
        else if (event->modifiers() & Qt::ControlModifier)
        {
            m_model->setActiveVisibleItem(trackVisibleItem);
        }
        else
        {
            m_model->setActiveVisibleItem(trackVisibleItem);
            m_model->setSelection(makeRangeItem(m_model->activeItem()));
        }

        m_widgetCore->ensureVisible(trackVisibleItem, m_cacheSpace, false);

        m_trackItem = spaceGrid->toAbsolute(trackVisibleItem);

        return true;
    }
    else if (!eventIsProcessed)
    {
        ItemID activeVisibleItem = m_model->activeVisibleItem();
        if (activeVisibleItem.isValid())
        {
            // try to perform edit by keyboard on active cell
            return m_widgetCore->doInplaceEdit(activeVisibleItem, m_cacheSpace, event);
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

    ItemID activeVisibleItem = m_model->activeVisibleItem();
    if (activeVisibleItem.isValid())
    {
        // try to perform edit by keyboard on active cell
        return m_widgetCore->doInplaceEdit(activeVisibleItem, m_cacheSpace, event);
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
        m_trackItem = m_model->activeItem();
        m_selection.clear();
    }
}

} // end namespace Qi
