#include <QMouseEvent>

#include "otherGuiClasses.h"


MyGroupBox::MyGroupBox(QWidget * parent) : QWidget(parent)
{ }
void MyGroupBox::setText(const QString &_text)
{
    text = _text;
    this->repaint();
}
void MyGroupBox::paintEvent(QPaintEvent * /* e */)
{
    int mrgn = 7, m2 = 14;
    painter.begin(this);
    QPen P;
    P.setWidth(2);
    painter.setPen(P);
    painter.drawRect(5, mrgn, width()-10, height()-5-mrgn);
    P.setWidth(1);
    painter.setPen(P);
    QPainterPath path1,path2;
    QBrush B(Qt::white);
    path1.addEllipse(m2,0,m2,m2);
    path1.addEllipse(this->width()-m2*2,0,m2,m2);
    painter.fillPath(path1,B);
    painter.drawEllipse(m2,0,m2,m2);
    painter.drawEllipse(this->width()-m2*2,0,m2,m2);

    path2.addRect(mrgn*3,0,this->width()-mrgn*6,m2);
    painter.fillPath(path2,B);
    painter.drawLine(mrgn*3,0,this->width()-mrgn*3,0);
    painter.drawLine(mrgn*3,m2,this->width()-mrgn*3,m2);
    painter.drawText(mrgn*3,m2-1,text);
    painter.end();
}


////----------------------
/// GrawGrid class


bool * GrawGrid::getPoint(int x, int y)
{
//     int x0, x1 = x_arr , x2
    return m_cells + (y * m_size / height()) * m_size + x * m_size / width();
}
void GrawGrid::createArray(int w, int h)
{
    w_cell = w / m_size;
    h_cell = h / m_size;
    for (int i = 0; i <= m_size; i++)
    {
        x_arr[i] = i * (w-1) / m_size;
        y_arr[i] = i * (h-1) / m_size;
    }
}
void GrawGrid::createField(int sz)
{
    if (sz < 1 || sz == m_size)
        return;
    Model::createField(sz);
    delete x_arr;
    x_arr = new int[sz * 2 + 2];
    y_arr = x_arr + sz + 1;
    createArray(width(),height());
}

void GrawGrid::paintEvent(QPaintEvent * /* e */)
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

void GrawGrid::mousePressEvent(QMouseEvent *e)
{
    *getPoint(e->x(),e->y()) = e->button() == Qt::LeftButton;
    repaint();
}

GrawGrid& GrawGrid::operator = (const Model &model)
{
    Model::operator =(model);
    repaint();
    return *this;
}

GrawGrid::GrawGrid(QWidget * parent) : QWidget(parent), Model(0)
{
    x_arr = NULL;
    m_cells = NULL;
    createField(5);
}
GrawGrid::~GrawGrid()
{
    delete x_arr;
}
void GrawGrid::setQuadSize(int sz, bool * field)
{
    createField(sz);
    for (int i = 0; i<m_square; i++)
        m_cells[i] = field[i];
    repaint();
}

void GrawGrid::setQuadSize(int sz)
{
    createField(sz);
    repaint();
}


///------------------------------------------------------
// MyFileDialogClass

MyFileDialog::MyFileDialog() :
    QFileDialog()
{
    filters += tr("Figure files (*.cf)");
    filters += tr("All files (*)");
    setNameFilters(filters);
    connect(this,SIGNAL(filterSelected(const QString&) ),this,SLOT(filterChanged(const QString&) ) );
    setDefaultSuffix(tr("cf"));
}
QString MyFileDialog::execSave()
{
    this->setAcceptMode(QFileDialog::AcceptSave);
    QString ret;
    if (QFileDialog::exec() != QDialog::Accepted)
        return ret;
    return selectedFiles()[0];
}

void MyFileDialog::filterChanged(const QString& filter)
{
    if (filter == filters[0])
        this->setDefaultSuffix(tr("cf"));
    else
        this->setDefaultSuffix("");
}
