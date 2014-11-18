#include "MainWindow.h"
#include "ui_MainWindow.h"
//#include "core/Views.h"
//#include "core/Layouts.h"
//#include "core/Ranges.h"

//using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    ui->listWidget->grid().rows().setCount(100);
    ui->listWidget->grid().columns().setCount(10);
    ui->listWidget->grid().columns().setAllLinesSize(100);

    ui->listWidget->grid().addItemSchema(new RangeColumn(1), new LayoutLeft(), new ViewCheck());
    ui->listWidget->grid().addItemSchema(new RangeAll(), new LayoutAll(), new ViewText());
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
