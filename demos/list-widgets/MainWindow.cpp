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
#include "cache/space/CacheSpaceGrid.h"
#include "cache/CacheItem.h"
#include "utils/MemFunction.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QVariantAnimation>
#include <QPainterPath>

#include <cmath>

using namespace Qi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_resizer = QSharedPointer<Qi::ListColumnsResizer>::create(ui->listWidget);

    auto& grid = *ui->listWidget->grid();

    grid.rows()->setCount(20);
    grid.rows()->setLineSizeAll(150);
    grid.columns()->setCount(1);
    //grid.columns()->setLineSizeAll(500);

    auto modelRating = QSharedPointer<ModelStorageColumn<int>>::create(grid.rows());
    {
        m_images = QSharedPointer<ModelStorageColumn<QPixmap>>::create(grid.rows());
        m_names = QSharedPointer<ModelStorageColumn<QString>>::create(grid.rows());
        m_descriptions = QSharedPointer<ModelStorageColumn<QString>>::create(grid.rows());

        m_images->setValue(0, 0, QPixmap("://img/afghan-hound_01_sm.jpg"));
        m_names->setValue(0, 0, "Afghan Hound");
        m_descriptions->setValue(0, 0, "The Afghan Hound is a hound that is one of the oldest dog breeds in existence. Distinguished by its thick, fine, silky coat and its tail with a ring curl at the end, the breed acquired its unique features in the cold mountains of Afghanistan.");
        modelRating->setValue(0, 0, 2);

        m_images->setValue(1, 0, QPixmap("://img/alaskan-malamute_01_sm.jpg"));
        m_names->setValue(1, 0, "Alaskan Malamute");
        m_descriptions->setValue(1, 0, "The Alaskan Malamute is a large breed of domestic dog (Canis lupus familiaris) originally bred for hauling heavy freight because of their strength and endurance, and later an Alaskan sled dog.");
        modelRating->setValue(1, 0, 3);

        m_images->setValue(2, 0, QPixmap("://img/american-foxhound_01_sm.jpg"));
        m_names->setValue(2, 0, "American Foxhound");
        m_descriptions->setValue(2, 0, "The American Foxhound is a breed of dog that is a cousin of the English Foxhound. They are scent hounds, bred to hunt foxes by scent.");
        modelRating->setValue(2, 0, 5);

        m_images->setValue(3, 0, QPixmap("://img/anatolian-shepherd_01_sm.jpg"));
        m_names->setValue(3, 0, "Anatolian Shepherd");
        m_descriptions->setValue(3, 0, "The Anatolian Shepherd Dog (Turkish: Anadolu çoban köpeği) is a breed of dog which originated in Anatolia (central Turkey) and was further developed as a breed in America. It is rugged, large and very strong; with superior sight and hearing allowing it to protect livestock.");
        modelRating->setValue(3, 0, 2);

        m_images->setValue(4, 0, QPixmap("://img/australian-shepherd_01_sm.jpg"));
        m_names->setValue(4, 0, "Australian Shepherd");
        m_descriptions->setValue(4, 0, "The Australian Shepherd, commonly known as the Aussie, is a medium size breed of dog that was developed on ranches in the western United States. Despite its name, the breed was not developed in Australia, but rather in the United States where they were seen in the West as early as the 1800s.");
        modelRating->setValue(4, 0, 0);

        m_images->setValue(5, 0, QPixmap("://img/basenji_01_sm.jpg"));
        m_names->setValue(5, 0, "Basenji");
        m_descriptions->setValue(5, 0, "The Basenji is a breed of hunting dog that was bred from stock originating in central Africa. Most of the major kennel clubs in the English-speaking world place the breed in the Hound Group; more specifically, it may be classified as belonging to the sighthound type.");
        modelRating->setValue(5, 0, 1);

        m_images->setValue(6, 0, QPixmap("://img/basset-hound_01_sm.jpg"));
        m_names->setValue(6, 0, "Basset Hound");
        m_descriptions->setValue(6, 0, "The Basset Hound is a short-legged breed of dog of the hound family, as well as one of six recognized Basset breeds in France; furthermore, Bassets are scent hounds that were originally bred for the purpose of hunting rabbits and hare.");
        modelRating->setValue(6, 0, 4);

        m_images->setValue(7, 0, QPixmap("://img/beagle_01_sm.jpg"));
        m_names->setValue(7, 0, "Beagle");
        m_descriptions->setValue(7, 0, "The Beagle is a breed of small to medium-sized dog. A member of the hound group, it is similar in appearance to the foxhound, but smaller with shorter legs and longer, softer ears. Beagles are scent hounds, developed primarily for tracking hare, rabbit, deer, and other small game.");
        modelRating->setValue(7, 0, 5);

        m_images->setValue(8, 0, QPixmap("://img/belgian-malinois_01_sm.jpg"));
        m_names->setValue(8, 0, "Malinois");
        m_descriptions->setValue(8, 0, "The Malinois or Belgian Shepherd Dog is a medium breed of dog, sometimes classified as a variety of the Belgian Shepherd Dog classification, rather than as a separate breed. The Malinois is recognized in the United States under the name Belgian Malinois.");
        modelRating->setValue(8, 0, 0);

        m_images->setValue(9, 0, QPixmap("://img/borzoi_01_sm.jpg"));
        m_names->setValue(9, 0, "Borzoi");
        m_descriptions->setValue(9, 0, "The Borzoi, also called the Russian wolfhound, is a breed of domestic dog (Canis lupus familiaris). Descended from dogs brought to Russia from central Asian countries, it is similar in shape to a greyhound, and is also a member of the sighthound family.");
        modelRating->setValue(9, 0, 5);

        m_images->setValue(10, 0, QPixmap("://img/bulldog_01_sm.jpg"));
        m_names->setValue(10, 0, "Bulldog");
        m_descriptions->setValue(10, 0, "The Bulldog is a medium-sized breed of dog commonly referred to as the English Bulldog or British Bulldog. The Bulldog is a muscular, heavy dog with a wrinkled face and a distinctive pushed-in nose.");
        modelRating->setValue(10, 0, 1);

        m_images->setValue(11, 0, QPixmap("://img/vizsla_01_sm.jpg"));
        m_names->setValue(11, 0, "Vizsla");
        m_descriptions->setValue(11, 0, "The Vizsla is a dog breed originating in Hungary, which belongs under the FCI group 7 (Pointer group). The Hungarian or Magyar Vizsla are sporting dogs and loyal companions, in addition to being the smallest of the all-round pointer-retriever breeds.");
        modelRating->setValue(11, 0, 2);

        m_images->setValue(12, 0, QPixmap("://img/dalmation_01_sm.jpg"));
        m_names->setValue(12, 0, "Dalmatian");
        m_descriptions->setValue(12, 0, "The Dalmatian is a large breed of dog noted for its unique black or liver spotted coat and was mainly used as a carriage dog in its early days. Its roots trace back to Croatia and its historical region of Dalmatia. Today, this dog remains a well-loved family pet, and many dog enthusiasts enter their pets into kennel club competitions.");
        modelRating->setValue(12, 0, 1);

        m_images->setValue(13, 0, QPixmap("://img/english-setter_01_sm.jpg"));
        m_names->setValue(13, 0, "English Setter");
        m_descriptions->setValue(13, 0, "The English Setter is a medium size breed of dog. It is part of the Setter family, which includes the red Irish Setters, Irish Red and White Setters, and black-and-tan Gordon Setters. The mainly white body coat is of medium length with long silky fringes on the back of the legs, under the belly and on the tail.");
        modelRating->setValue(13, 0, 4);

        m_images->setValue(14, 0, QPixmap("://img/greater-swiss-mountain-dog_01_sm.jpg"));
        m_names->setValue(14, 0, "Greater Swiss Mountain Dog");
        m_descriptions->setValue(14, 0, "The Greater Swiss Mountain Dog (German: Grosser Schweizer Sennenhund or French: Grand Bouvier Suisse) is a dog breed which was developed in the Swiss Alps. The name Sennenhund refers to people called Senn or Senner, dairymen and herders in the Swiss Alps.");
        modelRating->setValue(14, 0, 3);

        m_images->setValue(15, 0, QPixmap("://img/irish-water-spaniel_01_sm.jpg"));
        m_names->setValue(15, 0, "Irish Water Spaniel");
        m_descriptions->setValue(15, 0, "The Irish Water Spaniel (Irish: An Spáinnéar Uisce) is a breed of dog that is the largest and one of the oldest of spaniels. The Irish Water Spaniel is one of the rarer breeds with the AKC in terms of registrations.");
        modelRating->setValue(15, 0, 5);

        m_images->setValue(16, 0, QPixmap("://img/neopolitan-mastiff_01_sm.jpg"));
        m_names->setValue(16, 0, "Neapolitan Mastiff");
        m_descriptions->setValue(16, 0, "The Neapolitan Mastiff or Italian Mastiff, (Italian: Mastino Napoletano) is a large, ancient dog breed. This massive breed is often used as a guard and defender of family and property due to their protective instincts and their fearsome appearance.");
        modelRating->setValue(16, 0, 5);

        m_images->setValue(17, 0, QPixmap("://img/poodle-standard_01_sm.jpg"));
        m_names->setValue(17, 0, "Poodle");
        m_descriptions->setValue(17, 0, "The poodle is a group of formal dog breeds, the Standard Poodle, Miniature Poodle and Toy Poodle (one registry organisation also recognizes a Medium Poodle variety, between Standard and Miniature), with many coat colors. Originally bred in Germany as a type of water dog, the breed was standardized in France.");
        modelRating->setValue(17, 0, 2);

        m_images->setValue(18, 0, QPixmap("://img/shiba-inu_01_sm.jpg"));
        m_names->setValue(18, 0, "Shiba Inu");
        m_descriptions->setValue(18, 0, "The Shiba Inu is the smallest of the six original and distinct spitz breeds of dog from Japan. A small, agile dog that copes very well with mountainous terrain, the Shiba Inu was originally bred for hunting.");
        modelRating->setValue(18, 0, 3);

        m_images->setValue(19, 0, QPixmap("://img/siberian-husky_01_sm.jpg"));
        m_names->setValue(19, 0, "Siberian Husky");
        m_descriptions->setValue(19, 0, "The Siberian Husky is a medium size, dense-coat working dog breed that originated in north-eastern Siberia. The breed belongs to the Spitz genetic family. It is recognizable by its thickly furred double coat, erect triangular ears, and distinctive markings.");
        modelRating->setValue(19, 0, 2);
    }

    // setup alternate background
    //grid.addSchema(makeRangeAll(), QSharedPointer<ViewAlternateBackground>::create(), makeLayoutBackground());

    QFont boldFont = ui->listWidget->font();
    boldFont.setBold(true);

    auto viewImages = QSharedPointer<ViewPixmap>::create(m_images);

    grid.addSchema(makeRangeColumn(0), QSharedPointer<ViewRowBorder>::create(), makeLayoutBackground());
    grid.addSchema(makeRangeColumn(0), viewImages, makeLayoutLeft());

    {
        QVector<ViewSchema> subViews;
        subViews.append(ViewSchema(makeLayoutBackground(), QSharedPointer<ViewTextFont>::create(boldFont)));
        m_ratingView = QSharedPointer<ViewRating>::create(modelRating, QPixmap("://img/StarOn.png"), QPixmap("://img/StarOff.png"));
        subViews.append(ViewSchema(makeLayoutRight(), m_ratingView));
        subViews.append(ViewSchema(makeLayoutClient(), QSharedPointer<ViewText>::create(m_names, ViewDefaultControllerNone, Qt::AlignHCenter|Qt::AlignTop, Qt::ElideRight)));
        QMargins margins(0, 2, 0, 2);
        grid.addSchema(makeRangeColumn(0), QSharedPointer<ViewComposite>::create(subViews, margins), makeLayoutTop());
    }

    grid.addSchema(makeRangeColumn(0), QSharedPointer<ViewText>::create(m_descriptions, ViewDefaultControllerNone, Qt::Alignment(Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap)), makeLayoutClient());

    {
        auto wikiModel = QSharedPointer<ModelCallback<QUrl>>::create();
        wikiModel->getValueFunction = [this](const ItemID& item)->QUrl {
            return QUrl(QString("http://en.wikipedia.org/wiki/%1").arg(m_names->value(item)));
        };
        m_wikiView = QSharedPointer<ViewLink>::create(QSharedPointer<ModelStorageValue<QString>>::create("more"), wikiModel);
        QVector<ViewSchema> subViews;
        subViews.append(ViewSchema(makeLayoutRight(), m_wikiView));
        grid.addSchema(makeRangeColumn(0), QSharedPointer<ViewComposite>::create(subViews), makeLayoutBottom());
    }

    // animation
    m_animation = new CacheSpaceAnimationShiftViewsRight(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data());
    m_animation->setEasingCurve(QEasingCurve::OutCirc);
    m_animation->start();
}

