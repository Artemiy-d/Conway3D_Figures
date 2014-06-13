#include <QTimer>
#include <QMouseEvent>

#include "Scene3D.h"
#include "FigureClasses.h"


Scene3D::StatisticWidget::StatisticWidget(QWidget * _parent)
    : QWidget(_parent)
{
    m_text[STEPS] = QString("Step 0");
    m_text[LIVING_CELLS] = QString("Users: ");
    m_font.setWeight(10);
}
void Scene3D::Scene3D::StatisticWidget::resizeEvent(QResizeEvent * /*_e*/)
{
    if (height() != (m_font.weight() + 3) * STATISTIC_ITEMS_COUNT + 10)
        setGeometry(pos().x(), pos().y(), width(), (m_font.weight() + 3) * STATISTIC_ITEMS_COUNT + 10);
}
void Scene3D::StatisticWidget::paintEvent(QPaintEvent * /*_e*/)
{
    m_painter.begin(this);
    m_painter.setFont(m_font);
    m_painter.fillRect(0, 0, width(), height(), QColor(0, 255, 0));
    for (int i = 0; i < STATISTIC_ITEMS_COUNT; ++i)
        m_painter.drawText(5, (m_font.weight() + 3) * (i + 1), m_text[i]);
    m_painter.end();
}
void Scene3D::StatisticWidget::setText(const QString & _text, int _stringNumber, bool _refresh)
{
    if (_stringNumber < 0 || _stringNumber >= STATISTIC_ITEMS_COUNT)
        return;
    m_text[_stringNumber] = _text;
    if (_refresh)
        this->repaint();
}


void Scene3D::stepFigure()
{
    if (m_figure == NULL)
        return;
    m_figure->step();

    if (m_statisticVisible)
        drawStatistic();
    if (m_animationOn)
        updateGL();

    ++m_stepsNumber;
}

Scene3D::Scene3D(QWidget* _parent)
    : QGLWidget(_parent)
{
    m_stepsNumber = 0;
    m_statisticWidget = new StatisticWidget(this);
    m_statisticWidget->setGeometry(0, 0, 100, 20);
    m_animationOn = true;
    m_axesVisible = true;
    m_statisticVisible = true;
    setDrawingEnable(false);
    m_figure = NULL;
    m_timer = new QTimer(this);
    m_timer->stop();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(stepFigure()));
    m_currentModel = NULL;
    m_savedParent = NULL;
}

Scene3D::~Scene3D()
{
    delete m_timer;
    delete m_statisticWidget;
    delete m_figure;
}

void Scene3D::createAgar()
{
    m_figure->createAgar();
    updateGL();
}
void Scene3D::clearMap()
{
    m_figure->clearMap();
    updateGL();
}
void Scene3D::createRandomMap()
{
    m_figure->createRandomMap(0.15);
    updateGL();
}

void Scene3D::setGridEnable(int _on)
{
    m_figure->m_gridEnable = _on != 0;
    this->updateGL();
}
void Scene3D::setDrawingEnable(int _on)
{
    m_drawingOn = _on != 0;
    QCursor cursor;
    cursor.setShape(m_drawingOn
               ? Qt::PointingHandCursor
               : Qt::OpenHandCursor);
    this->setCursor(cursor);
}

void Scene3D::setAnimationEnable(int _on)
{
    m_animationOn = _on != 0;
    this->updateGL();
}
void Scene3D::setStatisticVisible(int _on)
{
    m_statisticWidget->setVisible(m_statisticVisible = (_on != 0));
}
void Scene3D::setAxesVisible(int _on)
{
    m_axesVisible = _on != 0;
    updateGL();
}

void Scene3D::drawStatistic()
{
    if (m_figure != NULL)
    {
        m_statisticWidget->setText("Step " + QString::number(m_stepsNumber), StatisticWidget::STEPS, false);
        m_statisticWidget->setText("Users: " + QString::number(m_figure->getLivingCellsCount()), StatisticWidget::LIVING_CELLS);
    }
}


