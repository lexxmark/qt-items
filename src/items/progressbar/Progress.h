#ifndef QI_PROGRESS_H
#define QI_PROGRESS_H

#include "core/ext/ModelCallback.h"
#include "core/ext/ViewModeled.h"

namespace Qi
{

typedef ModelTyped<float> ModelProgress;
typedef ModelCallback<float> ModelProgressCallback;

class QI_EXPORT ViewProgressBase: public ViewModeled<ModelProgress>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewProgressBase)

public:
    ViewProgressBase(const QSharedPointer<ModelProgress>& model);

    std::function<bool(float& value, const ItemID& item)> tuneValue;
};

class QI_EXPORT ViewProgressContents: public ViewProgressBase
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewProgressContents)

public:
    ViewProgressContents(const QSharedPointer<ModelProgress>& model);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
};

enum ProgressLabelMode
{
    ProgressLabelModeNormalized, // show value in [0,1], ex. 0.34
    ProgressLabelModeProgress, // show value in [0, 100], ex. 62.32
    ProgressLabelModePercent // show value in [0, 100] with percentile symbol, ex. 23.54%
};

class QI_EXPORT ViewProgressLabel: public ViewProgressBase
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewProgressLabel)

public:
    ViewProgressLabel(const QSharedPointer<ModelProgress>& model, ProgressLabelMode mode = ProgressLabelModeProgress);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    ProgressLabelMode m_mode;
};

class QI_EXPORT ViewProgressBox: public ViewProgressBase
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewProgressBox)

public:
    ViewProgressBox(const QSharedPointer<ModelProgress>& model);

    QColor contentsColor;
    QColor boundsColor;

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
};

} // end namespace Qi

#endif // QI_PROGRESS_H
