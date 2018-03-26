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

#ifndef QI_LINES_H
#define QI_LINES_H

#include "QiAPI.h"
#include <QObject>
#include <QVector>
#include <functional>

namespace Qi
{

class LinesVisibility;

class QI_EXPORT Lines: public QObject
{
    Q_OBJECT

public:
    Lines(int count = 0);

    QSharedPointer<Lines> clone() const;

    int count() const { return m_count; }
    void setCount(int count);

    int visibleCount() const;
    int visibleSize() const;

    bool isEmpty() const { return m_count == 0; }
    bool isEmptyVisible() const { return visibleCount() == 0; }

    int lineSize(int line) const;
    void setLineSize(int line, int size);
    void setLineSizeAll(int size);

    bool isLineVisible(int line) const;
    // returns 0 - all invisible
    // returns 1 - all visible
    // returns -1 - some visible and invisible
    int isLinesVisibleAll() const;
    void setLineVisible(int line, bool visible);
    void setLineVisibleAll(bool visible);

    void setLinesVisible(const QVector<int>& lines, bool visible);
    void setLinesVisibleExact(const QVector<int>& lines, bool visible);

    bool addLinesVisibility(const QSharedPointer<LinesVisibility>& linesVisibility);
    bool removeLinesVisibility(const QSharedPointer<LinesVisibility>& linesVisibility);
    void clearLinesVisibility();

    int moveLines(int oldAbsoluteLine, int newRelativeLine, int linesCount = 1);
    int moveVisibleLines(int oldLine, int newLine, int linesCount = 1);
    int insertVisibleLines(int lineBefore, int linesCount = 1);

    int toAbsolute(int visibleLine) const { validateVisibles(); Q_ASSERT(visibleLine >= 0 && visibleLine < m_visible2absolute.size()); return m_visible2absolute[visibleLine]; }
    int toVisible(int absoluteLine) const { validateVisibles(); Q_ASSERT(absoluteLine >= 0 && absoluteLine < m_absolute2visible.size()); return m_absolute2visible[absoluteLine]; }

    int toAbsoluteSafe(int visibleLine) const { validateVisibles(); return (visibleLine < m_visible2absolute.size()) ? m_visible2absolute[visibleLine] : InvalidIndex; }
    int toVisibleSafe(int absoluteLine) const { validateVisibles(); return (absoluteLine < m_absolute2visible.size()) ? m_absolute2visible[absoluteLine] : InvalidIndex; }

    // see m_visibleLinesSizes for possible return values
    int findVisibleIDByPos(int position, bool noTailLine = true) const;
    int findVisibleIDByPos(int position, int fromVisibleLine, int toVisibleLine) const;

    int startPos(int visibleLine) const;
    int endPos(int visibleLine) const;

    // pred has less operator - bool operator() (int leftLine, int rightLine) const;
    template <typename Pred> void sort(bool stable, const Pred& pred)
    {
        if (stable)
            std::stable_sort(m_relative2absolute.begin(), m_relative2absolute.end(), pred);
        else
            std::sort(m_relative2absolute.begin(), m_relative2absolute.end(), pred);

        invalidateVisibles();
        emit linesChanged(this, ChangeReasonLinesOrder);
    }

    // permutation[relativeID] == absoluteID
    const QVector<int>& permutation() const { return m_relative2absolute; }
    void setPermutation(const QVector<int>& permutation);

signals:
    void linesChanged(const Lines*, ChangeReason);

private:
    Lines(const Lines& lines);
    Lines& operator=(const Lines&);

    bool isLineVisibleRaw(int line) const;

    int findVisibleIDByPosImpl(int position, int fromVisibleLine, int toVisibleLine) const;

    void invalidateVisibles() { m_visible2absolute.clear(); m_absolute2visible.clear(); invalidateSizes(); }
    void validateVisibles() const;

    void invalidateSizes() { m_visibleLinesSizes.clear(); }
    void validateSizes() const;

    void onLinesVisibilityChanged(const LinesVisibility*);

    // lines count
    int m_count;

    // lines sizes
    // m_linesSize.empty - all lines has DEFAULT_LINE_SIZE size
    // m_linesSize.size() == 1 - all lines has size m_linesSize[0]
    QVector<int> m_linesSize;
    // lines visible
    // m_linesVisible.empty - all lines has DEFAULT_LINE_VISIBILITY visibility
    // m_linesVisible.size() == 1 - all lines has m_linesVisible[0] visibility
    QVector<bool> m_linesVisible;

    // lines permutation (m_indices[relativeLine] = absoluteLine)
    mutable QVector<int> m_relative2absolute;
    // m_visible2absolute[visible line] = absolute line
    mutable QVector<int> m_visible2absolute;
    // m_absolute2visible[absolute line] = { visible line | INVALID_INDEX }
    mutable QVector<int> m_absolute2visible;

    // cache for line sizes
    // m_visibleLinesSizes[line] - start position of the visible line
    // m_visibleLinesSizes[visibleLineCount] - end position of the last visible line
    mutable QVector<int> m_visibleLinesSizes;

    //
    // lines visibility stuff
    //
    QVector<QSharedPointer<LinesVisibility>> m_linesVisibility;
};

// interface for handle line visible state
class QI_EXPORT LinesVisibility: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LinesVisibility)

public:
    virtual ~LinesVisibility() {}

    bool isLineVisible(int line) const { return isLineVisibleImpl(line); }

signals:
    void visibilityChanged(const LinesVisibility* visibility);

protected:
    LinesVisibility() {}

    virtual bool isLineVisibleImpl(int line) const = 0;
};

class QI_EXPORT LinesVisibilityCallback: public LinesVisibility
{
public:
    LinesVisibilityCallback(std::function<bool(int)> callback = nullptr)
        : isLineVisibleCallback(callback)
    {}

    std::function<bool(int)> isLineVisibleCallback;

protected:
    bool isLineVisibleImpl(int line) const override { return isLineVisibleCallback(line); }
};

} // end namespace Qi 

#endif // QI_LINES_H
