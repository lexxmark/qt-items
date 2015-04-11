/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "CacheSpaceAnimation.h"

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

    if (m_cacheSpace->animation())
        return false;

    m_state = Started;
    m_policy = policy;
    m_direction = direction;
    m_cacheSpace->setAnimation(this);
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

void CacheSpaceAnimationAbstract::drawCacheSpace(const CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx)
{
    Q_UNUSED(cacheSpace);
    Q_ASSERT(m_cacheSpace == cacheSpace);
    Q_ASSERT(m_widget == ctx.widget);
    Q_ASSERT(m_state != Stopped);

    if (m_state == Running)
        return;

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

    m_cacheSpace->setAnimation(nullptr);

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

    return animationFactory(this, cacheSpace, painter, ctx);
}

CacheSpaceAnimationShiftViews::CacheSpaceAnimationShiftViews(QWidget* widget, CacheSpace* cacheSpace, CacheSpaceAnimationShiftDirection direction, const View* viewToApply)
    : CacheSpaceAnimationAbstract(widget, cacheSpace),
      m_direction(direction),
      m_viewToApply(viewToApply)
{
}

QAbstractAnimation* CacheSpaceAnimationShiftViews::createAnimationImpl(CacheSpace* cacheSpace, QPainter* /*painter*/, const GuiContext& ctx)
{
    cacheSpace->validate(ctx);

    auto animation = new QParallelAnimationGroup(this);

    // enumerate all cache views
    cacheSpace->forEachCacheView([this, animation, cacheSpace](const CacheSpace::IterateInfo& info)->bool {

        // skip non target views
        if (m_viewToApply && (m_viewToApply != info.cacheView->view()))
            return true;

        auto subAnimation = new QSequentialAnimationGroup(animation);

        auto rectAnimation = new QVariantAnimation(subAnimation);
        rectAnimation->setDuration(1000);
        rectAnimation->setEasingCurve(easingCurve());
        QRect startRect = info.cacheView->rect();
        moveToStart(cacheSpace, startRect);
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

void CacheSpaceAnimationShiftViews::moveToStart(const CacheSpace* cacheSpace, QRect& rect) const
{
    switch (m_direction) {

    case CacheSpaceAnimationShiftRight:
        rect.moveTo(cacheSpace->window().left() - rect.width(), rect.top());
        break;

    case CacheSpaceAnimationShiftLeft:
        rect.moveTo(cacheSpace->window().right(), rect.top());
        break;

    case CacheSpaceAnimationShiftTop:
        rect.moveTo(rect.left(), cacheSpace->window().top() - rect.height());
        break;

    case CacheSpaceAnimationShiftBottom:
        rect.moveTo(rect.left(), cacheSpace->window().bottom());
        break;

    default:
        Q_ASSERT(false);
        break;
    }
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

