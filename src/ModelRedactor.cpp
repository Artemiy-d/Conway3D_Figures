#include <QMouseEvent>
#include <QPainter>

#include "ModelRedactor.h"
#include "Models.h"

ModelRedactor::View::View(const QSize & _visibleSize, const Model * _model) :
    m_visibleSize(_visibleSize),
    m_model(_model),
    m_XPoints(NULL),
    m_YPoints(NULL)
{
    updateBorders();
}

ModelRedactor::View::~View()
{
    delete m_XPoints;
}

void ModelRedactor::View::getModelPointByVisiblePoint(const QPoint & _visiblePoint, int & _x, int & _y) const
{
    int sz = m_model->getSize();
    _x = _visiblePoint.x() * sz / m_visibleSize.width();
    _y = _visiblePoint.y() * sz / m_visibleSize.height();
}

void ModelRedactor::View::draw(QPaintDevice * _device)
{
    int sz = m_model->getSize();
    if (sz <= 0)
        return;

    QPainter painter(_device);

    for (int i = 0; i < sz; ++i)
        for (int j = 0; j < sz; ++j)
        {
            painter.fillRect(m_XPoints[j],
                             m_YPoints[i],
                             m_XPoints[j + 1] - m_XPoints[j],
                             m_YPoints[i + 1] - m_YPoints[i],
                             m_model->isCellFilled( j, i ) ? Qt::black : Qt::white);
        }
    painter.setPen(Qt::blue);
    for (int i = 0; i <= sz; ++i)
    {
        painter.drawLine(m_XPoints[0], m_YPoints[i], m_XPoints[sz], m_YPoints[i]);
        painter.drawLine(m_XPoints[i], m_YPoints[0], m_XPoints[i], m_YPoints[sz]);
    }
    painter.end();
}

void ModelRedactor::View::updateBorders()
{
    int sz = m_model->getSize();
    if (sz <= 0)
        return;
    delete m_XPoints;
    m_XPoints = new int[sz * 2 + 2];
    m_YPoints = m_XPoints + sz + 1;
    for (int i = 0; i <= sz; ++i)
    {
        m_XPoints[i] = i * ( m_visibleSize.width() - 1) / sz;
        m_YPoints[i] = i * ( m_visibleSize.height() - 1) / sz;
    }
}

void ModelRedactor::View::update(const QSize & _visibleSize, const Model * _model)
{
    m_visibleSize = _visibleSize;
    m_model = _model;
    updateBorders();
}

void ModelRedactor::createField(int _sz)
{
    if (_sz < 1 || _sz == m_model->getSize())
        return;
    m_model->createField( _sz );
    m_view->update( size(), m_model );
}

void ModelRedactor::paintEvent(QPaintEvent * /* _e */)
{
    m_view->draw( this );
}

void ModelRedactor::mousePressEvent(QMouseEvent *_e)
{
    int i = 0, j = 0;
    m_view->getModelPointByVisiblePoint( _e->pos(), i, j );
    m_model->setCellFilled( i, j, _e->button() == Qt::LeftButton );
    repaint();
}

void ModelRedactor::resizeEvent(QResizeEvent * /*_e*/ )
{
    m_view->update( size(), m_model );
}

void ModelRedactor::setModel(const Model * _model)
{
    delete m_model;
    m_model = _model->clone();
    m_view->update( size(), m_model );
    repaint();
}

const Model * ModelRedactor::getModel() const
{
    return m_model;
}

ModelRedactor::ModelRedactor(QWidget * _parent)
    : QWidget(_parent),
      m_model(new Model(0)),
      m_view( new View(size(), m_model) )
{
    createField(5);
}

ModelRedactor::~ModelRedactor()
{
    delete m_model;
    delete m_view;
}

void ModelRedactor::setQuadSize(int _sz)
{
    createField(_sz);
    repaint();
}

