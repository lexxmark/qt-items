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
#include "cache/space/CacheSpaceGrid.h"

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
    selection->setActiveItem(ItemID(1, 1));
    selection->setSelection(QSharedPointer<RangeColumn>::create(3));

    // setup items borders
    {
        auto range = QSharedPointer<RangeCallback>::create([](const ItemID& item)->bool{
            return item.column < 5;
        });
        clientGrid->addSchema(range, QSharedPointer<ViewRowBorder>::create(), makeLayoutBottom(LayoutBehaviorTransparent));
        clientGrid->addSchema(range, QSharedPointer<ViewColumnBorder>::create(), makeLayoutRight(LayoutBehaviorTransparent));
    }

    // setup alternate background
    {
        auto range = QSharedPointer<RangeCallback>::create([](const ItemID& item)->bool{
            return item.column >= 5;
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
        viewVisible->isItemVisible = [modelSorting](const ItemID& item){
            return modelSorting->activeSortingItem() == item;
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
        auto range = QSharedPointer<RangeCallback>::create();
        range->hasItemCallback = [filterByText] (const ItemID& item)-> bool {
            return item.row == 1 && !filterByText->filterByColumn(item.column).isNull();
        };
        topGrid->addSchema(range, view, makeLayoutClient());
    }

    // setup top fixed sub-grid
    {
        auto modelText = QSharedPointer<ModelTextCallback>::create();
        modelText->getValueFunction = [](const ItemID& item)->QString {
            return QString("Caption[%1, %2]").arg(item.row).arg(item.column);
        };
        topGrid->addSchema(makeRangeRow(0), QSharedPointer<ViewText>::create(modelText));
    }

    // setup left fixed sub-grid
    {
        auto modelRowNum = QSharedPointer<ModelRowNumber>::create();
        auto modelRowText = QSharedPointer<ModelNumericText<int>>::create(modelRowNum);
        leftGrid->addSchema(makeRangeColumn(0), QSharedPointer<ViewText>::create(modelRowText, ViewDefaultControllerNone, Qt::AlignRight | Qt::AlignVCenter));
    }

    // text in all items except column 5, 6, 10 and 11
    auto modelText = QSharedPointer<ModelTextCallback>::create();
    {
        modelText->getValueFunction = [](const ItemID& item)->QString {
            return QString("Item [%1, %2]").arg(item.row).arg(item.column);
        };
        auto range = QSharedPointer<RangeCallback>::create([](const ItemID& item)->bool{
            return item.column != 5 && item.column != 6 && item.column != 10 && item.column != 11;
        });
        clientGrid->addSchema(range, QSharedPointer<ViewText>::create(modelText));

        modelSorting->addSortingModel(0, modelText);
    }

    // Checkbox example
    {
        auto modelChecks = QSharedPointer<ModelStorageColumn<Qt::CheckState>>::create(clientGrid->rows());
        clientGrid->addSchema(makeRangeColumn(0), QSharedPointer<ViewCheck>::create(modelChecks), makeLayoutLeft());

        auto filter = QSharedPointer<ItemsFilterTextByText>::create(modelText);
        filterByText->addFilterByColumn(0, filter);
    }

    // radiobutton example
    {
        auto modelRadio = QSharedPointer<ModelRadioStorage>::create();
        clientGrid->addSchema(makeRangeColumn(1), QSharedPointer<ViewRadio>::create(modelRadio), makeLayoutLeft());
        modelSorting->addSortingModel(1, modelRadio);
    }

    // sort column 2 by selection
    modelSorting->addSortingModel(2, selection);

    // Button with text example
    {
        auto modelBttnText = QSharedPointer<ModelStorageValue<QString>>::create(" ... ");
        auto viewBttnText  = QSharedPointer<ViewText>::create(modelBttnText);
        auto viewBttn = QSharedPointer<ViewButton>::create(viewBttnText);
        viewBttn->tuneBttnState = [](const ItemID& item, QStyle::State& bttnState) {
            if (item.row % 2 == 0)
                bttnState |= QStyle::State_Sunken;
        };
        viewBttn->action = [](const ItemID& item,  const ControllerContext& context, const ViewButton*) {
            QMessageBox::information(context.widget, "ViewButton clicked", QString("item[%1, %2]").arg(item.row).arg(item.column));
        };
        viewBttn->setTooltipText("Click me");
        clientGrid->addSchema(makeRangeColumn(2), viewBttn, makeLayoutRight());
    }

    // Standard Pixmap example
    {
        auto viewStdIcon = QSharedPointer<ViewStyleStandardPixmap>::create(QStyle::SP_DialogOkButton);
        viewStdIcon->action = [](const ItemID& item, const ControllerContext& context, const ViewStyleStandardPixmap*) {
            QMessageBox::information(context.widget, "ViewStyleStandardPixmap clicked", QString("item[%1, %2]").arg(item.row).arg(item.column));
        };
        clientGrid->addSchema(makeRangeColumn(3), viewStdIcon, makeLayoutRight());
    }

    // QImage example
    {
        m_images[0] = QImage(":/new/img/abort-icon.png");
        m_images[1] = QImage(":/new/img/application-icon.png");
        m_images[2] = QImage(":/new/img/information-icon.png");
        auto modelImage = QSharedPointer<ModelImageCallback>::create();
        modelImage->getValueFunction = std::bind(std::mem_fn(&MainWindow::image), this, _1);
        auto viewImage = QSharedPointer<ViewImage>::create(modelImage);
        clientGrid->addSchema(makeRangeColumn(4), viewImage, makeLayoutLeft());
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
        clientGrid->addSchema(makeRangeColumn(4), viewPixmap, makeLayoutLeft());
    }

    // Link example
    {
        auto modelLink = QSharedPointer<ModelTextCallback>::create();
        modelLink->getValueFunction = [](const ItemID& item)->QString {
            return QString("Link [%1, %2]").arg(item.row).arg(item.column);
        };
        auto viewLink = QSharedPointer<ViewLink>::create(modelLink);
        viewLink->action = [](const ItemID& item, const ControllerContext& context, const ViewLink* viewLink) {
            QMessageBox::information(context.widget, "Link clicked",
                                     QString("%1 was clicked").arg(viewLink->theModel()->value(item)));
        };

        clientGrid->addSchema(makeRangeColumn(5), viewLink, makeLayoutLeft());
    }

    // progressbar example
    {
        auto modelProgress = QSharedPointer<ModelProgressCallback>::create();
        modelProgress->getValueFunction = [](const ItemID& item)->float {
            return float((item.row)%101)/100.f;
        };
        auto viewProgress = QSharedPointer<ViewProgressContents>::create(modelProgress);
        clientGrid->addSchema(makeRangeColumn(6), viewProgress, makeLayoutBackground());

        auto viewProgressLabel = QSharedPointer<ViewProgressLabel>::create(modelProgress, ProgressLabelModePercent);
        clientGrid->addSchema(makeRangeColumn(6), viewProgressLabel, makeLayoutClient());

        auto viewProgressBox = QSharedPointer<ViewProgressBox>::create(modelProgress);
        clientGrid->addSchema(makeRangeColumn(7), viewProgressBox, makeLayoutBackground());
    }

    // color example
    {
        // fill random colors
        m_colors = QSharedPointer<ModelStorageColumn<QColor>>::create(clientGrid->rows());
        for (ItemsIteratorGridByColumn it(*clientGrid, 8); it.isValid(); it.toNext())
        {
            m_colors->setValue(it.item(), QColor(Qt::GlobalColor(rand()%20)));
        }

        auto viewColor = QSharedPointer<ViewColor>::create(m_colors);
        clientGrid->addSchema(makeRangeColumn(8), viewColor, makeLayoutSquareLeft());
    }

    // color background color
    {
        auto modelColor = QSharedPointer<ModelColorCallback>::create();
        modelColor->getValueFunction = [](const ItemID& item)->QColor {
            int gradient = item.row * 255 / 99;
            return QColor(255, 0, 0, gradient);
        };

        auto viewColor = QSharedPointer<ViewColor>::create(modelColor, false, false);
        // insert view before selection
        clientGrid->insertSchema(selectionViewIndex, makeRangeColumn(9), viewColor, makeLayoutBackground());
    }

    // numeric example
    {
        auto modelNumeric = QSharedPointer<ModelCallback<double>>::create();
        modelNumeric->getValueFunction = [](const ItemID& item)->double {
            return sin((double)item.row);
        };

        auto viewText = QSharedPointer<ViewText>::create(QSharedPointer<ModelNumericText<double>>::create(modelNumeric));
        clientGrid->addSchema(makeRangeColumn(10), viewText);
        modelSorting->addSortingModel(10, modelNumeric);
    }

    // enum example
    {
        auto traits = QSharedPointer<ColorEnumTraits>::create();
        auto modelEnumValues= QSharedPointer<ModelCallback<COLORS>>::create();
        modelEnumValues->getValueFunction = [](const ItemID& item)->COLORS {
            return COLORS(item.row%3);
        };
        auto modelEnum = QSharedPointer<ModelEnum<COLORS>>::create(traits, modelEnumValues);
        auto viewEnum = QSharedPointer<ViewEnumText<COLORS>>::create(modelEnum);
        clientGrid->addSchema(makeRangeColumn(11), viewEnum);
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

QImage MainWindow::image(const Qi::ItemID& item) const
{
    return m_images[item.row%3];
}

QPixmap MainWindow::pixmap(const Qi::ItemID& item) const
{
    return m_pixmaps[item.row%3];
}

bool MainWindow::pixmapTooltip(const Qi::ItemID& item, QString& text) const
{
    int index = item.row%3;
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

