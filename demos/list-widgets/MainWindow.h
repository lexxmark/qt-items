#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEasingCurve>
#include "items/image/Pixmap.h"
#include "items/text/Text.h"
#include "items/rating/Rating.h"

class QAbstractAnimation;

namespace Ui {
class MainWindow;
}

namespace Qi {
class CacheSpace;
class GuiContext;
class CacheSpaceAnimationAbstract;
class ListColumnsResizer;
class ViewButton;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_shiftRightBttn_clicked();
    void on_shiftLeftBttn_clicked();
    void on_shiftRandomBttn_clicked();
    void on_circleImageBttn_clicked();
    void on_shiftRightItemBttn_clicked();
    void on_shiftDownItemBttn_clicked();
    void on_shiftBottomRightBttn_clicked();
    void on_fadeBttn_clicked();

    void on_clearData_clicked();

    void on_alphabetOrderBttn_clicked();

    void on_starsOrderBttn_clicked();

    void on_createImageBttn_clicked();

private:
    void loadData();
    void shuffleRows();
    void playAnimation(Qi::CacheSpaceAnimationAbstract* animation, const QEasingCurve & easing = QEasingCurve::OutCirc);
    void onLoadBttnPressed(const Qi::ItemID&, const Qi::ControllerContext&, const Qi::ViewButton*);

    QAbstractAnimation* createCircleViewAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const;
    QAbstractAnimation* createShiftRightItemAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const;
    QAbstractAnimation* createShiftDownItemAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const;
    QAbstractAnimation* createShiftBottomRightAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const;
    QAbstractAnimation* createFadeAnimation(const Qi::CacheSpaceAnimationAbstract* mainAnimation, Qi::CacheSpace* cacheSpace, QPainter* painter, const Qi::GuiContext& ctx) const;

    Ui::MainWindow *ui;
    QSharedPointer<Qi::ModelPixmap> m_images;
    QSharedPointer<Qi::ModelText> m_names;
    QSharedPointer<Qi::ModelText> m_descriptions;
    QSharedPointer<Qi::ModelRating> m_rates;
    QSharedPointer<Qi::View> m_wikiView;
    QSharedPointer<Qi::ListColumnsResizer> m_resizer;

    QPointer<Qi::CacheSpaceAnimationAbstract> m_animation;
};

#endif // MAINWINDOW_H
