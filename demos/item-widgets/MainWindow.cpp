#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "core/Layout.h"
#include "core/ext/Ranges.h"
#include "core/ext/ModelStore.h"
#include "items/checkbox/Check.h"
#include "items/radiobutton/Radio.h"
#include "items/text/Text.h"
#include "items/rating/Rating.h"

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_rating(3)
{
    ui->setupUi(this);

    // setup checkboxes
    ui->checkBox1->spaceItem().setItem(ItemID(0, 0));
    ui->checkBox2->spaceItem().setItem(ItemID(1, 0));
    ui->checkBox3->spaceItem().setItem(ItemID(2, 0));

    auto modelChecks = QSharedPointer<ModelStorageVector<Qt::CheckState>>::create();
    modelChecks->setValueAll(Qt::Checked, 3);
    ItemSchema schema(makeRangeAll(), makeLayoutLeft(), QSharedPointer<ViewCheck>::create(modelChecks));
    auto controller = schema.view->controller();

    ui->checkBox1->spaceItem().addSchema(schema);
    ui->checkBox2->spaceItem().addSchema(schema);
    ui->checkBox3->spaceItem().addSchema(schema);

    auto modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item) -> QString {
        return QString("Checkbox #%1").arg(item.row);
    };

    schema.layout = makeLayoutClient();
    schema.view = QSharedPointer<ViewText>::create(modelText, ViewDefaultControllerNone, Qt::AlignLeft, Qt::ElideRight);
    schema.view->setController(controller);
    schema.view->tooltipTextCallback = [](const ItemID& /*item*/, QString& text)->bool {
        text = "Click me";
        return true;
    };

    ui->checkBox1->spaceItem().addSchema(schema);
    ui->checkBox2->spaceItem().addSchema(schema);
    ui->checkBox3->spaceItem().addSchema(schema);

    // setup radio buttons
    ui->radioButton1->spaceItem().setItem(ItemID(0, 0));
    ui->radioButton2->spaceItem().setItem(ItemID(1, 0));
    ui->radioButton3->spaceItem().setItem(ItemID(2, 0));

    schema.layout = makeLayoutLeft();
    auto modelRadio = QSharedPointer<ModelRadioStorage>::create();
    schema.view = QSharedPointer<ViewRadio>::create(modelRadio);

    ui->radioButton1->spaceItem().addSchema(schema);
    ui->radioButton2->spaceItem().addSchema(schema);
    ui->radioButton3->spaceItem().addSchema(schema);

    schema.layout = makeLayoutClient();
    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](const ItemID& item) -> QString {
        return QString("RadioButton #%1").arg(item.row);
    };
    schema.view = QSharedPointer<ViewText>::create(modelText, ViewDefaultControllerNone, Qt::AlignLeft, Qt::ElideMiddle);

    ui->radioButton1->spaceItem().addSchema(schema);
    ui->radioButton2->spaceItem().addSchema(schema);
    ui->radioButton3->spaceItem().addSchema(schema);

    schema.layout = makeLayoutClient();
    auto modelRating = QSharedPointer<ModelRatingCallback>::create();
    modelRating->getValueFunction = [this](const ItemID&) { return m_rating; };
    modelRating->setValueFunction = [this](const ItemID&, int rating) {
        m_rating = rating;
        return true;
    };
    schema.view = QSharedPointer<ViewRating>::create(modelRating, QPixmap(":/img/img/likeOn.png"), QPixmap(":/img/img/likeOff.png"));

    ui->rating->spaceItem().addSchema(schema);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
