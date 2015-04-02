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

    void drawProxy(const CacheSpace& cacheSpace, QPainter* painter, const GuiContext& ctx);
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

class CacheSpaceAnimationCallback : public CacheSpaceAnimationAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationCallback)

public:
    CacheSpaceAnimationCallback(QWidget* widget, CacheSpace* cacheSpace)
        : CacheSpaceAnimationAbstract(widget, cacheSpace)
    {}

    std::function<QAbstractAnimation* (CacheSpace*, QPainter*, const GuiContext&)> animationFactory;

protected:
    QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) override;
};

class CacheSpaceAnimationShiftViewsRight : public CacheSpaceAnimationAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationShiftViewsRight)

public:
    CacheSpaceAnimationShiftViewsRight(QWidget* widget, CacheSpace* cacheSpace)
        : CacheSpaceAnimationAbstract(widget, cacheSpace)
    {}

protected:
    QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) override;
};

class CacheSpaceAnimationShiftViewsLeft : public CacheSpaceAnimationAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceAnimationShiftViewsLeft)

public:
    CacheSpaceAnimationShiftViewsLeft(QWidget* widget, CacheSpace* cacheSpace)
        : CacheSpaceAnimationAbstract(widget, cacheSpace)
    {}

protected:
    QAbstractAnimation* createAnimationImpl(CacheSpace* cacheSpace, QPainter* painter, const GuiContext& ctx) override;
};

class CacheSpaceAnimationShiftViewsRandom : public CacheSpaceAnimationAbstract
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
