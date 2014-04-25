#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "utils/ViewBasic.h"
#include "utils/LayoutBasic.h"
#include "utils/RangesBasic.h"

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    ui->listWidget->grid().rows().setCount(100);
    ui->listWidget->grid().columns().setCount(10);
    ui->listWidget->grid().columns().setAllLinesSize(100);

    ui->listWidget->grid().addCellsSchema(new RangeColumn(1), new LayoutLeft(), new ViewCheck());
    ui->listWidget->grid().addCellsSchema(new RangeAll(), new LayoutAll(), new ViewText());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
