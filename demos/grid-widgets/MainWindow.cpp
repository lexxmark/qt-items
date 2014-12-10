#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "core/Layout.h"
#include "core/ext/Ranges.h"
#include "core/ext/ModelStore.h"
#include "items/checkbox/Check.h"
#include "items/radiobutton/Radio.h"
#include "items/button/Button.h"
#include "items/text/Text.h"
#include "items/selection/Selection.h"

#include "cache/space/CacheSpaceGrid.h"

#include <QMessageBox>

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto fixedGrid = ui->gridWidget->subGrid(topLeftID);

    auto clientGrid = ui->gridWidget->subGrid();
    clientGrid->setDimensions(100, 100);
    clientGrid->rows()->setLineSizeAll(25);
    clientGrid->columns()->setLineSizeAll(100);

    auto topGrid = ui->gridWidget->subGrid(topID);
    topGrid->setRowsCount(2);
    topGrid->rows()->setLineSizeAll(25);

    auto leftGrid = ui->gridWidget->subGrid(leftID);
    leftGrid->setColumnsCount(2);
    leftGrid->columns()->setLineSizeAll(40);

    auto selection = QSharedPointer<ModelSelection>::create(clientGrid);
    selection->setActiveItem(ItemID(1, 1));
    selection->setSelection(QSharedPointer<RangeColumn>::create(3));

    clientGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionClient>::create(selection), makeLayoutBackground());
    topGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionHeader>::create(selection, SelectionRowsHeader), makeLayoutBackground());
    leftGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionHeader>::create(selection, SelectionColumnsHeader), makeLayoutBackground());
    fixedGrid->addSchema(makeRangeAll(), QSharedPointer<ViewSelectionHeader>::create(selection, SelectionCornerHeader), makeLayoutBackground());
    ui->gridWidget->setControllerKeyboard(QSharedPointer<ControllerKeyboardSelection>::create(selection, &ui->gridWidget->cacheSubGrid(clientID), ui->gridWidget));

    auto modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item)->QString {
        return QString("Item [%1, %2]").arg(item.row).arg(item.column);
    };
    clientGrid->addSchema(makeRangeAll(), QSharedPointer<ViewText>::create(modelText));

    auto modelChecks = QSharedPointer<ModelStorageValue<Qt::CheckState>>::create(Qt::Checked);
    clientGrid->addSchema(makeRangeColumn(0), QSharedPointer<ViewCheck>::create(modelChecks), makeLayoutLeft());

    auto modelRadio = QSharedPointer<ModelRadioStorage>::create();
    clientGrid->addSchema(makeRangeColumn(1), QSharedPointer<ViewRadio>::create(modelRadio), makeLayoutLeft());

    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item)->QString {
        return QString("Top Item [%1, %2]").arg(item.row).arg(item.column);
    };
    topGrid->addSchema(makeRangeAll(), QSharedPointer<ViewText>::create(modelText));

    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item)->QString {
        return QString("Left Item [%1, %2]").arg(item.row).arg(item.column);
    };
    leftGrid->addSchema(makeRangeAll(), QSharedPointer<ViewText>::create(modelText));

    modelRadio = QSharedPointer<ModelRadioStorage>::create(ItemID(0, 5));
    leftGrid->addSchema(makeRangeColumn(0), QSharedPointer<ViewRadio>::create(modelRadio), makeLayoutLeft());

    auto modelBttnText = QSharedPointer<ModelStorageValue<QString>>::create(" ... ");
    auto viewBttnText  = QSharedPointer<ViewText>::create(modelBttnText);
    auto viewBttn = QSharedPointer<ViewButton>::create(viewBttnText);
    viewBttn->bttnAction = [](const ItemID& item,  const ControllerContext& context, const ViewButton*) {
        QMessageBox::information(context.widget, "Bttn clicked", QString("Bttn[%1, %2]").arg(item.row).arg(item.column));
    };
    viewBttn->setTooltipText("Click me");

    clientGrid->addSchema(makeRangeColumn(2), viewBttn, makeLayoutRight());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
