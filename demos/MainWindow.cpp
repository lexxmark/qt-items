#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "utils/ViewBasic.h"
#include "utils/LayoutBasic.h"

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    QSharedPointer<View> view = QSharedPointer<ViewCheck>::create();
    QSharedPointer<Layout> layout = QSharedPointer<LayoutLeft>::create();
    ui->itemWidget->addView(view, layout);
    
    view = QSharedPointer<ViewText>::create();
    layout = QSharedPointer<LayoutAll>::create();
    ui->itemWidget->addView(view, layout);
   
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
