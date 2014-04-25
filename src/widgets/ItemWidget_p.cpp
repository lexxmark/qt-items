#include "ItemWidget_p.h"
#include <QPaintEvent>
#include <QResizeEvent>
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
    clearViewSchemas();
}

void ItemWidgetPrivate::addViewSchema(Layout* layout, View* view, ControllerMouse* controller)
{
    Q_ASSERT(layout);
    Q_ASSERT(view);

    ViewSchema schema;
    schema.layout = layout;
    schema.view = view;
    schema.controller = controller;
    // init ownership
    schema.initOwner(m_owner);

    m_views.append(schema);
    
    m_isCacheValid = false;
    m_sizeHint = QSize();

    // notify owner to repaint and resize
    m_owner->update();
    m_owner->updateGeometry();
}

void ItemWidgetPrivate::clearViewSchemas()
{
    QVector<ViewSchema> views;
    views.swap(m_views);

    m_isCacheValid = false;
    m_cache.clear();
    m_sizeHint = QSize();

    // notify owner to repaint and resize
    m_owner->update();
    m_owner->updateGeometry();

    // delete own objects explicitly
    for (auto& viewSchema: views)
    {
        viewSchema.deleteIfOwnedBy(m_owner);
    }
}

void ItemWidgetPrivate::setCell(const CellID& cell)
{
    m_cache.setCell(cell);
    m_isCacheValid = false;
    m_sizeHint = QSize();

    // notify owner to repaint and resize
    m_owner->update();
    m_owner->updateGeometry();
}

QSize ItemWidgetPrivate::sizeHint() const
{
    if (!m_sizeHint.isValid())
    {
        validateCacheCell();
        m_sizeHint = m_cache.sizeHint(m_owner);
    }

    return m_sizeHint;
}

void ItemWidgetPrivate::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Paint:
        doPaintEvent(static_cast<QPaintEvent*>(event));
        break;

    case QEvent::Resize:
        doResizeEvent(static_cast<QResizeEvent*>(event));

    default:
        ;
    }
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
