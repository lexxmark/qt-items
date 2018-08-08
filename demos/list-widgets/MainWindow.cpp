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
#include "items/numeric/Numeric.h"
#include "items/selection/Selection.h"
#include "items/image/StyleStandardPixmap.h"
#include "items/image/Image.h"
#include "items/link/Link.h"
#include "items/progressbar/Progress.h"
#include "items/enum/Enum.h"
#include "items/rating/Rating.h"

#include "misc/GridColumnsResizer.h"
#include "misc/CacheSpaceAnimation.h"
#include "space/grid/CacheSpaceGrid.h"
#include "space/grid/RangeGrid.h"
#include "cache/CacheItem.h"
#include "cache/CacheItemFactory.h"
#include "utils/MemFunction.h"
#include "utils/PainterState.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QVariantAnimation>
#include <QPainterPath>
#include <QClipboard>

#include <cmath>

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_resizer = makeShared<Qi::ListColumnsResizer>(ui->listWidget);

    auto& grid = *ui->listWidget->grid();
    grid.columns()->setCount(1);

    m_images = makeShared<ModelStorageColumn<QPixmap>>(grid.rows());
    m_names = makeShared<ModelStorageColumn<QString>>(grid.rows());
    m_descriptions = makeShared<ModelStorageColumn<QString>>(grid.rows());
    m_rates = makeShared<ModelStorageColumn<int>>(grid.rows());

    {
        auto textView = makeShared<ViewText>(makeShared<ModelTextValue>("Load"));
        auto imageView = makeShared<ViewPixmap>(makeShared<ModelPixmapValue>(QPixmap("://img/Download.png")));

        QVector<ViewSchema> subViews;
        subViews.append(ViewSchema(makeLayoutLeft(), imageView));
        subViews.append(ViewSchema(makeLayoutClient(), textView));

        auto bttnView = makeShared<ViewButton>(makeShared<ViewComposite>(subViews));
        bttnView->action = memFunction(this, &MainWindow::onLoadBttnPressed);
        bttnView->setMargins(QMargins(10, 10, 10, 10));
        ui->listWidget->installEmptyView(bttnView, makeLayoutCenter());
    }

    QFont boldFont = ui->listWidget->font();
    boldFont.setBold(true);

    auto viewImages = makeShared<ViewPixmap>(m_images);

    grid.addSchema(makeRangeGridColumn(0), makeShared<ViewRowBorder>(), makeLayoutBottom());
    grid.addSchema(makeRangeGridColumn(0), viewImages, makeLayoutLeft());

    {
        QVector<ViewSchema> subViews;
        subViews.append(ViewSchema(makeLayoutBackground(), makeShared<ViewTextFont>(boldFont)));
        auto ratingView = makeShared<ViewRating>(m_rates, QPixmap("://img/StarOn.png"), QPixmap("://img/StarOff.png"));
        subViews.append(ViewSchema(makeLayoutRight(), ratingView));
        subViews.append(ViewSchema(makeLayoutClient(), makeShared<ViewText>(m_names, ViewDefaultControllerNone, Qt::AlignHCenter|Qt::AlignTop, Qt::ElideRight)));
        QMargins margins(0, 2, 0, 2);
        grid.addSchema(makeRangeGridColumn(0), makeShared<ViewComposite>(subViews, margins), makeLayoutTop());
    }

    grid.addSchema(makeRangeGridColumn(0), makeShared<ViewText>(m_descriptions, ViewDefaultControllerNone, Qt::Alignment(Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap)), makeLayoutClient());

    {
        auto wikiModel = makeShared<ModelCallback<QUrl>>();
        wikiModel->getValueFunction = [this](ID id)->QUrl {
            return QUrl(QString("http://en.wikipedia.org/wiki/%1").arg(m_names->value(id)));
        };
        m_wikiView = makeShared<ViewLink>(makeShared<ModelStorageValue<QString>>("more"), wikiModel);
        QVector<ViewSchema> subViews;
        subViews.append(ViewSchema(makeLayoutRight(), m_wikiView));
        grid.addSchema(makeRangeGridColumn(0), makeShared<ViewComposite>(subViews), makeLayoutBottom());
    }

    // show load button animation
    m_animation = new CacheSpaceAnimationShiftViews(ui->listWidget->viewport(), &ui->listWidget->rMainCacheSpace(), CacheSpaceAnimationShiftTop);
    m_animation->setEasingCurve(QEasingCurve::OutBack);
    m_animation->start();
}

