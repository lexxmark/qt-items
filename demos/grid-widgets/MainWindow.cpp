#include "MainWindow.h"
#include "ui_MainWindow.h"
//#include "core/Views.h"
//#include "core/Layouts.h"
//#include "core/Ranges.h"

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
