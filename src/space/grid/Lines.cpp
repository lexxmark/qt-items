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

#include "Lines.h"

namespace Qi
{

static const int DefaultLineSize = 0;
static const bool DefaultLineVisibility = true;

Lines::Lines(int count)
    : m_count(0)
{
    setCount(count);
}

Lines::Lines(const Lines& lines)
    : QObject(),
      m_count(lines.m_count),
      m_linesSize(lines.m_linesSize),
      m_linesVisible(lines.m_linesVisible),
      m_relative2absolute(lines.m_relative2absolute),
      m_visible2absolute(lines.m_visible2absolute),
      m_absolute2visible(lines.m_absolute2visible),
      m_visibleLinesSizes(lines.m_visibleLinesSizes)
{
}

QSharedPointer<Lines> Lines::clone() const
{
    return QSharedPointer<Lines>(new Lines(*this));
}

void Lines::setCount(int _count)
{
    int count = _count;

    if (m_count == count)
    {
        // fire events for listeners who is interested if even actual count wasn't changed
        emit linesChanged(this, ChangeReasonLinesCountWeak);
        return;
    }

    m_count = count;

    int storeCount = qMax(m_count, 1);

    // resize all lines data
    if (m_linesSize.size() > 1)
    {
        int oldSize = m_linesSize.size();
        m_linesSize.resize(storeCount);
        for (int i = oldSize; i < storeCount; ++i)
            m_linesSize[i] = DefaultLineSize;
    }

    if (m_linesVisible.size() > 1)
    {
        int oldSize = m_linesVisible.size();
        m_linesVisible.resize(storeCount);
        for (int i = oldSize; i < storeCount; ++i)
            m_linesVisible[i] = DefaultLineVisibility;
    }

    // initialize permutation
    m_relative2absolute.resize(count);
    for (int i = 0; i < count; ++i)
        m_relative2absolute[i] = i;

    // invalidate caches
    invalidateVisibles();

    // fire signal
    linesChanged(this, ChangeReasonLinesCount|ChangeReasonLinesCountWeak);
}

static int moveValues(QVector<int>& values, int oldIndex, int newIndex, int count)
{
    Q_ASSERT(oldIndex >= 0 && oldIndex < values.size());
    Q_ASSERT(newIndex >= 0 && newIndex < values.size());
    Q_ASSERT(count > 0);

    int index = InvalidIndex;

    QVector<int> newValues;
    newValues.resize(values.size());

    if (newIndex < oldIndex)
    {
        std::copy(values.begin(),                           values.begin() + newIndex,              newValues.begin());
        std::copy(values.begin() + newIndex,                values.begin() + oldIndex,              newValues.begin() + newIndex + count);
        std::copy(values.begin() + oldIndex,                values.begin() + oldIndex + count,      newValues.begin() + newIndex);
        std::copy(values.begin() + oldIndex + count,        values.end(),                           newValues.begin() + oldIndex + count);
        index = newIndex;
    }
    else // newLine > oldLine + lineCount
    {
        int midSize = qMin(newIndex - oldIndex, values.size() - (oldIndex + count));
        std::copy(values.begin(),                           values.begin() + oldIndex,                      newValues.begin());
        std::copy(values.begin() + oldIndex + count,        values.begin() + oldIndex + count + midSize,    newValues.begin() + oldIndex);
        std::copy(values.begin() + oldIndex,                values.begin() + oldIndex + count,              newValues.begin() + oldIndex + midSize);

        if (midSize < values.size() - (oldIndex + count))
        {
            std::copy(values.begin() + oldIndex + count + midSize, values.end(), newValues.begin() + oldIndex + count + midSize);
        }
        index = oldIndex + midSize;
    }

    values.swap(newValues);

    return index;
}

int Lines::moveLines(int oldAbsoluteLine, int newRelativeLine, int linesCount)
{
    Q_ASSERT(linesCount > 0);
    if (oldAbsoluteLine >= count())
        return InvalidIndex;

    // convert absolute line to relative line
    int oldLine = (int)std::distance(m_relative2absolute.begin(), std::find(m_relative2absolute.begin(), m_relative2absolute.end(), oldAbsoluteLine));
    int newLine = newRelativeLine;

    // cannot perform moving
    if (((oldLine + linesCount) > count()) || (newLine > count()) || (newLine > oldLine && newLine < (oldLine + linesCount)))
        return InvalidIndex;

    int index = moveValues(m_relative2absolute, oldLine, newLine, linesCount);

    invalidateVisibles();

    emit linesChanged(this, ChangeReasonLinesOrder);

    return index;
}

int Lines::moveVisibleLines(int oldLine, int newLine, int linesCount)
{
    Q_ASSERT(linesCount > 0);
    // cannot perform moving
    if (((oldLine + linesCount) > count()) || (newLine > count()) || (newLine > oldLine && newLine < (oldLine + linesCount)))
        return InvalidIndex;

    // convert visible lines to relative lines
    oldLine = (int)std::distance(m_relative2absolute.begin(), std::find(m_relative2absolute.begin(), m_relative2absolute.end(), toAbsolute(oldLine)));
    newLine = (int)std::distance(m_relative2absolute.begin(), std::find(m_relative2absolute.begin(), m_relative2absolute.end(), toAbsolute(newLine)));

    int index = moveValues(m_relative2absolute, oldLine, newLine, linesCount);

    invalidateVisibles();

    emit linesChanged(this, ChangeReasonLinesOrder);

    return index;
}

int Lines::insertVisibleLines(int lineBefore, int linesCount)
{
    bool justAppend = lineBefore >= count();

    // append
    setCount(count() + linesCount);

    if (justAppend)
    {
        return visibleCount() - linesCount;
    }
    else
    {
        return moveVisibleLines(visibleCount() - linesCount, lineBefore, linesCount);
    }
}

int Lines::findVisibleIDByPos(int position, bool noTailLine) const
{
    if (isEmptyVisible())
        return InvalidIndex;

    if (position <= 0)
        return 0;

    if (position > endPos(visibleCount() - 1))
        return noTailLine ? visibleCount() - 1 : visibleCount();

    validateSizes();
    return findVisibleIDByPosImpl(position, 0, visibleCount() - 1);
}

int Lines::findVisibleIDByPos(int position, int fromVisibleLine, int toVisibleLine) const
{
    Q_ASSERT(fromVisibleLine < m_count && toVisibleLine < m_count && fromVisibleLine <= toVisibleLine);

    validateSizes();

    if (position < m_visibleLinesSizes[fromVisibleLine])
        return InvalidIndex;
    else if (position > m_visibleLinesSizes[toVisibleLine + 1])
        return InvalidIndex;
    else
        return findVisibleIDByPosImpl(position, fromVisibleLine, toVisibleLine);
}

int Lines::findVisibleIDByPosImpl(int position, int fromVisibleLine, int toVisibleLine) const
{
    Q_ASSERT(fromVisibleLine < m_count && toVisibleLine < m_count && fromVisibleLine <= toVisibleLine);

    if (fromVisibleLine == toVisibleLine)
    {
        return fromVisibleLine;
    }
    else if ((toVisibleLine - fromVisibleLine) == 1)
    {
        if (position < m_visibleLinesSizes[toVisibleLine])
            return fromVisibleLine;
        else
            return toVisibleLine;
    }

    Q_ASSERT(position >= m_visibleLinesSizes[fromVisibleLine]);

    int middleLine = (fromVisibleLine + toVisibleLine) / 2;
    if (position < m_visibleLinesSizes[middleLine])
    {
        return findVisibleIDByPosImpl(position, fromVisibleLine, middleLine);
    }
    else
    {
        return findVisibleIDByPosImpl(position, middleLine, toVisibleLine);
    }
}

void Lines::validateVisibles() const
{
    if (!m_absolute2visible.empty())
        return;

    m_visible2absolute.clear();
    m_absolute2visible.fill(InvalidIndex, m_relative2absolute.size());
    for (int i = 0, count = m_relative2absolute.size(); i < count; ++i)
    {
        int absoluteLine = m_relative2absolute[i];
        if (isLineVisible(absoluteLine))
        {
            m_visible2absolute.append(absoluteLine);
            m_absolute2visible[absoluteLine] = m_visible2absolute.size() - 1;
        }
    }
}

void Lines::validateSizes() const
{
    if (!m_visibleLinesSizes.empty())
        return;

    validateVisibles();

    int size = 0;
    m_visibleLinesSizes.resize(visibleCount() + 1);

    for (int line = 0; line < m_visibleLinesSizes.size() - 1; ++line)
    {
        m_visibleLinesSizes[line] = size;
        size += lineSize(toAbsolute(line));
    }
    m_visibleLinesSizes.back() = size;
}


void Lines::setLinesVisible(const QVector<int>& lines, bool visible)
{
    for (auto line: lines)
    {
        m_linesVisible[line] = visible;
    }

    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);
}