MainWindow::~MainWindow()
{
    if (!m_animation.isNull())
        delete m_animation.data();

    delete ui;
}

void MainWindow::loadData()
{
    auto& grid = *ui->listWidget->grid();

    grid.rows()->setCount(20);

    ID id = makeID<GridID>(0, 0);
    m_images->setValue(id, QPixmap("://img/afghan-hound_01_sm.jpg"));
    m_names->setValue(id, "Afghan Hound");
    m_descriptions->setValue(id, "The Afghan Hound is a hound that is one of the oldest dog breeds in existence. Distinguished by its thick, fine, silky coat and its tail with a ring curl at the end, the breed acquired its unique features in the cold mountains of Afghanistan.");
    m_rates->setValue(id, 2);

    id = makeID<GridID>(1, 0);
    m_images->setValue(id, QPixmap("://img/alaskan-malamute_01_sm.jpg"));
    m_names->setValue(id, "Alaskan Malamute");
    m_descriptions->setValue(id, "The Alaskan Malamute is a large breed of domestic dog (Canis lupus familiaris) originally bred for hauling heavy freight because of their strength and endurance, and later an Alaskan sled dog.");
    m_rates->setValue(id, 3);

    id = makeID<GridID>(2, 0);
    m_images->setValue(id, QPixmap("://img/american-foxhound_01_sm.jpg"));
    m_names->setValue(id, "American Foxhound");
    m_descriptions->setValue(id, "The American Foxhound is a breed of dog that is a cousin of the English Foxhound. They are scent hounds, bred to hunt foxes by scent.");
    m_rates->setValue(id, 5);

    id = makeID<GridID>(3, 0);
    m_images->setValue(id, QPixmap("://img/anatolian-shepherd_01_sm.jpg"));
    m_names->setValue(id, "Anatolian Shepherd");
    m_descriptions->setValue(id, "The Anatolian Shepherd Dog (Turkish: Anadolu çoban köpeği) is a breed of dog which originated in Anatolia (central Turkey) and was further developed as a breed in America. It is rugged, large and very strong; with superior sight and hearing allowing it to protect livestock.");
    m_rates->setValue(id, 2);

    id = makeID<GridID>(4, 0);
    m_images->setValue(id, QPixmap("://img/australian-shepherd_01_sm.jpg"));
    m_names->setValue(id, "Australian Shepherd");
    m_descriptions->setValue(id, "The Australian Shepherd, commonly known as the Aussie, is a medium size breed of dog that was developed on ranches in the western United States. Despite its name, the breed was not developed in Australia, but rather in the United States where they were seen in the West as early as the 1800s.");
    m_rates->setValue(id, 0);

    id = makeID<GridID>(5, 0);
    m_images->setValue(id, QPixmap("://img/basenji_01_sm.jpg"));
    m_names->setValue(id, "Basenji");
    m_descriptions->setValue(id, "The Basenji is a breed of hunting dog that was bred from stock originating in central Africa. Most of the major kennel clubs in the English-speaking world place the breed in the Hound Group; more specifically, it may be classified as belonging to the sighthound type.");
    m_rates->setValue(id, 1);

    id = makeID<GridID>(6, 0);
    m_images->setValue(id, QPixmap("://img/basset-hound_01_sm.jpg"));
    m_names->setValue(id, "Basset Hound");
    m_descriptions->setValue(id, "The Basset Hound is a short-legged breed of dog of the hound family, as well as one of six recognized Basset breeds in France; furthermore, Bassets are scent hounds that were originally bred for the purpose of hunting rabbits and hare.");
    m_rates->setValue(id, 4);

    id = makeID<GridID>(7, 0);
    m_images->setValue(id, QPixmap("://img/beagle_01_sm.jpg"));
    m_names->setValue(id, "Beagle");
    m_descriptions->setValue(id, "The Beagle is a breed of small to medium-sized dog. A member of the hound group, it is similar in appearance to the foxhound, but smaller with shorter legs and longer, softer ears. Beagles are scent hounds, developed primarily for tracking hare, rabbit, deer, and other small game.");
    m_rates->setValue(id, 5);

    id = makeID<GridID>(8, 0);
    m_images->setValue(id, QPixmap("://img/belgian-malinois_01_sm.jpg"));
    m_names->setValue(id, "Malinois");
    m_descriptions->setValue(id, "The Malinois or Belgian Shepherd Dog is a medium breed of dog, sometimes classified as a variety of the Belgian Shepherd Dog classification, rather than as a separate breed. The Malinois is recognized in the United States under the name Belgian Malinois.");
    m_rates->setValue(id, 0);

    id = makeID<GridID>(9, 0);
    m_images->setValue(id, QPixmap("://img/borzoi_01_sm.jpg"));
    m_names->setValue(id, "Borzoi");
    m_descriptions->setValue(id, "The Borzoi, also called the Russian wolfhound, is a breed of domestic dog (Canis lupus familiaris). Descended from dogs brought to Russia from central Asian countries, it is similar in shape to a greyhound, and is also a member of the sighthound family.");
    m_rates->setValue(id, 5);

    id = makeID<GridID>(10, 0);
    m_images->setValue(id, QPixmap("://img/bulldog_01_sm.jpg"));
    m_names->setValue(id, "Bulldog");
    m_descriptions->setValue(id, "The Bulldog is a medium-sized breed of dog commonly referred to as the English Bulldog or British Bulldog. The Bulldog is a muscular, heavy dog with a wrinkled face and a distinctive pushed-in nose.");
    m_rates->setValue(id, 1);

    id = makeID<GridID>(11, 0);
    m_images->setValue(id, QPixmap("://img/vizsla_01_sm.jpg"));
    m_names->setValue(id, "Vizsla");
    m_descriptions->setValue(id, "The Vizsla is a dog breed originating in Hungary, which belongs under the FCI group 7 (Pointer group). The Hungarian or Magyar Vizsla are sporting dogs and loyal companions, in addition to being the smallest of the all-round pointer-retriever breeds.");
    m_rates->setValue(id, 2);

    id = makeID<GridID>(12, 0);
    m_images->setValue(id, QPixmap("://img/dalmation_01_sm.jpg"));
    m_names->setValue(id, "Dalmatian");
    m_descriptions->setValue(id, "The Dalmatian is a large breed of dog noted for its unique black or liver spotted coat and was mainly used as a carriage dog in its early days. Its roots trace back to Croatia and its historical region of Dalmatia. Today, this dog remains a well-loved family pet, and many dog enthusiasts enter their pets into kennel club competitions.");
    m_rates->setValue(id, 1);

    id = makeID<GridID>(13, 0);
    m_images->setValue(id, QPixmap("://img/english-setter_01_sm.jpg"));
    m_names->setValue(id, "English Setter");
    m_descriptions->setValue(id, "The English Setter is a medium size breed of dog. It is part of the Setter family, which includes the red Irish Setters, Irish Red and White Setters, and black-and-tan Gordon Setters. The mainly white body coat is of medium length with long silky fringes on the back of the legs, under the belly and on the tail.");
    m_rates->setValue(id, 4);

    id = makeID<GridID>(14, 0);
    m_images->setValue(id, QPixmap("://img/greater-swiss-mountain-dog_01_sm.jpg"));
    m_names->setValue(id, "Greater Swiss Mountain Dog");
    m_descriptions->setValue(id, "The Greater Swiss Mountain Dog (German: Grosser Schweizer Sennenhund or French: Grand Bouvier Suisse) is a dog breed which was developed in the Swiss Alps. The name Sennenhund refers to people called Senn or Senner, dairymen and herders in the Swiss Alps.");
    m_rates->setValue(id, 3);

    id = makeID<GridID>(15, 0);
    m_images->setValue(id, QPixmap("://img/irish-water-spaniel_01_sm.jpg"));
    m_names->setValue(id, "Irish Water Spaniel");
    m_descriptions->setValue(id, "The Irish Water Spaniel (Irish: An Spáinnéar Uisce) is a breed of dog that is the largest and one of the oldest of spaniels. The Irish Water Spaniel is one of the rarer breeds with the AKC in terms of registrations.");
    m_rates->setValue(id, 5);

    id = makeID<GridID>(16, 0);
    m_images->setValue(id, QPixmap("://img/neopolitan-mastiff_01_sm.jpg"));
    m_names->setValue(id, "Neapolitan Mastiff");
    m_descriptions->setValue(id, "The Neapolitan Mastiff or Italian Mastiff, (Italian: Mastino Napoletano) is a large, ancient dog breed. This massive breed is often used as a guard and defender of family and property due to their protective instincts and their fearsome appearance.");
    m_rates->setValue(id, 5);

    id = makeID<GridID>(17, 0);
    m_images->setValue(id, QPixmap("://img/poodle-standard_01_sm.jpg"));
    m_names->setValue(id, "Poodle");
    m_descriptions->setValue(id, "The poodle is a group of formal dog breeds, the Standard Poodle, Miniature Poodle and Toy Poodle (one registry organisation also recognizes a Medium Poodle variety, between Standard and Miniature), with many coat colors. Originally bred in Germany as a type of water dog, the breed was standardized in France.");
    m_rates->setValue(id, 2);

    id = makeID<GridID>(18, 0);
    m_images->setValue(id, QPixmap("://img/shiba-inu_01_sm.jpg"));
    m_names->setValue(id, "Shiba Inu");
    m_descriptions->setValue(id, "The Shiba Inu is the smallest of the six original and distinct spitz breeds of dog from Japan. A small, agile dog that copes very well with mountainous terrain, the Shiba Inu was originally bred for hunting.");
    m_rates->setValue(id, 3);

    id = makeID<GridID>(19, 0);
    m_images->setValue(id, QPixmap("://img/siberian-husky_01_sm.jpg"));
    m_names->setValue(id, "Siberian Husky");
    m_descriptions->setValue(id, "The Siberian Husky is a medium size, dense-coat working dog breed that originated in north-eastern Siberia. The breed belongs to the Spitz genetic family. It is recognizable by its thickly furred double coat, erect triangular ears, and distinctive markings.");
    m_rates->setValue(id, 2);

    // set row heights as 1st item height
    auto cacheItem = makeShared<CacheItem>(ui->listWidget->cacheGrid()->cacheItemFactory().create(makeID<GridID>(0, 0)));
    Q_ASSERT(cacheItem);
    cacheItem->validateCacheView(ui->listWidget->guiContext());
    QSize itemSize = cacheItem->calculateItemSize(ui->listWidget->guiContext());
    grid.rows()->setLineSizeAll(itemSize.height());

    // show items animation
    m_animation = new CacheSpaceAnimationShiftViews(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data(), CacheSpaceAnimationShiftRight);
    m_animation->setEasingCurve(QEasingCurve::OutCirc);
    m_animation->start();

    // enable buttons
    ui->shiftRightBttn->setEnabled(true);
    ui->shiftLeftBttn->setEnabled(true);
    ui->shiftRandomBttn->setEnabled(true);
    ui->circleImageBttn->setEnabled(true);
    ui->shiftRightItemBttn->setEnabled(true);
    ui->shiftDownItemBttn->setEnabled(true);
    ui->shiftBottomRightBttn->setEnabled(true);
    ui->fadeBttn->setEnabled(true);
}

