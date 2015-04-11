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

#ifndef QI_CACHE_SPACE_ANIMATION_H
#define QI_CACHE_SPACE_ANIMATION_H

#include "cache/space/CacheSpace.h"
#include <QAbstractAnimation>
#include <QEasingCurve>
#include <functional>

class QWidget;

namespace Qi
{

namespace Impl
{
    class AuxAnimation;
}

class QI_EXPORT CacheSpaceAnimationAbstract: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationAbstract)

public:
    CacheSpaceAnimationAbstract(QWidget* widget, CacheSpace* cacheSpace);
    virtual ~CacheSpaceAnimationAbstract();

    QEasingCurve easingCurve() const { return m_easingCurve; }
    void setEasingCurve(const QEasingCurve & easing) { m_easingCurve = easing; }

    QAbstractAnimation::Direction direction() const { return m_direction; }

    bool start(QAbstractAnimation::Direction direction = QAbstractAnimation::Forward, QAbstractAnimation::DeletionPolicy policy = QAbstractAnimation::KeepWhenStopped);
    bool stop();

signals:
    void stopped();

protected:
    virtual QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) = 0;

private:
    friend class Impl::AuxAnimation;
    friend class CacheSpace;

    void drawCacheSpace(const CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx);
    void onCacheChanged(const CacheSpace* cache, ChangeReason reason);
    void onAuxAnimationStopped();

    QEasingCurve m_easingCurve;
    QAbstractAnimation* m_animation;
    Impl::AuxAnimation* m_auxAnimation;

    QWidget* m_widget;
    CacheSpace* m_cacheSpace;

    QAbstractAnimation::DeletionPolicy m_policy;
    QAbstractAnimation::Direction m_direction;

    enum State
    {
        Stopped,
        Started,
        Running
    };

    State m_state;
    bool m_destruction;
};

class QI_EXPORT CacheSpaceAnimationCallback : public CacheSpaceAnimationAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationCallback)

public:
    CacheSpaceAnimationCallback(QWidget* widget, CacheSpace* cacheSpace)
        : CacheSpaceAnimationAbstract(widget, cacheSpace)
    {}

    std::function<QAbstractAnimation* (const CacheSpaceAnimationAbstract*, CacheSpace*, QPainter*, const GuiContext&)> animationFactory;

protected:
    QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) override;
};

enum CacheSpaceAnimationShiftDirection
{
    CacheSpaceAnimationShiftLeft,
    CacheSpaceAnimationShiftRight,
    CacheSpaceAnimationShiftTop,
    CacheSpaceAnimationShiftBottom
};

class QI_EXPORT CacheSpaceAnimationShiftViews : public CacheSpaceAnimationAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationShiftViews)

public:
    CacheSpaceAnimationShiftViews(QWidget* widget, CacheSpace* cacheSpace, CacheSpaceAnimationShiftDirection direction, const View* viewToApply = nullptr);

protected:
    QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) override;

private:
    void moveToStart(const CacheSpace* cacheSpace, QRect& rect) const;

    CacheSpaceAnimationShiftDirection m_direction;
    const View* m_viewToApply;
};

class QI_EXPORT CacheSpaceAnimationShiftViewsRandom : public CacheSpaceAnimationAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationShiftViewsRandom)

public:
    CacheSpaceAnimationShiftViewsRandom(QWidget* widget, CacheSpace* cacheSpace)
        : CacheSpaceAnimationAbstract(widget, cacheSpace)
    {}

protected:
    QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) override;
};

} // end namespace Qi

#endif // QI_CACHE_SPACE_ANIMATION_H
