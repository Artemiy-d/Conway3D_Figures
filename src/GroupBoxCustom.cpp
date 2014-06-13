#include <QPainter>

#include "GroupBoxCustom.h"


GroupBoxCustom::GroupBoxCustom(QWidget * _parent)
    : QWidget(_parent)
{ }

void GroupBoxCustom::setText(const QString &_text)
{
    m_text = _text;
    repaint();
}
void GroupBoxCustom::paintEvent(QPaintEvent * /* e */)
{
    static const int mrgn = 7;
    static const int m2 = mrgn * 2;
    static const int m3 = mrgn * 3;
    static const int m4 = mrgn * 4;
    static const int m6 = mrgn * 6;

    static const int lineMrgn = 5;

    QPainter painter(this);
    QPen pen;

    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(lineMrgn, mrgn, width() - lineMrgn * 2, height() - lineMrgn - mrgn);

    pen.setWidth(1);
    painter.setPen(pen);

    QPainterPath path1;
    QPainterPath path2;

    QBrush brush(Qt::white);
    path1.addEllipse(m2, 0, m2, m2);
    path1.addEllipse(width() - m4, 0, m2, m2);
    painter.fillPath(path1, brush);
    painter.drawEllipse(m2, 0, m2, m2);
    painter.drawEllipse(width() - m4, 0, m2, m2);

    path2.addRect(m3, 0, width() - m6, m2);
    painter.fillPath(path2, brush);
    painter.drawLine(m3, 0, width() - m3, 0);
    painter.drawLine(m3, m2, width() - m3, m2);
    painter.drawText(m3, m2 - 1, m_text);
}