void MainWindow::onLoadBttnPressed(const Qi::ID&, const Qi::ControllerContext&, const Qi::ViewButton*)
{
    // hide load button animation
    m_animation = new CacheSpaceAnimationShiftViews(ui->listWidget->viewport(), &ui->listWidget->rMainCacheSpace(), CacheSpaceAnimationShiftBottom);
    m_animation->setEasingCurve(QEasingCurve::OutBack);

    connect(m_animation.data(), &CacheSpaceAnimationAbstract::stopped, [this](){
        loadData();
    });

    m_animation->start(QAbstractAnimation::Backward);
}

void MainWindow::shuffleRows()
{
    QVector<int> permutation = ui->listWidget->grid()->rows()->permutation();
    std::random_shuffle(permutation.begin(), permutation.end());
    ui->listWidget->grid()->rows()->setPermutation(permutation);
}

void MainWindow::playAnimation(Qi::CacheSpaceAnimationAbstract* animation, const QEasingCurve & easing)
{
    Q_ASSERT(animation);

    // connect to the end of old animation
    connect(m_animation.data(), &CacheSpaceAnimationAbstract::stopped, [this, animation, easing]() {

        shuffleRows();

        // don't delete while old m_animation is emitting stopped signal
        m_animation->deleteLater();

        // change to new animation
        m_animation = animation;
        m_animation->setEasingCurve(easing);
        // run new animation
        m_animation->start();
    });

    // run old animation back
    m_animation->start(QAbstractAnimation::Backward);
}


