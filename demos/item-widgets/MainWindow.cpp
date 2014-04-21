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

    QSharedPointer<View> view;
    QSharedPointer<Layout> layout;
    QSharedPointer<Range> range;

    view.reset(new ViewCheck());
    layout.reset(new LayoutLeft());
    ui->checkBox1->addViewSchema(view, layout);
    ui->checkBox2->addViewSchema(view, layout);
    ui->checkBox3->addViewSchema(view, layout);

    view.reset(new ViewText());
    layout.reset(new LayoutAll());
    ui->checkBox1->addViewSchema(view, layout);
    ui->checkBox2->addViewSchema(view, layout);
    ui->checkBox3->addViewSchema(view, layout);

    ui->radioButton1->setCell(CellID(0, 0));
    ui->radioButton2->setCell(CellID(1, 0));
    ui->radioButton3->setCell(CellID(2, 0));

    view.reset(new ViewRadio());
    layout.reset(new LayoutLeft());
    ui->radioButton1->addViewSchema(view, layout);
    ui->radioButton2->addViewSchema(view, layout);
    ui->radioButton3->addViewSchema(view, layout);

    view.reset(new ViewText());
    layout.reset(new LayoutAll());
    ui->radioButton1->addViewSchema(view, layout);
    ui->radioButton2->addViewSchema(view, layout);
    ui->radioButton3->addViewSchema(view, layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
