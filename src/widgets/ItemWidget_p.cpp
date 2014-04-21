#include "ItemWidget_p.h"
#include <QEvent>
#include <QPainter>

namespace Qi
{

ItemWidgetPrivate::ItemWidgetPrivate(QWidget* owner)
    : m_owner(owner),
      m_cache(CellID(0, 0)),
      m_isCacheValid(false)
{
    Q_ASSERT(m_owner);
}

ItemWidgetPrivate::~ItemWidgetPrivate()
{
}

void ItemWidgetPrivate::addViewSchema(QSharedPointer<View> view, QSharedPointer<Layout> layout)
{
    ViewSchema schema;
    schema.view = view;
    schema.layout = layout;
    m_views.push_back(schema);
    
    m_isCacheValid = false;
    m_sizeHint = QSize();

    m_owner->update();
    m_owner->updateGeometry();
}

void ItemWidgetPrivate::setCell(const CellID& cell)
{
    m_cache.setCell(cell);
    m_isCacheValid = false;
    m_sizeHint = QSize();

    m_owner->update();
    m_owner->updateGeometry();
}

QSize ItemWidgetPrivate::doSizeHint() const
{
    if (!m_sizeHint.isValid())
    {
        validateCacheCell();
        m_sizeHint = m_cache.sizeHint(m_owner);
    }

    return m_sizeHint;
}

void ItemWidgetPrivate::doPaintEvent(QPaintEvent* event)
{
    validateCacheCell();
    
    QPainter painter(m_owner);
    painter.setRenderHint(QPainter::Antialiasing);
    
    m_cache.draw(&painter, m_owner);
}

void ItemWidgetPrivate::doResizeEvent(QResizeEvent * event)
{
    m_isCacheValid = false;
}

void ItemWidgetPrivate::validateCacheCell() const
{
    if (!m_isCacheValid)
    {
        m_cache.reinit(m_views, m_owner->rect());
        m_isCacheValid = true;
    }
}

} // end namespace Qi
