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

    ui->checkBox1->setCell(CellID(0, 0));
    ui->checkBox2->setCell(CellID(1, 0));
    ui->checkBox3->setCell(CellID(2, 0));

    Layout* layout = new LayoutLeft();
    View* view = new ViewCheck();
    ui->checkBox1->addViewSchema(layout, view);
    ui->checkBox2->addViewSchema(layout, view);
    ui->checkBox3->addViewSchema(layout, view);

    view = new ViewText();
    layout = new LayoutAll();
    ui->checkBox1->addViewSchema(layout, view);
    ui->checkBox2->addViewSchema(layout, view);
    ui->checkBox3->addViewSchema(layout, view);

    ui->radioButton1->setCell(CellID(0, 0));
    ui->radioButton2->setCell(CellID(1, 0));
    ui->radioButton3->setCell(CellID(2, 0));

    view = new ViewRadio();
    layout = new LayoutLeft();
    ui->radioButton1->addViewSchema(layout, view);
    ui->radioButton2->addViewSchema(layout, view);
    ui->radioButton3->addViewSchema(layout, view);

    view = new ViewText();
    layout = new LayoutAll();
    ui->radioButton1->addViewSchema(layout, view);
    ui->radioButton2->addViewSchema(layout, view);
    ui->radioButton3->addViewSchema(layout, view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
