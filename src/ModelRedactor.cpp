#include <QMouseEvent>

#include "ModelRedactor.h"

bool * ModelRedactor::getPoint(int x, int y)
{
//     int x0, x1 = x_arr , x2
    return m_cells + (y * m_size / height()) * m_size + x * m_size / width();
}
void ModelRedactor::createArray(int w, int h)
{
    w_cell = w / m_size;
    h_cell = h / m_size;
    for (int i = 0; i <= m_size; i++)
    {
        x_arr[i] = i * (w-1) / m_size;
        y_arr[i] = i * (h-1) / m_size;
    }
}
void ModelRedactor::createField(int sz)
{
    if (sz < 1 || sz == m_size)
        return;
    Model::createField(sz);
    delete x_arr;
    x_arr = new int[sz * 2 + 2];
    y_arr = x_arr + sz + 1;
    createArray(width(),height());
}

void ModelRedactor::paintEvent(QPaintEvent * /* e */)
{
    painter.begin(this);
    int k = 0;
    for (int i = 0; i < m_size; i++)
        for (int j = 0; j < m_size; j++)
        {
            painter.fillRect(x_arr[j],
                             y_arr[i],
                             x_arr[j + 1] - x_arr[j],
                             y_arr[i + 1] - y_arr[i],
                             m_cells[k++] ? Qt::black : Qt::white);
        }
    painter.setPen(Qt::blue);
    for (int i = 0; i<=m_size; i++)
    {
        painter.drawLine(x_arr[0], y_arr[i], x_arr[m_size], y_arr[i]);
        painter.drawLine(x_arr[i], y_arr[0], x_arr[i], y_arr[m_size]);
    }
    painter.end();
}

void ModelRedactor::mousePressEvent(QMouseEvent *e)
{
    *getPoint(e->x(),e->y()) = e->button() == Qt::LeftButton;
    repaint();
}

ModelRedactor& ModelRedactor::operator = (const Model &model)
{
    Model::operator = (model);
    repaint();
    return *this;
}

ModelRedactor::ModelRedactor(QWidget * parent) : QWidget(parent), Model(0)
{
    x_arr = NULL;
    m_cells = NULL;
    createField(5);
}
ModelRedactor::~ModelRedactor()
{
    delete x_arr;
}
void ModelRedactor::setQuadSize(int sz, bool * field)
{
    createField(sz);
    for (int i = 0; i<m_square; i++)
        m_cells[i] = field[i];
    repaint();
}

void ModelRedactor::setQuadSize(int sz)
{
    createField(sz);
    repaint();
}

