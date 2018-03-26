#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "core/Layout.h"
#include "core/ext/Ranges.h"
#include "core/ext/ModelStore.h"
#include "items/misc/ViewItemBorder.h"
#include "items/misc/ViewAlternateBackground.h"
#include "items/misc/ControllerMouseLinesResizer.h"
#include "items/visible/Visible.h"
#include "items/sorting/Sorting.h"
#include "items/filter/FilterText.h"
#include "items/checkbox/Check.h"
#include "items/radiobutton/Radio.h"
#include "items/button/Button.h"
#include "items/text/Text.h"
#include "items/numeric/Numeric.h"
#include "items/selection/Selection.h"
#include "items/image/StyleStandardPixmap.h"
#include "items/image/Image.h"
#include "items/image/Pixmap.h"
#include "items/link/Link.h"
#include "items/progressbar/Progress.h"
#include "items/enum/Enum.h"

#include "misc/GridColumnsResizer.h"
#include "space/grid/CacheSpaceGrid.h"
#include "space/grid/RangeGrid.h"

#include <QMessageBox>

#include <functional>
#include <cstdlib>
#include <time.h>

using namespace Qi;

enum COLORS
{
    RED = 0,
    BLUE,
    WHITE
};

class ColorEnumTraits: public EnumTraits<COLORS>
{
protected:
    QVector<COLORS> uniqueValuesImpl() const override
    {
        QVector<COLORS> colors;
        colors << RED << BLUE << WHITE;
        return colors;
    }
    QString valueTextImpl(COLORS value) const override
    {
        switch (value)
        {
        case RED: return "Red";
        case BLUE: return "Blue";
        case WHITE: return "White";
        default: return "<NA>";
        }
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    using namespace std::placeholders;

     srand(time(nullptr));

    ui->setupUi(this);

    auto fixedGrid = ui->gridWidget->subGrid(topLeftID);

    auto clientGrid = ui->gridWidget->subGrid();
    clientGrid->setDimensions(100, 100);
    clientGrid->rows()->setLineSizeAll(25);
    clientGrid->columns()->setLineSizeAll(150);

    auto topGrid = ui->gridWidget->subGrid(topID);
    topGrid->setRowsCount(2);
    topGrid->rows()->setLineSizeAll(25);

    auto leftGrid = ui->gridWidget->subGrid(leftID);
    leftGrid->setColumnsCount(1);
    leftGrid->columns()->setLineSizeAll(40);

    auto selection = QSharedPointer<ModelSelection>::create(clientGrid);
    selection->setActiveId(GridID(1, 1));
    selection->setSelection(QSharedPointer<RangeGridColumn>::create(3));

    // setup items borders
    {
        auto range = QSharedPointer<RangeGridCallback>::create([](GridID id)->bool{
            return id.column < 5;
        });
        clientGrid->addSchema(range, QSharedPointer<ViewRowBorder>::create(), makeLayoutBottom(LayoutBehaviorTransparent));
        clientGrid->addSchema(range, QSharedPointer<ViewColumnBorder>::create(), makeLayoutRight(LayoutBehaviorTransparent));
    }

    // setup alternate background
    {
        auto range = QSharedPointer<RangeGridCallback>::create([](GridID id)->bool{
            return id.column >= 5;
        });
        clientGrid->addSchema(range, QSharedPointer<ViewAlternateBackground>::create(), makeLayoutBackground());
    }

    int selectionViewIndex = 0;
    // setup selection
    {
        selectionViewIndex = clientGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionClient>::create(selection), makeLayoutBackground());
        topGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionHeader>::create(selection, SelectionRowsHeader), makeLayoutBackground());
        leftGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionHeader>::create(selection, SelectionColumnsHeader), makeLayoutBackground());
        fixedGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionHeader>::create(selection, SelectionCornerHeader), makeLayoutBackground());
        ui->gridWidget->setControllerKeyboard(QSharedPointer<ControllerKeyboardSelection>::create(selection, ui->gridWidget->cacheSubGrid(clientID).data(), ui->gridWidget));
    }

    // setup controllers to resize columns and rows
    {
        auto rangeAll = makeRangeAll();
        auto layoutRows = makeLayoutFixedBottom(3, LayoutBehaviorTransparent);
        auto layoutColumns = makeLayoutFixedRight(3);

        auto view = QSharedPointer<View>::create();
        QSharedPointer<ControllerMouse> controller = QSharedPointer<ControllerMouseColumnsResizer>::create(topGrid->columns());
        view->addController(controller);
        controller = QSharedPointer<ControllerMouseColumnsAutoFit>::create(ui->gridWidget, topID.column);
        view->addController(controller);
        topGrid->addSchema(rangeAll, view, layoutColumns);

        view = QSharedPointer<View>::create();
        controller = QSharedPointer<ControllerMouseRowsResizer>::create(leftGrid->rows());
        view->addController(controller);
        leftGrid->addSchema(rangeAll, view, layoutRows);

        view = QSharedPointer<View>::create();
        controller = QSharedPointer<ControllerMouseColumnsResizer>::create(fixedGrid->columns());
        view->addController(controller);
        fixedGrid->addSchema(rangeAll, view, layoutColumns);

        view = QSharedPointer<View>::create();
        controller = QSharedPointer<ControllerMouseRowsResizer>::create(fixedGrid->rows());
        view->addController(controller);
        fixedGrid->addSchema(rangeAll, view, layoutRows);
    }

    // setup sortings
    auto modelSorting = QSharedPointer<ModelGridSorting>::create(clientGrid);
    {
        auto viewSorting = QSharedPointer<ViewGridSorting>::create(modelSorting);
        auto viewVisible = QSharedPointer<ViewVisible>::create(viewSorting);
        viewVisible->isItemVisible = [modelSorting](ID id){
            return modelSorting->activeSortingId() == id.as<GridID>();
        };

        auto view = QSharedPointer<View>::create();
        view->setController(QSharedPointer<ControllerMouseVisible>::create(viewVisible));

        topGrid->addSchema(makeRangeGridSorter(modelSorting), view, makeLayoutBackground());
        topGrid->addSchema(makeRangeGridSorter(modelSorting), viewVisible, makeLayoutSquareRight());
    }

    // setup filtering
    auto filterByText = QSharedPointer<RowsFilterByText>::create();
    clientGrid->rows()->addLinesVisibility(filterByText);
    {
        auto view = makeViewRowsFilterByText(filterByText);
        auto range = QSharedPointer<RangeGridCallback>::create();
        range->hasItemCallback = [filterByText] (GridID id)-> bool {
            return id.row == 1 && !filterByText->filterByColumn(id.column).isNull();
        };
        topGrid->addSchema(range, view, makeLayoutClient());
    }

    // setup top fixed sub-grid
    {
        auto modelText = QSharedPointer<ModelTextCallback>::create();
        modelText->getValueFunction = [](ID id)->QString {
            return QString("Caption[%1, %2]").arg(row(id)).arg(column(id));
        };
        topGrid->addSchema(makeRangeGridRow(0), QSharedPointer<ViewText>::create(modelText));
    }

    // setup left fixed sub-grid
    {
        auto modelRowNum = QSharedPointer<ModelRowNumber>::create();
        auto modelRowText = QSharedPointer<ModelNumericText<int>>::create(modelRowNum);
        leftGrid->addSchema(makeRangeGridColumn(0), QSharedPointer<ViewText>::create(modelRowText, ViewDefaultControllerNone, Qt::AlignRight | Qt::AlignVCenter));
    }

    // text in all items except column 5, 6, 10 and 11
    auto modelText = QSharedPointer<ModelTextCallback>::create();
    {
        modelText->getValueFunction = [](ID id)->QString {
            return QString("Item [%1, %2]").arg(row(id)).arg(column(id));
        };
        auto range = QSharedPointer<RangeCallback>::create([](ID id)->bool{
            return column(id) != 5 && column(id) != 6 && column(id) != 10 && column(id) != 11;
        });
        clientGrid->addSchema(range, QSharedPointer<ViewText>::create(modelText));

        modelSorting->addSortingModel(0, modelText);
    }

    // Checkbox example
    {
        auto modelChecks = QSharedPointer<ModelStorageColumn<Qt::CheckState>>::create(clientGrid->rows());
        clientGrid->addSchema(makeRangeGridColumn(0), QSharedPointer<ViewCheck>::create(modelChecks), makeLayoutLeft());

        auto filter = QSharedPointer<ItemsFilterTextByText>::create(modelText);
        filterByText->addFilterByColumn(0, filter);
    }

    // radiobutton example
    {
        auto modelRadio = QSharedPointer<ModelRadioStorage>::create(makeID<GridID>());
        clientGrid->addSchema(makeRangeGridColumn(1), QSharedPointer<ViewRadio>::create(modelRadio), makeLayoutLeft());
        modelSorting->addSortingModel(1, modelRadio);
    }

    // sort column 2 by selection
    modelSorting->addSortingModel(2, selection);

    // Button with text example
    {
        auto modelBttnText = QSharedPointer<ModelStorageValue<QString>>::create(" ... ");
        auto viewBttnText  = QSharedPointer<ViewText>::create(modelBttnText);
        auto viewBttn = QSharedPointer<ViewButton>::create(viewBttnText);
        viewBttn->tuneBttnState = [](ID id, QStyle::State& bttnState) {
            if (row(id) % 2 == 0)
                bttnState |= QStyle::State_Sunken;
        };
        viewBttn->action = [](ID id,  const ControllerContext& context, const ViewButton*) {
            QMessageBox::information(context.widget, "ViewButton clicked", QString("id[%1, %2]").arg(row(id)).arg(column(id)));
        };
        viewBttn->setTooltipText("Click me");
        clientGrid->addSchema(makeRangeGridColumn(2), viewBttn, makeLayoutRight());
    }

    // Standard Pixmap example
    {
        auto viewStdIcon = QSharedPointer<ViewStyleStandardPixmap>::create(QStyle::SP_DialogOkButton);
        viewStdIcon->action = [](ID id, const ControllerContext& context, const ViewStyleStandardPixmap*) {
            QMessageBox::information(context.widget, "ViewStyleStandardPixmap clicked", QString("id[%1, %2]").arg(row(id)).arg(column(id)));
        };
        clientGrid->addSchema(makeRangeGridColumn(3), viewStdIcon, makeLayoutRight());
    }

    // QImage example
    {
        m_images[0] = QImage(":/new/img/abort-icon.png");
        m_images[1] = QImage(":/new/img/application-icon.png");
        m_images[2] = QImage(":/new/img/information-icon.png");
        auto modelImage = QSharedPointer<ModelImageCallback>::create();
        modelImage->getValueFunction = std::bind(std::mem_fn(&MainWindow::image), this, _1);
        auto viewImage = QSharedPointer<ViewImage>::create(modelImage);
        clientGrid->addSchema(makeRangeGridColumn(4), viewImage, makeLayoutLeft());
    }

    // QPixmap example
    {
        m_pixmaps[0] = QPixmap(":/new/img/bubble-icon.png");
        m_pixmaps[1] = QPixmap(":/new/img/calendar-icon.png");
        m_pixmaps[2] = QPixmap(":/new/img/home-icon.png");
        auto modelPixmap = QSharedPointer<ModelPixmapCallback>::create();
        modelPixmap->getValueFunction = std::bind(std::mem_fn(&MainWindow::pixmap), this, _1);
        auto viewPixmap = QSharedPointer<ViewPixmap>::create(modelPixmap);
        viewPixmap->tooltipTextCallback = std::bind(std::mem_fn(&MainWindow::pixmapTooltip), this, _1, _2);
        clientGrid->addSchema(makeRangeGridColumn(4), viewPixmap, makeLayoutLeft());
    }

    // Link example
    {
        auto modelLink = QSharedPointer<ModelTextCallback>::create();
        modelLink->getValueFunction = [](ID id)->QString {
            return QString("Link [%1, %2]").arg(row(id)).arg(column(id));
        };
        auto viewLink = QSharedPointer<ViewLink>::create(modelLink);
        viewLink->action = [](ID id, const ControllerContext& context, const ViewLink* viewLink) {
            QMessageBox::information(context.widget, "Link clicked",
                                     QString("%1 was clicked").arg(viewLink->theModel()->value(id)));
        };

        clientGrid->addSchema(makeRangeGridColumn(5), viewLink, makeLayoutLeft());
    }

    // progressbar example
    {
        auto modelProgress = QSharedPointer<ModelProgressCallback>::create();
        modelProgress->getValueFunction = [](ID id)->float {
            return float((row(id))%101)/100.f;
        };
        auto viewProgress = QSharedPointer<ViewProgressContents>::create(modelProgress);
        clientGrid->addSchema(makeRangeGridColumn(6), viewProgress, makeLayoutBackground());

        auto viewProgressLabel = QSharedPointer<ViewProgressLabel>::create(modelProgress, ProgressLabelModePercent);
        clientGrid->addSchema(makeRangeGridColumn(6), viewProgressLabel, makeLayoutClient());

        auto viewProgressBox = QSharedPointer<ViewProgressBox>::create(modelProgress);
        clientGrid->addSchema(makeRangeGridColumn(7), viewProgressBox, makeLayoutBackground());
    }

    // color example
    {
        // fill random colors
        m_colors = QSharedPointer<ModelStorageColumn<QColor>>::create(clientGrid->rows());
        for (ItemsIteratorGridByColumn it(*clientGrid, 8); it.isValid(); it.toNext())
        {
            m_colors->setValue(it.id(), QColor(Qt::GlobalColor(rand()%20)));
        }

        auto viewColor = QSharedPointer<ViewColor>::create(m_colors);
        clientGrid->addSchema(makeRangeGridColumn(8), viewColor, makeLayoutSquareLeft());
    }

    // color background color
    {
        auto modelColor = QSharedPointer<ModelColorCallback>::create();
        modelColor->getValueFunction = [](ID id)->QColor {
            int gradient = row(id) * 255 / 99;
            return QColor(255, 0, 0, gradient);
        };

        auto viewColor = QSharedPointer<ViewColor>::create(modelColor, false, false);
        // insert view before selection
        clientGrid->insertSchema(selectionViewIndex, makeRangeGridColumn(9), viewColor, makeLayoutBackground());
    }

    // numeric example
    {
        auto modelNumeric = QSharedPointer<ModelCallback<double>>::create();
        modelNumeric->getValueFunction = [](ID id)->double {
            return sin((double)row(id));
        };

        auto viewText = QSharedPointer<ViewText>::create(QSharedPointer<ModelNumericText<double>>::create(modelNumeric));
        clientGrid->addSchema(makeRangeGridColumn(10), viewText);
        modelSorting->addSortingModel(10, modelNumeric);
    }

    // enum example
    {
        auto traits = QSharedPointer<ColorEnumTraits>::create();
        auto modelEnumValues= QSharedPointer<ModelCallback<COLORS>>::create();
        modelEnumValues->getValueFunction = [](ID id)->COLORS {
            return COLORS(row(id)%3);
        };
        auto modelEnum = QSharedPointer<ModelEnum<COLORS>>::create(traits, modelEnumValues);
        auto viewEnum = QSharedPointer<ViewEnumText<COLORS>>::create(modelEnum);
        clientGrid->addSchema(makeRangeGridColumn(11), viewEnum);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}

QImage MainWindow::image(Qi::ID id) const
{
    return m_images[row(id)%3];
}

QPixmap MainWindow::pixmap(Qi::ID id) const
{
    return m_pixmaps[row(id)%3];
}

bool MainWindow::pixmapTooltip(ID id, QString& text) const
{
    int index = row(id)%3;
    switch (index)
    {
    case 0:
        text = "Bubble";
        return true;
    case 1:
        text = "Calendar";
        return true;
    case 2:
        text = "Home";
        return true;
    default:
        return false;
    }
}