void Lines::setLinesVisibleExact(const QVector<int>& lines, bool visible)
{
    m_linesVisible.fill(!visible, m_count);

    for (auto line: lines)
    {
        m_linesVisible[line] = visible;
    }

    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);
}

int Lines::lineSize(int line) const
{
    Q_ASSERT(line < m_count);

    if (m_linesSize.empty())
        return DefaultLineSize;
    else if (m_linesSize.size() == 1)
        return m_linesSize.front();
    else
        return m_linesSize[line];
}

void Lines::setLineSize(int line, int size)
{
    Q_ASSERT(line < m_count);
    Q_ASSERT(size >= 0);

    if (m_linesSize.empty())
    {
        m_linesSize.fill(DefaultLineSize, m_count);
    }
    else if (m_linesSize.size() == 1)
    {
        // make explicit copy of front
        int size = m_linesSize.front();
        m_linesSize.fill(size, m_count);
    }

    if (m_linesSize[line] != size)
    {
        m_linesSize[line] = size;
        invalidateSizes();
        emit linesChanged(this, ChangeReasonLinesSize);
    }
}

void Lines::setLineSizeAll(int size)
{
    Q_ASSERT(size >= 0);
    m_linesSize.fill(size, 1);

    invalidateSizes();
    emit linesChanged(this, ChangeReasonLinesSize);
}

