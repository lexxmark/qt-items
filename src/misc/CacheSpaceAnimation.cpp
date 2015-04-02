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
    void updateCurrentTime(int /*currentTime*/) override
    {
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
      m_state(Stopped)
{
}

CacheSpaceAnimationAbstract::~CacheSpaceAnimationAbstract()
{
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

    m_state = Running;

    connect(m_cacheSpace, &CacheSpace::cacheChanged, this, &CacheSpaceAnimationAbstract::onCacheChanged);

    m_animation = createAnimationImpl(m_cacheSpace, painter, ctx);
    Q_ASSERT(m_animation);
    m_auxAnimation = new Impl::AuxAnimation(this);

    callLater(this, [this](){
        m_animation->setDirection(m_direction);
        m_animation->start(QAbstractAnimation::DeleteWhenStopped);
        m_auxAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
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
    Q_ASSERT(m_animation);
    Q_ASSERT(m_auxAnimation);

    disconnect(m_cacheSpace, &CacheSpace::cacheChanged, this, &CacheSpaceAnimationAbstract::onCacheChanged);
    m_cacheSpace->setDrawProxy(nullptr);
    m_cacheSpace->clear();

    m_animation = nullptr;
    m_auxAnimation = nullptr;

    m_state = Stopped;

    emit stopped();
}

QAbstractAnimation* CacheSpaceAnimationShiftRight::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);
    // init
    cacheSpace->forEachCacheView([this, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {
        auto ss = new QSequentialAnimationGroup(animation);

        auto va = new QVariantAnimation(ss);
        va->setDuration(1000);
        va->setEasingCurve(easingCurve());
        QRect startRect = info.cacheView->rect();
        startRect.moveTo(cacheSpace->window().left() - startRect.width(), startRect.top());
        va->setStartValue(startRect);
        va->setEndValue(info.cacheView->rect());
        CacheView* cv = info.cacheView;
        connect(va, &QVariantAnimation::valueChanged, [cv](const QVariant &value){
            cv->rRect() = value.toRect();
        });
        ss->addPause(info.cacheItemIndex*100);
        ss->addAnimation(va);

        animation->addAnimation(ss);

        if (direction() == QAbstractAnimation::Forward)
        {
            info.cacheView->rRect() = startRect;
        }

        return true;
    });

    return animation;
}

QAbstractAnimation* CacheSpaceAnimationShiftLeft::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);
    // init
    cacheSpace->forEachCacheView([this, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {
        auto ss = new QSequentialAnimationGroup(animation);

        auto va = new QVariantAnimation(ss);
        va->setDuration(1000);
        va->setEasingCurve(easingCurve());
        QRect startRect = info.cacheView->rect();
        startRect.moveTo(cacheSpace->window().right(), startRect.top());
        va->setStartValue(startRect);
        va->setEndValue(info.cacheView->rect());
        CacheView* cv = info.cacheView;
        connect(va, &QVariantAnimation::valueChanged, [cv](const QVariant &value){
            cv->rRect() = value.toRect();
        });
        ss->addPause(info.cacheItemIndex*100);
        ss->addAnimation(va);

        animation->addAnimation(ss);

        if (direction() == QAbstractAnimation::Forward)
        {
            info.cacheView->rRect() = startRect;
        }

        return true;
    });

    return animation;
}

QAbstractAnimation* CacheSpaceAnimationShiftRandom::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);
    QRect randomArea = cacheSpace->window();
    randomArea.adjust(-2*randomArea.width(), -2*randomArea.height(), 2*randomArea.width(), 2*randomArea.height());

    cacheSpace->forEachCacheView([this, &randomArea, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {
        auto ss = new QSequentialAnimationGroup(animation);

        auto va = new QVariantAnimation(ss);
        va->setDuration(1000);
        va->setEasingCurve(easingCurve());
        QRect startRect;

        do
        {
            //startRect = info.cacheView->rect();
            QPoint randomPoint;
            randomPoint.rx() = randomArea.left() + std::rand()%randomArea.width();
            randomPoint.ry() = randomArea.top() + std::rand()%randomArea.height();
            startRect = QRect(randomPoint, QSize(1, 1));
        } while (startRect.intersects(cacheSpace->window()));

        va->setStartValue(startRect);
        va->setEndValue(info.cacheView->rect());
        CacheView* cv = info.cacheView;
        connect(va, &QVariantAnimation::valueChanged, [cv](const QVariant &value){
            cv->rRect() = value.toRect();
        });
        ss->addPause(info.cacheItemIndex*100);
        ss->addAnimation(va);

        animation->addAnimation(ss);

        if (direction() == QAbstractAnimation::Forward)
        {
            info.cacheView->rRect() = startRect;
        }

        return true;
    });

    return animation;
}



} // end namespace Qi

