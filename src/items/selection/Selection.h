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

#ifndef QI_SELECTION_H
#define QI_SELECTION_H

#include "core/ext/Ranges.h"
#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"
#include "core/ControllerKeyboard.h"

#include <space/grid/SpaceGrid.h>

namespace Qi
{

class WidgetDriver;
class CacheSpace;

class QI_EXPORT ModelSelection: public ModelComparable
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelSelection)

public:
    ModelSelection(SharedPtr<SpaceGrid> space);
    virtual ~ModelSelection();

    const SpaceGrid& space() const { return *m_space.data(); }

    bool isItemSelected(GridID id) const { return isItemSelectedImpl(id); }
    bool isVisibleItemSelected(GridID visibleId) const;

    void addSelection(SharedPtr<Range> range, bool exclude);
    void setSelection(SharedPtr<Range> range);
    void clearSelection();

    const RangeSelection& selection() const { return m_selection; }
    void applySelection(const RangeSelection& selection);

    GridID activeId() const { return m_activeId; }
    GridID activeVisibleId() const;
    void setActiveId(GridID id);
    void setActiveVisibleId(GridID visibleId);

    void startSelectionOperation();
    void stopSelectionOperation();

    enum ChangeReason
    {
        ChangeReasonSelection = 0x1,
        ChangeReasonActiveItem = 0x2
    };

signals:
    void selectionChanged(const ModelSelection* selection, ChangeReason changeReason);
    void selectionOperationPerformed(const ModelSelection* selection, bool started);

protected:
    int compareImpl(ID left, ID right) const override;
    bool isAscendingDefaultImpl(ID /*id*/) const override { return false; }

    virtual bool isItemSelectedImpl(GridID id) const { return m_selection.hasItem(ID(id)); }

    void emitChangedSignals(ChangeReason changeReason);

    RangeSelection m_selection;
    GridID m_activeId;
    QWeakPointer<SpaceGrid> m_space;
    int m_selectionOperations;
};

class QI_EXPORT ModelSelectionRows: public ModelSelection
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelSelectionRows)

public:
    ModelSelectionRows(SharedPtr<SpaceGrid> space)
        : ModelSelection(std::move(space))
    {}

    bool isRowSelected(int row) const;
    void selectRows(const QSet<int>& rows);

protected:
    bool isItemSelectedImpl(GridID id) const override { return isRowSelected(id.row); }
};

class QI_EXPORT ModelSelectionRow: public ModelSelection
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelSelectionRow)

public:
    ModelSelectionRow(SharedPtr<SpaceGrid> space)
        : ModelSelection(std::move(space))
    {}

    bool isRowSelected(int row) const { return activeId().row == row; }
    void selectRow(int row) { setActiveId(GridID(row, activeId().column)); }

protected:
    bool isItemSelectedImpl(GridID id) const override { return isRowSelected(id.row); }
};

class QI_EXPORT ModelSelectionColumns: public ModelSelection
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelSelectionColumns)

public:
    ModelSelectionColumns(SharedPtr<SpaceGrid> space)
        : ModelSelection(std::move(space))
    {}

    bool isColumnSelected(int column) const { return activeId().column == column; }
    void selectColumns(const QSet<int>& columns);

protected:
    bool isItemSelectedImpl(GridID id) const override { return isColumnSelected(id.column); }
};

class QI_EXPORT ViewSelectionClient: public ViewModeled<ModelSelection>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewSelectionClient)

public:
    ViewSelectionClient(const SharedPtr<ModelSelection>& model, bool useDefaultController = true);
    ~ViewSelectionClient();

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    void cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const override;

private:
    mutable PainterState m_painterState;
};

enum SelectionHeaderType
{
    SelectionRowsHeader,
    SelectionColumnsHeader,
    SelectionCornerHeader
};

class QI_EXPORT ViewSelectionHeader: public ViewModeled<ModelSelection>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewSelectionHeader)

public:
    ViewSelectionHeader(const SharedPtr<ModelSelection>& model, SelectionHeaderType type, bool useDefaultController = true);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    SelectionHeaderType m_type;
    PushableTracker m_pushableTracker;
};

class QI_EXPORT ControllerMouseSelectionClient: public ControllerMouseCaptured
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseSelectionClient)

public:
    ControllerMouseSelectionClient(SharedPtr<ModelSelection> model);

    const SharedPtr<ModelSelection>& theModel() const { return m_model; }

    bool processLButtonDown(QMouseEvent* event) override;
    bool processLButtonDblClick(QMouseEvent* event) override;
    bool processMouseMove(QMouseEvent* event) override;
    bool processContextMenu(QContextMenuEvent* event) override;

protected:
    void startCapturingImpl() override;
    void stopCapturingImpl() override;

private:
    void applySelection(bool makeStartItemAsActive);

    SharedPtr<ModelSelection> m_model;
    RangeSelection m_selection;
    GridID m_startId;
    GridID m_trackId;
    bool m_exclude;
};

class QI_EXPORT ControllerMouseSelectionHeader: public ControllerMouseCaptured
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseSelectionHeader)

public:
    ControllerMouseSelectionHeader(SharedPtr<ModelSelection> model, SelectionHeaderType type);

    bool processLButtonDown(QMouseEvent* event) override;
    bool processMouseMove(QMouseEvent* event) override;

protected:
    void startCapturingImpl() override;
    void stopCapturingImpl() override;

private:
    void applySelection(bool makeStartItemAsActive);

    SharedPtr<ModelSelection> m_model;
    SelectionHeaderType m_type;
    RangeSelection m_selection;
    int m_startLine;
    int m_trackLine;
};

class QI_EXPORT ControllerMouseSelectionNonItems: public ControllerMouse
{
public:
    ControllerMouseSelectionNonItems(SharedPtr<ModelSelection> model);

    bool processLButtonDown(QMouseEvent* event) override;

private:
    SharedPtr<ModelSelection> m_model;
};

class QI_EXPORT ControllerKeyboardSelection: public ControllerKeyboard
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerKeyboardSelection)

public:
    ControllerKeyboardSelection(SharedPtr<ModelSelection> model, const CacheSpace* cacheSpace, SpaceWidgetCore* widgetCore);
    virtual ~ControllerKeyboardSelection();

    bool processKeyPress(QKeyEvent* event) override;
    bool processKeyRelease(QKeyEvent* event) override;

    void startCapturing() override;
    void stopCapturing() override;

private:
    void onSelectionChanged(const ModelSelection*, ModelSelection::ChangeReason reason);

    SharedPtr<ModelSelection> m_model;
    const CacheSpace* m_cacheSpace;
    SpaceWidgetCore* m_widgetCore;

    GridID m_trackId;
    RangeSelection m_selection;
    QSet<int> m_pressedKeys;
};

} // end namespace Qi

#endif // QI_SELECTION_H