void MainWindow::on_pushButton_clicked()
{
    close();
}

void MainWindow::on_shiftRightBttn_clicked()
{
    playAnimation(new CacheSpaceAnimationShiftViews(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data(), CacheSpaceAnimationShiftRight));
}

void MainWindow::on_shiftLeftBttn_clicked()
{
    playAnimation(new CacheSpaceAnimationShiftViews(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data(), CacheSpaceAnimationShiftLeft), QEasingCurve::InOutQuart);
}

void MainWindow::on_shiftRandomBttn_clicked()
{
    playAnimation(new CacheSpaceAnimationShiftViewsRandom(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data()));
}

QAbstractAnimation* MainWindow::createCircleViewAnimation(const Qi::CacheSpaceAnimationAbstract* /*mainAnimation*/, CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx) const
{
    auto animation = new QParallelAnimationGroup();

    cacheSpace->validate(ctx);
    cacheSpace->forEachCacheView([this, animation](const CacheSpace::IterateInfo& info)->bool {

        auto v = (View2*)info.cacheView->view();

        // animate images
        if (v->model() == m_images.data())
        {
            auto subAnimation = new QVariantAnimation(animation);
            subAnimation->setDuration(1000);
            int radius = (int)hypot(qreal(info.cacheView->rect().width()), qreal(info.cacheView->rect().height()));
            subAnimation->setStartValue(0);
            subAnimation->setEndValue(radius);

            info.cacheView->drawProxy = [subAnimation](const CacheView2* cacheView, QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect* visibleRect) {

                painter->save();

                QPointF c = cacheView->rect().center();
                int r = subAnimation->currentValue().toInt();

                QPainterPath path;
                path.addEllipse(c, (qreal)r, (qreal)r);
                painter->setClipPath(path, Qt::IntersectClip);
                cacheView->drawRaw(painter, ctx, id, itemRect, visibleRect);

                painter->restore();
            };

            animation->addAnimation(subAnimation);
        }
        // animate others
        else if (v->model() == m_descriptions.data() ||
                 v->model() == m_names.data() ||
                 v->model() == m_rates.data() ||
                 v == m_wikiView.data())
        {
            auto subAnimation = new QVariantAnimation(animation);
            subAnimation->setDuration(1000);
            subAnimation->setStartValue(0.f);
            subAnimation->setEndValue(1.f);

            info.cacheView->drawProxy = [subAnimation](const CacheView2* cacheView, QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect* visibleRect) {

                float progress = subAnimation->currentValue().toFloat();

                qreal oldOpacity = painter->opacity();
                painter->setOpacity(progress);

                cacheView->drawRaw(painter, ctx, id, itemRect, visibleRect);

                painter->setOpacity(oldOpacity);
            };

            animation->addAnimation(subAnimation);
        }

        return true;
    });

    return animation;
}

