#include <QPainter>

#include "GroupBoxCustom.h"


GroupBoxCustom::GroupBoxCustom(QWidget * _parent)
    : QWidget(_parent)
{ }

void GroupBoxCustom::setText(const QString &_text)
{
    m_text = _text;
    this->repaint();
}
void GroupBoxCustom::paintEvent(QPaintEvent * /* e */)
{
    QPainter painter(this);
    int mrgn = 7, m2 = 14;
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(5, mrgn, width() - 10, height() - 5 - mrgn);
    pen.setWidth(1);
    painter.setPen(pen);
    QPainterPath path1 ,path2;

    QBrush brush(Qt::white);
    path1.addEllipse(m2, 0, m2, m2);
    path1.addEllipse(this->width() - m2 * 2, 0, m2, m2);
    painter.fillPath(path1, brush);
    painter.drawEllipse(m2, 0, m2, m2);
    painter.drawEllipse(this->width() - m2 * 2, 0, m2, m2);

    path2.addRect(mrgn * 3, 0, this->width() - mrgn * 6, m2);
    painter.fillPath(path2, brush);
    painter.drawLine(mrgn * 3, 0, this->width() - mrgn * 3, 0);
    painter.drawLine(mrgn * 3, m2, this->width() - mrgn * 3, m2);
    painter.drawText(mrgn * 3, m2 - 1, m_text);
}


