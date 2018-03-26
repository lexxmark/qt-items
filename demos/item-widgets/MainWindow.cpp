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
    ui->checkBox1->spaceItem().setId(ID(0));
    ui->checkBox2->spaceItem().setId(ID(1));
    ui->checkBox3->spaceItem().setId(ID(2));

    auto modelChecks = QSharedPointer<ModelStorageVector<Qt::CheckState>>::create();
    modelChecks->setValueAll(Qt::Checked, 3);
    ItemSchema schema(makeRangeAll(), makeLayoutLeft(), QSharedPointer<ViewCheck>::create(modelChecks));
    auto controller = schema.view->controller();

    ui->checkBox1->spaceItem().addSchema(schema);
    ui->checkBox2->spaceItem().addSchema(schema);
    ui->checkBox3->spaceItem().addSchema(schema);

    auto modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](ID id) -> QString {
        return QString("Checkbox #%1").arg(id.as<int>());
    };

    schema.layout = makeLayoutClient();
    schema.view = QSharedPointer<ViewText>::create(modelText, ViewDefaultControllerNone, Qt::AlignLeft, Qt::ElideRight);
    schema.view->setController(controller);
    schema.view->tooltipTextCallback = [](ID /*id*/, QString& text)->bool {
        text = "Click me";
        return true;
    };

    ui->checkBox1->spaceItem().addSchema(schema);
    ui->checkBox2->spaceItem().addSchema(schema);
    ui->checkBox3->spaceItem().addSchema(schema);

    // setup radio buttons
    ui->radioButton1->spaceItem().setId(ID(0));
    ui->radioButton2->spaceItem().setId(ID(1));
    ui->radioButton3->spaceItem().setId(ID(2));

    schema.layout = makeLayoutLeft();
    auto modelRadio = QSharedPointer<ModelRadioStorage>::create(ID(0));
    schema.view = QSharedPointer<ViewRadio>::create(modelRadio);

    ui->radioButton1->spaceItem().addSchema(schema);
    ui->radioButton2->spaceItem().addSchema(schema);
    ui->radioButton3->spaceItem().addSchema(schema);

    schema.layout = makeLayoutClient();
    modelText = QSharedPointer<ModelTextCallback>::create();
    modelText->getValueFunction = [](ID id) -> QString {
        return QString("RadioButton #%1").arg(id.as<int>());
    };
    schema.view = QSharedPointer<ViewText>::create(modelText, ViewDefaultControllerNone, Qt::AlignLeft, Qt::ElideMiddle);

    ui->radioButton1->spaceItem().addSchema(schema);
    ui->radioButton2->spaceItem().addSchema(schema);
    ui->radioButton3->spaceItem().addSchema(schema);

    schema.layout = makeLayoutClient();
    auto modelRating = QSharedPointer<ModelRatingCallback>::create();
    modelRating->getValueFunction = [this](ID) { return m_rating; };
    modelRating->setValueFunction = [this](ID, int rating) {
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
