#include "ItemWidget.h"
#include <QPainter>

namespace Qi
{

ItemWidget::ItemWidget(QWidget* parent)
    : QWidget(parent),
      m_cache(CellID(0, 0)),
      m_isCacheValid(false)
{
}

ItemWidget::~ItemWidget()
{
}

void ItemWidget::addView(QSharedPointer<View> view, QSharedPointer<Layout> layout)
{
    ViewInfo info;
    info.view = view;
    info.layout = layout;
    m_views.append(info);
    
    m_isCacheValid = false;
    m_sizeHint = QSize();
}

QSize ItemWidget::sizeHint() const
{
    if (!m_sizeHint.isValid())
    {
        QPainter painter(const_cast<ItemWidget*>(this));
        //painter.setRenderHint(QPainter::Antialiasing);
        
        DrawContext dc;
        dc.painter = &painter;
        dc.style = style();
        dc.widget = this;
        
        m_sizeHint = m_cache.sizeHint(dc);
    }
    return m_sizeHint;
}

QSize ItemWidget::minimumSizeHint() const
{
    return ItemWidget::sizeHint();
}

void ItemWidget::paintEvent(QPaintEvent* event)
{
    validateCacheCell();
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    DrawContext dc;
    dc.painter = &painter;
    dc.style = style();
    dc.widget = this;
    
    m_cache.draw(dc);
}

void ItemWidget::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    m_isCacheValid = false;
}

void ItemWidget::validateCacheCell()
{
    if (!m_isCacheValid)
    {
        m_cache.reinit(m_views, rect());
        m_isCacheValid = true;
    }
}


} // end namespace Qi