bool Lines::isLineVisibleRaw(int line) const
{
    Q_ASSERT(line < m_count);

    if (m_linesVisible.empty())
        return DefaultLineVisibility;
    else if (m_linesVisible.size() == 1)
        return m_linesVisible.front();
    else
        return m_linesVisible[line];
}

bool Lines::isLineVisible(int line) const
{
    if (!isLineVisibleRaw(line))
        return false;

    for (const auto& linesVisibility: m_linesVisibility)
    {
        if (!linesVisibility->isLineVisible(line))
            return false;
    }

    return true;
}

int Lines::isLinesVisibleAll() const
{
    // undefined
    if (isEmpty())
        return -1;

    if (m_linesVisible.empty())
        return DefaultLineVisibility ? 1 : 0;

    if (m_linesVisibility.empty())
    {
        const bool visibility = m_linesVisible.front();
        for (auto it = m_linesVisible.begin()+1; it != m_linesVisible.end(); ++it)
        {
            if ((*it) != visibility)
                return -1;
        }

        return visibility ? 1 : 0;
    }
    else
    {
        const bool visibility = isLineVisible(0);
        for (int line = 1, n = count(); line < n; ++line)
        {
            if (isLineVisible(line) != visibility)
                return -1;
        }

        return visibility ? 1 : 0;
    }
}

void Lines::setLineVisible(int line, bool visible)
{
    Q_ASSERT(line < m_count);

    if (m_linesVisible.empty())
    {
        m_linesVisible.fill(DefaultLineVisibility, m_count);
    }
    else if (m_linesVisible.size() == 1)
    {
        // make explicit copy of front
        bool visibility = m_linesVisible.front();
        m_linesVisible.fill(visibility, m_count);
    }

    if (m_linesVisible[line] != visible)
    {
        m_linesVisible[line] = visible;
        invalidateVisibles();
        emit linesChanged(this, ChangeReasonLinesVisibility);
    }
}

void Lines::setLineVisibleAll(bool visible)
{
    m_linesVisible.fill(visible, 1);

    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);
}

bool Lines::addLinesVisibility(const QSharedPointer<LinesVisibility>& linesVisibility)
{
    if (m_linesVisibility.indexOf(linesVisibility) != -1)
        return false;

    m_linesVisibility.append(linesVisibility);
    connect(linesVisibility.data(), &LinesVisibility::visibilityChanged, this, &Lines::onLinesVisibilityChanged);

    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);

    return true;
}

bool Lines::removeLinesVisibility(const QSharedPointer<LinesVisibility>& linesVisibility)
{
    auto it = std::find(m_linesVisibility.begin(), m_linesVisibility.end(), linesVisibility);
    if (it == m_linesVisibility.end())
        return false;

    m_linesVisibility.erase(it);
    disconnect(linesVisibility.data(), &LinesVisibility::visibilityChanged, this, &Lines::onLinesVisibilityChanged);

    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);

    return true;
}

void Lines::clearLinesVisibility()
{
    for (const auto& linesVisibility: m_linesVisibility)
    {
        disconnect(linesVisibility.data(), &LinesVisibility::visibilityChanged, this, &Lines::onLinesVisibilityChanged);
    }
    m_linesVisibility.clear();

    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);
}

void Lines::onLinesVisibilityChanged(const LinesVisibility*)
{
    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesVisibility);
}

int Lines::visibleCount() const
{
    validateVisibles();
    return m_visible2absolute.size();
}

int Lines::visibleSize() const
{
    validateSizes();
    return m_visibleLinesSizes.back();
}

int Lines::startPos(int visibleLine) const
{
    validateSizes();
    return m_visibleLinesSizes[visibleLine];
}

int Lines::endPos(int visibleLine) const
{
    validateSizes();
    return m_visibleLinesSizes[visibleLine + 1];
}

void Lines::setPermutation(const QVector<int>& permutation)
{
    Q_ASSERT(permutation.size() == count());
    m_relative2absolute = permutation;
    invalidateVisibles();
    emit linesChanged(this, ChangeReasonLinesOrder);
}

} // end namespace Qi
