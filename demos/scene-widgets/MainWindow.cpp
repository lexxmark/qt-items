#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "core/Layout.h"
#include "core/ext/Ranges.h"
#include "core/ext/ModelStore.h"
#include "items/misc/ViewItemBorder.h"
#include "items/misc/ViewAlternateBackground.h"
#include "items/misc/ControllerMouseLinesResizer.h"
#include "items/visible/Visible.h"
#include "items/sorting/Sorting.h"
#include "items/filter/FilterText.h"
#include "items/checkbox/Check.h"
#include "items/radiobutton/Radio.h"
#include "items/button/Button.h"
#include "items/text/Text.h"
#include "items/numeric/Numeric.h"
#include "items/selection/Selection.h"
#include "items/image/StyleStandardPixmap.h"
#include "items/image/Image.h"
#include "items/image/Pixmap.h"
#include "items/link/Link.h"
#include "items/progressbar/Progress.h"
#include "items/enum/Enum.h"

#include "misc/GridColumnsResizer.h"
#include "cache/space/CacheSpaceGrid.h"

#include <QMessageBox>

#include <functional>
#include <cstdlib>
#include <time.h>

using namespace Qi;

class ViewLine: public Qi::View
{
public:
    ViewLine()
    {}

protected:
    void drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const override
    {
        painter->drawLine(cache.cacheView.rect().topLeft(), cache.cacheView.rect().bottomRight());
    }
};

class ViewRect: public Qi::View
{
public:
    ViewRect()
    {}

protected:
    void drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const override
    {
        painter->drawRect(cache.cacheView.rect());
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = QSharedPointer<Qi::SpaceSceneElements>::create();
    ui->sceneWidget->initScene(m_scene);

    auto root = QSharedPointer<SceneElementNode>::create(QRect(200, 10, 100, 100));
    auto left = QSharedPointer<SceneElementNode>::create(QRect(100, 150, 100, 100));
    auto right= QSharedPointer<SceneElementNode>::create(QRect(300, 150, 100, 100));

    auto anchor1 = QSharedPointer<SceneElementAnchor>::create(root, HCenter|Bottom);

    auto anchor2 = QSharedPointer<SceneElementAnchor>::create(left, HCenter|Top);
    m_scene->addElement(QSharedPointer<SceneElementConnection>::create(anchor1, anchor2));

    anchor2 = QSharedPointer<SceneElementAnchor>::create(right, HCenter|Top);
    m_scene->addElement(QSharedPointer<SceneElementConnection>::create(anchor1, anchor2));

    m_scene->addElement(right);
    m_scene->addElement(left);
    m_scene->addElement(root);

    m_scene->addSchema(makeRangeByType(m_scene.data(), SceneElementTypeNode), QSharedPointer<ViewRect>::create(), makeLayoutBackground());
    m_scene->addSchema(makeRangeByType(m_scene.data(), SceneElementTypeConnection), QSharedPointer<ViewLine>::create(), makeLayoutBackground());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