void MainWindow::on_circleImageBttn_clicked()
{
    auto animation = new CacheSpaceAnimationCallback(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data());
    animation->animationFactory = memFunction(this, &MainWindow::createCircleViewAnimation);

    playAnimation(animation);
}

QAbstractAnimation* MainWindow::createShiftRightItemAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const
{
    auto animation = new QParallelAnimationGroup();

    cacheSpace->validate(ctx);
    int itemIndex = 0;
    cacheSpace->forEachCacheItem([mainAnimation, animation, &ctx, cacheSpace, &itemIndex, painter](const SharedPtr<CacheItem>& cacheItem)->bool {

        // create image of the item
        {
            QPixmap itemPixmap(cacheItem->rect.size());
            itemPixmap.fill(Qt::transparent);
            QPainter itemPainter(&itemPixmap);
            copyPainterState(painter, &itemPainter);
            itemPainter.setWindow(cacheItem->rect);

            cacheItem->draw(&itemPainter, ctx, &cacheSpace->window());

            cacheItem->drawProxy = [itemPixmap](CacheItem* cacheItem, QPainter* painter, const GuiContext& /*ctx*/, const QRect* /*visibleRect*/) {
                painter->drawPixmap(cacheItem->rect.topLeft(), itemPixmap);
            };
        }

        auto subAnimation = new QSequentialAnimationGroup(animation);

        auto rectAnimation = new QVariantAnimation(subAnimation);
        rectAnimation->setDuration(1000);
        rectAnimation->setEasingCurve(mainAnimation->easingCurve());
        QRect startRect = cacheItem->rect;
        startRect.moveTo(cacheSpace->window().left() - startRect.width(), startRect.top());
        rectAnimation->setStartValue(startRect);
        rectAnimation->setEndValue(cacheItem->rect);
        QObject::connect(rectAnimation, &QVariantAnimation::valueChanged, [cacheItem](const QVariant &value){
            cacheItem->rect = value.toRect();
        });
        subAnimation->addPause(itemIndex*100);
        subAnimation->addAnimation(rectAnimation);

        animation->addAnimation(subAnimation);

        if (mainAnimation->direction() == QAbstractAnimation::Forward)
        {
            cacheItem->rect = startRect;
        }

        ++itemIndex;

        return true;
    });

    return animation;
}

