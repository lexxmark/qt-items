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

    // setup checkboxes
    ui->checkBox1->space().setItem(ItemID(0, 0));
    ui->checkBox2->space().setItem(ItemID(1, 0));
    ui->checkBox3->space().setItem(ItemID(2, 0));

    auto modelChecks = QSharedPointer<ModelStorageVector<Qt::CheckState>>::create();
    modelChecks->setValueAll(Qt::Checked, 3);
    ItemSchema schema(makeRangeAll(), makeLayoutLeft(), QSharedPointer<ViewCheck>::create(modelChecks));
    auto controller = schema.view->controller();

    ui->checkBox1->space().addSchema(schema);
    ui->checkBox2->space().addSchema(schema);
    ui->checkBox3->space().addSchema(schema);

    auto modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item) -> QString {
        return QString("Checkbox #%1").arg(item.row);
    };

    schema.layout = makeLayoutClient();
    schema.view = QSharedPointer<ViewText>::create(modelText, false, Qt::AlignLeft, Qt::ElideRight);
    schema.view->setController(controller);
    schema.view->tooltipTextCallback = [](const ItemID& /*item*/, QString& text)->bool {
        text = "Click me";
        return true;
    };

    ui->checkBox1->space().addSchema(schema);
    ui->checkBox2->space().addSchema(schema);
    ui->checkBox3->space().addSchema(schema);

    // setup radio buttons
    ui->radioButton1->space().setItem(ItemID(0, 0));
    ui->radioButton2->space().setItem(ItemID(1, 0));
    ui->radioButton3->space().setItem(ItemID(2, 0));

    schema.layout = makeLayoutLeft();
    auto modelRadio = QSharedPointer<ModelRadioStorage>::create();
    schema.view = QSharedPointer<ViewRadio>::create(modelRadio);

    ui->radioButton1->space().addSchema(schema);
    ui->radioButton2->space().addSchema(schema);
    ui->radioButton3->space().addSchema(schema);

    schema.layout = makeLayoutClient();
    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item) -> QString {
        return QString("RadioButton #%1").arg(item.row);
    };
    schema.view = QSharedPointer<ViewText>::create(modelText, false, Qt::AlignLeft, Qt::ElideMiddle);

    ui->radioButton1->space().addSchema(schema);
    ui->radioButton2->space().addSchema(schema);
    ui->radioButton3->space().addSchema(schema);

    /*
    view = new ViewText();
    layout = new LayoutAll();
    ui->checkBox1->addViewSchema(layout, view);
    ui->checkBox2->addViewSchema(layout, view);
    ui->checkBox3->addViewSchema(layout, view);

    ui->radioButton1->setItemID(ItemID(0, 0));
    ui->radioButton2->setItemID(ItemID(1, 0));
    ui->radioButton3->setItemID(ItemID(2, 0));

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