MainWindow::~MainWindow()
{
    if (!m_animation.isNull())
        delete m_animation.data();

    delete ui;
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

        // don't delete while m_animation is emitting stopped signal
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
    playAnimation(new CacheSpaceAnimationShiftViewsRight(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data()));
}

void MainWindow::on_shiftLeftBttn_clicked()
{
    playAnimation(new CacheSpaceAnimationShiftViewsLeft(ui->listWidget->viewport(), ui->listWidget->cacheGrid().data()), QEasingCurve::InOutQuart);
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

        auto v = (View*)info.cacheView->view();

        // animate images
        if (v->model() == m_images.data())
        {
            auto subAnimation = new QVariantAnimation(animation);
            subAnimation->setDuration(1000);
            int radius = (int)hypot(qreal(info.cacheView->rect().width()), qreal(info.cacheView->rect().height()));
            subAnimation->setStartValue(0);
            subAnimation->setEndValue(radius);

            info.cacheView->drawProxy = [subAnimation](const CacheView* cacheView, QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect* visibleRect) {

                painter->save();

                QPointF c = cacheView->rect().center();
                int r = subAnimation->currentValue().toInt();

                QPainterPath path;
                path.addEllipse(c, (qreal)r, (qreal)r);
                painter->setClipPath(path, Qt::IntersectClip);
                cacheView->drawRaw(painter, ctx, item, itemRect, visibleRect);

                painter->restore();
            };

            animation->addAnimation(subAnimation);
        }
        // animate others
        else if (v->model() == m_descriptions.data() ||
                 v->model() == m_names.data() ||
                 v == m_wikiView.data() ||
                 v == m_ratingView.data())
        {
            auto subAnimation = new QVariantAnimation(animation);
            subAnimation->setDuration(1000);
            subAnimation->setStartValue(0.f);
            subAnimation->setEndValue(1.f);

            info.cacheView->drawProxy = [subAnimation](const CacheView* cacheView, QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect* visibleRect) {

                float progress = subAnimation->currentValue().toFloat();

                qreal oldOpacity = painter->opacity();
                painter->setOpacity(progress);

                cacheView->drawRaw(painter, ctx, item, itemRect, visibleRect);

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
    cacheSpace->forEachCacheItem([mainAnimation, animation, &ctx, cacheSpace, &itemIndex, painter](const QSharedPointer<CacheItem>& cacheItem)->bool {

        // create image of the item
        {
            QPixmap itemPixmap(cacheItem->rect.size());
            itemPixmap.fill(Qt::transparent);
            QPainter itemPainter(&itemPixmap);
            itemPainter.setWindow(cacheItem->rect);
            itemPainter.setRenderHints(painter->renderHints());
            itemPainter.setBackgroundMode(painter->backgroundMode());

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
    cacheSpace->forEachCacheItem([mainAnimation, animation, &ctx, cacheSpace, &itemIndex, painter](const QSharedPointer<CacheItem>& cacheItem)->bool {

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
            itemPainter.setWindow(cacheItem->rect);
            itemPainter.setRenderHints(painter->renderHints());
            itemPainter.setBackgroundMode(painter->backgroundMode());

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
    {
        cachePixmap.fill(Qt::transparent);
        QPainter cachePainter(&cachePixmap);
        cachePainter.setWindow(cacheSpace->window());
        cachePainter.setRenderHints(painter->renderHints());
        cachePainter.setBackgroundMode(painter->backgroundMode());

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
    {
        cachePixmap.fill(Qt::transparent);
        QPainter cachePainter(&cachePixmap);
        cachePainter.setWindow(cacheSpace->window());
        cachePainter.setRenderHints(painter->renderHints());
        cachePainter.setBackgroundMode(painter->backgroundMode());

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