void MainWindow::on_shiftRightItemBttn_clicked()
{
    auto animation = new CacheSpaceAnimationCallback(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data());
    animation->animationFactory = memFunction(this, &MainWindow::createShiftRightItemAnimation);

    playAnimation(animation, QEasingCurve::OutBounce);
}

QAbstractAnimation* MainWindow::createShiftDownItemAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const
{
    auto animation = new QSequentialAnimationGroup();

    cacheSpace->validate(ctx);
    int itemIndex = 0;
    cacheSpace->forEachCacheItem([mainAnimation, animation, &ctx, cacheSpace, &itemIndex, painter](const SharedPtr<CacheItem>& cacheItem)->bool {

        auto topAnimation = new QVariantAnimation(animation);
        topAnimation->setDuration(200);
        topAnimation->setEasingCurve(QEasingCurve::OutBack);
        topAnimation->setStartValue(cacheItem->rect.top() - cacheItem->rect.height());
        topAnimation->setEndValue(cacheItem->rect.top());

        // create image of the item
        {
            QPixmap itemPixmap(cacheItem->rect.size());
            itemPixmap.fill(Qt::transparent);
            QPainter itemPainter(&itemPixmap);
            copyPainterState(painter, &itemPainter);
            itemPainter.setWindow(cacheItem->rect);

            cacheItem->draw(&itemPainter, ctx, &cacheSpace->window());

            cacheItem->drawProxy = [itemPixmap, topAnimation](CacheItem* cacheItem, QPainter* painter, const GuiContext& /*ctx*/, const QRect* /*visibleRect*/) {
                painter->save();
                painter->setClipRect(cacheItem->rect, Qt::IntersectClip);
                painter->drawPixmap(cacheItem->rect.left(), topAnimation->currentValue().toInt(), itemPixmap);
                painter->restore();
            };
        }

        animation->addAnimation(topAnimation);

        if (mainAnimation->direction() == QAbstractAnimation::Backward)
        {
            topAnimation->setCurrentTime(topAnimation->duration());
        }

        ++itemIndex;

        return true;
    });

    return animation;
}

void MainWindow::on_shiftDownItemBttn_clicked()
{
    auto animation = new CacheSpaceAnimationCallback(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data());
    animation->animationFactory = memFunction(this, &MainWindow::createShiftDownItemAnimation);

    playAnimation(animation);
}

