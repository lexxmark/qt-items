#include "CacheSpaceAnimation.h"
#include "utils/CallLater.h"
#include "utils/MemFunction.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QVariantAnimation>

namespace Qi
{

namespace Impl
{

class AuxAnimation: public QAbstractAnimation
{
public:
    AuxAnimation(CacheSpaceAnimationAbstract* owner)
        : m_owner(owner)
    {}

    int duration () const override
    {
        return m_owner->m_animation ? m_owner->m_animation->duration() : 0;
    }

protected:
    void updateCurrentTime(int currentTime) override
    {
        // don't repaint at start
        if (currentTime != 0)
            m_owner->m_widget->repaint();
    }

    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State /*oldState*/) override
    {
        if (newState == Stopped)
        {
            m_owner->onAuxAnimationStopped();
        }
    }

private:
    CacheSpaceAnimationAbstract* m_owner;
};

}

CacheSpaceAnimationAbstract::CacheSpaceAnimationAbstract(QWidget* widget, CacheSpace* cacheSpace)
    : QObject(widget),
      m_animation(nullptr),
      m_auxAnimation(nullptr),
      m_widget(widget),
      m_cacheSpace(cacheSpace),
      m_policy(QAbstractAnimation::DeleteWhenStopped),
      m_direction(QAbstractAnimation::Forward),
      m_state(Stopped),
      m_destruction(false)
{
}

CacheSpaceAnimationAbstract::~CacheSpaceAnimationAbstract()
{
    m_destruction = true;
    stop();
}

bool CacheSpaceAnimationAbstract::start(QAbstractAnimation::Direction direction, QAbstractAnimation::DeletionPolicy policy)
{
    if (m_state != Stopped)
        return false;

    if (m_cacheSpace->hasDrawProxy())
        return false;

    m_state = Started;
    m_policy = policy;
    m_direction = direction;
    m_cacheSpace->setDrawProxy(memFunction(this, &CacheSpaceAnimationAbstract::drawProxy));
    m_widget->update();

    return true;
}

bool CacheSpaceAnimationAbstract::stop()
{
    if (m_state == Stopped)
        return false;

    if (m_state == Started)
    {
        onAuxAnimationStopped();
    }
    else
    {
        Q_ASSERT(m_animation && m_auxAnimation);
        m_animation->stop();
        m_auxAnimation->stop();
    }

    return true;
}

void CacheSpaceAnimationAbstract::drawProxy(const CacheSpace& cacheSpace, QPainter* painter, const GuiContext& ctx)
{
    Q_UNUSED(cacheSpace);
    Q_ASSERT(m_cacheSpace == &cacheSpace);
    Q_ASSERT(m_widget == ctx.widget);
    Q_ASSERT(m_state != Stopped);

    if (m_state == Running)
    {
        m_cacheSpace->drawRaw(painter, ctx);
        return;
    }

    Q_ASSERT(!m_animation);
    Q_ASSERT(!m_auxAnimation);
    Q_ASSERT(m_state == Started);

    m_animation = createAnimationImpl(m_cacheSpace, painter, ctx);
    if (!m_animation)
    {
        // error
        Q_ASSERT(false);
        stop();
        return;
    }

    m_animation->setDirection(m_direction);
    m_auxAnimation = new Impl::AuxAnimation(this);
    connect(m_cacheSpace, &CacheSpace::cacheChanged, this, &CacheSpaceAnimationAbstract::onCacheChanged);
    m_state = Running;

    m_animation->start(QAbstractAnimation::DeleteWhenStopped);
    m_auxAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    m_cacheSpace->drawRaw(painter, ctx);
}

void CacheSpaceAnimationAbstract::onCacheChanged(const CacheSpace* cache, ChangeReason reason)
{
    Q_UNUSED(cache);
    Q_UNUSED(reason);
    Q_ASSERT(cache == m_cacheSpace);
    Q_ASSERT(m_state != Stopped);

    if (reason & ChangeReasonCacheItems)
        stop();
}

void CacheSpaceAnimationAbstract::onAuxAnimationStopped()
{
    Q_ASSERT(m_state != Stopped);

    m_cacheSpace->setDrawProxy(nullptr);

    if (m_state == Running)
    {
        Q_ASSERT(m_animation && m_auxAnimation);
        disconnect(m_cacheSpace, &CacheSpace::cacheChanged, this, &CacheSpaceAnimationAbstract::onCacheChanged);
        m_cacheSpace->clear();

        m_animation = nullptr;
        m_auxAnimation = nullptr;
    }

    m_state = Stopped;

    if (!m_destruction)
        emit stopped();
}

