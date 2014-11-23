#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "core/Layout.h"
#include "core/ext/Ranges.h"
#include "core/ext/ModelStore.h"
#include "items/checkbox/Check.h"
#include "items/radiobutton/Radio.h"
#include "items/text/Text.h"

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto& clientGrid = ui->gridWidget->subGrid();
    clientGrid.setDimensions(100, 100);
    clientGrid.rows()->setLineSizeAll(25);
    clientGrid.columns()->setLineSizeAll(100);

    auto& topGrid = ui->gridWidget->subGrid(topID);
    topGrid.setRowsCount(2);
    topGrid.rows()->setLineSizeAll(25);

    auto& leftGrid = ui->gridWidget->subGrid(leftID);
    leftGrid.setColumnsCount(2);
    leftGrid.columns()->setLineSizeAll(40);

    auto modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item)->QString {
        return QString("Item [%1, %2]").arg(item.row).arg(item.column);
    };
    clientGrid.addSchema(makeRangeAll(), QSharedPointer<ViewText>::create(modelText));

    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item)->QString {
        return QString("Top Item [%1, %2]").arg(item.row).arg(item.column);
    };
    topGrid.addSchema(makeRangeAll(), QSharedPointer<ViewText>::create(modelText));

    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item)->QString {
        return QString("Left Item [%1, %2]").arg(item.row).arg(item.column);
    };
    leftGrid.addSchema(makeRangeAll(), QSharedPointer<ViewText>::create(modelText));

    /*
    QSharedPointer<View> view;
    QSharedPointer<Layout> layout;
    QSharedPointer<Range> range;

    ui->listWidget->grid().rows().setCount(100);
    ui->listWidget->grid().columns().setCount(10);
    ui->listWidget->grid().columns().setAllLinesSize(100);

    view.reset(new ViewCheck());
    layout.reset(new LayoutLeft());
    range.reset(new RangeColumn(1));
    ui->listWidget->grid().addItemSchema(range, view, layout);

    view.reset(new ViewText());
    layout.reset(new LayoutAll());
    range.reset(new RangeAll());
    ui->listWidget->grid().addItemSchema(range, view, layout);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