QAbstractAnimation* MainWindow::createShiftBottomRightAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const
{
    QPixmap cachePixmap(cacheSpace->window().size());
    cachePixmap.fill(Qt::transparent);

    {
        QPainter cachePainter(&cachePixmap);
        copyPainterState(painter, &cachePainter);
        cachePainter.setWindow(cacheSpace->window());

        cacheSpace->drawRaw(&cachePainter, ctx);
    }

    auto topLeftAnimation = new QVariantAnimation();
    topLeftAnimation->setDuration(2000);
    topLeftAnimation->setEasingCurve(QEasingCurve::OutBack);
    QPoint startValue = cacheSpace->window().topLeft();
    if (mainAnimation->direction() == QAbstractAnimation::Forward)
        startValue.rx() -= cacheSpace->window().width();
    else
        startValue.rx() += cacheSpace->window().width();
    startValue.ry() -= cacheSpace->window().height();
    topLeftAnimation->setStartValue(startValue);
    topLeftAnimation->setEndValue(cacheSpace->window().topLeft());

    cacheSpace->drawProxy = [cachePixmap, topLeftAnimation](const CacheSpace* cacheSpace, QPainter* painter, const GuiContext& /*ctx*/) {
        painter->save();
        painter->setClipRect(cacheSpace->window(), Qt::IntersectClip);
        painter->drawPixmap(topLeftAnimation->currentValue().toPoint(), cachePixmap);
        painter->restore();
    };

    connect(mainAnimation, &Qi::CacheSpaceAnimationAbstract::stopped, [cacheSpace](){
        cacheSpace->drawProxy = nullptr;
    });

    return topLeftAnimation;
}

void MainWindow::on_shiftBottomRightBttn_clicked()
{
    auto animation = new CacheSpaceAnimationCallback(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data());
    animation->animationFactory = memFunction(this, &MainWindow::createShiftBottomRightAnimation);

    playAnimation(animation);
}

QAbstractAnimation* MainWindow::createFadeAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const
{
    QPixmap cachePixmap(cacheSpace->window().size());
    cachePixmap.fill(Qt::transparent);

    {
        QPainter cachePainter(&cachePixmap);
        copyPainterState(painter, &cachePainter);
        cachePainter.setWindow(cacheSpace->window());

        cacheSpace->drawRaw(&cachePainter, ctx);
    }

    auto fadeAnimation = new QVariantAnimation();
    fadeAnimation->setDuration(2000);
    fadeAnimation->setEasingCurve(QEasingCurve::InCirc);
    fadeAnimation->setStartValue(0.f);
    fadeAnimation->setEndValue(1.f);

    cacheSpace->drawProxy = [cachePixmap, fadeAnimation](const CacheSpace* cacheSpace, QPainter* painter, const GuiContext& /*ctx*/) {
        qreal oldOpacity = painter->opacity();
        painter->setOpacity(fadeAnimation->currentValue().toFloat());
        painter->drawPixmap(cacheSpace->window(), cachePixmap);
        painter->setOpacity(oldOpacity);
    };

    connect(mainAnimation, &Qi::CacheSpaceAnimationAbstract::stopped, [cacheSpace](){
        cacheSpace->drawProxy = nullptr;
    });

    return fadeAnimation;
}

void MainWindow::on_fadeBttn_clicked()
{
    auto animation = new CacheSpaceAnimationCallback(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data());
    animation->animationFactory = memFunction(this, &MainWindow::createFadeAnimation);

    playAnimation(animation);
}

void MainWindow::on_clearData_clicked()
{
    auto animation = new CacheSpaceAnimationShiftViews(ui->listWidget->viewport(), &ui->listWidget->rMainCacheSpace(), CacheSpaceAnimationShiftTop);
    connect(m_animation.data(), &CacheSpaceAnimationAbstract::stopped, [this]() {
        ui->listWidget->grid()->setRowsCount(0);

        // disable buttons
        ui->shiftRightBttn->setDisabled(true);
        ui->shiftLeftBttn->setDisabled(true);
        ui->shiftRandomBttn->setDisabled(true);
        ui->circleImageBttn->setDisabled(true);
        ui->shiftRightItemBttn->setDisabled(true);
        ui->shiftDownItemBttn->setDisabled(true);
        ui->shiftBottomRightBttn->setDisabled(true);
        ui->fadeBttn->setDisabled(true);
    });
    playAnimation(animation, QEasingCurve::OutBack);
}

void MainWindow::on_alphabetOrderBttn_clicked()
{
    ui->listWidget->grid()->sortColumnByModel(0, *m_names, true, false);
}

void MainWindow::on_starsOrderBttn_clicked()
{
    ui->listWidget->grid()->sortColumnByModel(0, *m_rates, false, false);
}

void MainWindow::on_createImageBttn_clicked()
{
    QPixmap pixmap = ui->listWidget->createPixmap();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(pixmap.toImage());
}