QAbstractAnimation* CacheSpaceAnimationCallback::createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx)
{
    if (!animationFactory)
        return nullptr;

    return animationFactory(cacheSpace, painter, ctx);
}

QAbstractAnimation* CacheSpaceAnimationShiftViewsRight::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);
    // init
    cacheSpace->forEachCacheView([this, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {

        auto subAnimation = new QSequentialAnimationGroup(animation);

        auto rectAnimation = new QVariantAnimation(subAnimation);
        rectAnimation->setDuration(1000);
        rectAnimation->setEasingCurve(easingCurve());
        QRect startRect = info.cacheView->rect();
        startRect.moveTo(cacheSpace->window().left() - startRect.width(), startRect.top());
        rectAnimation->setStartValue(startRect);
        rectAnimation->setEndValue(info.cacheView->rect());
        CacheView* cv = info.cacheView;
        connect(rectAnimation, &QVariantAnimation::valueChanged, [cv](const QVariant &value){
            cv->rRect() = value.toRect();
        });
        subAnimation->addPause(info.cacheItemIndex*100);
        subAnimation->addAnimation(rectAnimation);

        animation->addAnimation(subAnimation);

        if (direction() == QAbstractAnimation::Forward)
        {
            info.cacheView->rRect() = startRect;
        }

        return true;
    });

    return animation;
}

QAbstractAnimation* CacheSpaceAnimationShiftViewsLeft::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);
    // init
    cacheSpace->forEachCacheView([this, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {

        auto subAnimation = new QSequentialAnimationGroup(animation);

        auto rectAnimation = new QVariantAnimation(subAnimation);
        rectAnimation->setDuration(1000);
        rectAnimation->setEasingCurve(easingCurve());
        QRect startRect = info.cacheView->rect();
        startRect.moveTo(cacheSpace->window().right(), startRect.top());
        rectAnimation->setStartValue(startRect);
        rectAnimation->setEndValue(info.cacheView->rect());
        CacheView* cv = info.cacheView;
        connect(rectAnimation, &QVariantAnimation::valueChanged, [cv](const QVariant &value){
            cv->rRect() = value.toRect();
        });
        subAnimation->addPause(info.cacheItemIndex*100);
        subAnimation->addAnimation(rectAnimation);

        animation->addAnimation(subAnimation);

        if (direction() == QAbstractAnimation::Forward)
        {
            info.cacheView->rRect() = startRect;
        }

        return true;
    });

    return animation;
}

QAbstractAnimation* CacheSpaceAnimationShiftViewsRandom::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);
    QRect randomArea = cacheSpace->window();
    randomArea.adjust(-2*randomArea.width(), -2*randomArea.height(), 2*randomArea.width(), 2*randomArea.height());

    cacheSpace->forEachCacheView([this, &randomArea, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {

        auto subAnimation = new QSequentialAnimationGroup(animation);

        auto rectAnimation = new QVariantAnimation(subAnimation);
        rectAnimation->setDuration(1000);
        rectAnimation->setEasingCurve(easingCurve());

        // generate random start rect which is out of visible window
        QRect startRect;
        do
        {
            QPoint randomPoint;
            randomPoint.rx() = randomArea.left() + std::rand()%randomArea.width();
            randomPoint.ry() = randomArea.top() + std::rand()%randomArea.height();
            startRect = QRect(randomPoint, QSize(1, 1));
        } while (startRect.intersects(cacheSpace->window()));

        rectAnimation->setStartValue(startRect);
        rectAnimation->setEndValue(info.cacheView->rect());
        CacheView* cv = info.cacheView;
        connect(rectAnimation, &QVariantAnimation::valueChanged, [cv](const QVariant &value){
            cv->rRect() = value.toRect();
        });
        subAnimation->addPause(info.cacheItemIndex*100);
        subAnimation->addAnimation(rectAnimation);

        animation->addAnimation(subAnimation);

        if (direction() == QAbstractAnimation::Forward)
        {
            info.cacheView->rRect() = startRect;
        }

        return true;
    });

    return animation;
}



} // end namespace Qi