void Scene3D::setFigure(Figure * _figure)
{
    delete m_figure;
    m_figure = _figure;
    updateGL();
}

Figure * Scene3D::getFigure()
{
    return m_figure;
}

void Scene3D::switchFullScreen()
{
    if (parent() == NULL)
        restoreNormal();
    else
        setFullScreen();
}

void Scene3D::mouseDoubleClickEvent(QMouseEvent * /*_e*/)
{
    switchFullScreen();
}

void Scene3D::restoreNormal()
{
    if ( m_savedParent )
    {
        setParent(m_savedParent);
        setGeometry( m_savedGeometry );
    }
    showNormal();
}

void Scene3D::setFullScreen()
{
    m_savedGeometry = geometry();
    m_savedParent = (QWidget*)parent();
    setParent(NULL);
    showFullScreen();
}
void Scene3D::initializeGL()
{
    m_view.init();
    m_figure = new Torus(50, 100, 20);

    drawStatistic();
}

bool Scene3D::isExecuted()
{
    return m_timer && m_timer->isActive();
}

void Scene3D::start(int _interval)
{
    m_timer->start(_interval);
}
void Scene3D::stop()
{
    m_timer->stop();
}
void Scene3D::setInterval(int _interval)
{
    m_timer->setInterval(_interval);
}

void Scene3D::resizeGL(int _width, int _height)
{
    m_view.resize( _width, _height );
}

void Scene3D::setCurrentModel(Model * _model)
{
    m_currentModel = _model;
}

void Scene3D::paintGL()
{
   m_view.clear();
   if (!m_animationOn || m_figure == NULL)
       return;

   m_view.prepareDrawing();

   if (m_axesVisible)
       m_view.drawAxes();
   m_view.drawFigure( m_figure );
}

void Scene3D::drawModel(const QPoint & _point)
{
    Point3F pNear, pFar;
    m_view.getNearFarByPoints( _point.x(), height() - _point.y(), &pNear, &pFar );
    m_figure->selectAndPlus(pNear, pFar, m_leftButtonPressed, m_currentModel);
    m_figure->refresh();
}

void Scene3D::mousePressEvent(QMouseEvent* _e)
{
   m_mousePosition = _e->pos();
   m_leftButtonPressed = _e->button() == Qt::LeftButton;
   setFocus();
   if (m_drawingOn)
   {
       drawModel( _e->pos() );
   }
   updateGL();
}

void Scene3D::mouseMoveEvent(QMouseEvent* _e)
{
    if (!m_drawingOn)
    {
        float scaleCoeff = 2.f * M_PI / m_view.getScale();
        m_view.rotate( scaleCoeff * (  _e->x() - m_mousePosition.x() ) / width(),
                       scaleCoeff * ( -_e->y() + m_mousePosition.y() ) / height() );
        m_mousePosition = _e->pos();
    }
    else if (m_currentModel == NULL)
    {
        drawModel( _e->pos() );
    }
   
   updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* _e)
{
   if (_e->delta() > 0)
       m_view.scalePlus();
   else if (_e->delta() < 0)
       m_view.scaleMinus();
   updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* _e)
{
    switch (_e->key())
    {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        m_view.scalePlus();
        break;
    case Qt::Key_Minus:
        m_view.scaleMinus();
        break;
    case Qt::Key_Up:
        m_view.rotateUp();
        break;
    case Qt::Key_Down:
        m_view.rotateDown();
        break;
    case Qt::Key_Left:
        m_view.rotateLeft();
        break;
    case Qt::Key_Right:
        m_view.rotateRight();
        break;
    case Qt::Key_Space:
        m_view.setDefault();
        break;
    case Qt::Key_Escape:
        restoreNormal();
        break;
    }
    QWidget::keyPressEvent(_e);
    updateGL();
}



