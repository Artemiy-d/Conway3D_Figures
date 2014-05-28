#include <GL/glu.h>

#include <QTimer>
#include <QMouseEvent>
#include <time.h>

#include "Scene3D.h"
#include "FigureClasses.h"


Scene3D::StatisticWidget::StatisticWidget(QWidget * _parent)
    : QWidget(_parent)
{
    m_text[STEPS] = tr("Step 0");
    m_text[LIVING_CELLS] = tr("Users: ");
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
    for (int i = 0; i < STATISTIC_ITEMS_COUNT; i++)
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
        this->updateGL();

    m_stepsNumber++;
}

Scene3D::Scene3D(QWidget* _parent)
    : QGLWidget(_parent)
{
    m_stepsNumber = 0;
    m_statisticWidget = new StatisticWidget(this);
    m_statisticWidget->setGeometry(0, 0, 100, 20);
    m_executed = false;
    m_animationOn = true;
    m_axesVisible = true;
    m_gridEnable = false;
    m_statisticVisible = true;
    m_nSca = 1.f;
    setDrawingEnable(false);
    srand(time(0));
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
    m_figure->m_gridEnable = m_gridEnable = _on != 0;
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
        m_statisticWidget->setText("Users: "+ QString::number(m_figure->getLivingCellsCount()), StatisticWidget::LIVING_CELLS);
    }
}


void Scene3D::createFigure(FigureType _figuretype, int * _sizeParams, float * /*_phizParams*/, bool _copySettings)
{
    double pLive[9], pDead[9];
    bool gridOn = false;

    if (m_figure != NULL)
    {
        if (_copySettings)
        {
            m_figure->getProbabilities(pLive, pDead);
            gridOn = m_figure->m_gridEnable;
        }
        delete m_figure;
        m_figure = NULL;
    }
    else
    {
        _copySettings = false;
    }

    switch (_figuretype)
    {
        case figSurface:
            m_figure = new Surface(_sizeParams[0], _sizeParams[1]);
            break;
        case figTorus:
            m_figure = new Torus(_sizeParams[0], _sizeParams[1], _sizeParams[2]);
            break;
        case figEllipsoid:
            m_figure = new Ellipsoid(_sizeParams[0], _sizeParams[1], _sizeParams[2]);
            break;
        case figParallelepiped:
            m_figure = new Ellipsoid(_sizeParams[0], _sizeParams[1], _sizeParams[2], false);
            break;
    }

    m_stepsNumber = 0;
    if (_copySettings)
    {
        m_figure->m_gridEnable = gridOn;
        m_figure->setProbabilities(pLive, pDead);
    }

    updateGL();

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

void Scene3D::hideEvent(QHideEvent * /*_e*/)
{
    if (m_savedParent == NULL)
        return;
    this->setParent(m_savedParent);
    m_savedParent->resize(m_savedParent->width() + 1, m_savedParent->height());
    this->setVisible(true);
}


void Scene3D::mouseDoubleClickEvent(QMouseEvent * /*_e*/)
{
    if (this->parent() == NULL)
        this->hide();
    else
        setFullScreen();
}
void Scene3D::setFullScreen()
{
    m_savedParent = (QWidget*)this->parent();
    this->setParent(NULL);
    this->showFullScreen();
}
void Scene3D::initializeGL()
{
    qglClearColor(Qt::white);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float color[4] = {0.7,0.7,1,1};
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,color);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    m_figure = new Torus(50,100,20);

    drawStatistic();
}

bool Scene3D::isExecute()
{
    return m_executed;
}

void Scene3D::start(int _interval)
{
    m_timer->start(_interval);
    m_executed = true;
}
void Scene3D::stop()
{
    m_timer->stop();
    m_executed = false;
}
void Scene3D::setInterval(int _interval)
{
    m_timer->setInterval(_interval);
}

void Scene3D::resizeGL(int _nWidth, int _nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio=(GLfloat)_nHeight / (GLfloat)_nWidth;

   if (_nWidth >= _nHeight)
       glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -5.0, 2.0);
   else
      glOrtho(-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -5.0, 2.0);

   glViewport(0, 0, (GLint)_nWidth, (GLint)_nHeight);
}

void Scene3D::setCurrentModel(Model * _model)
{
    m_currentModel = _model;
}

void Scene3D::getCoord(int _mouseX, int _mouseY, Point3F * _point1, Point3F * _point_1)
{
    GLint    viewport[4];    // ��������� viewport-a.
    GLdouble projection[16]; // ������� ��������.
    GLdouble modelview[16];  // ������� �������.
    GLdouble vx,vy,vz;       // ���������� ������� ���� � ������� ��������� viewport-a.
    GLdouble wx,wy,wz;

    glGetIntegerv(GL_VIEWPORT, viewport);           // ������ ��������� viewport-a.
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // ������ ������� ��������.
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // ������ ������� �������.
    // ��������� ������� ���������� ������� � ������� ��������� viewport-a.
    vx = _mouseX;
    vy = height() - _mouseY - 1; // ��� height - ������� ������ ����.


    // ��������� ������� ����� �������������� �������.
 //   glReadPixels(vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);
    vz = -1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    _point1->x = wx;
    _point1->y = wy;
    _point1->z = wz;

    vz = 1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    _point_1->x = wx;
    _point_1->y = wy;
    _point_1->z = wz;
   // ((QWidget*)this->parent())->setWindowTitle(QString::number(point_1->x)+" "+QString::number(point_1->y)+" "+QString::number(point_1->z));
   // p1 = CVector3(wx,wy,wz);
    // ��������� ������� ����� �������������� �������.
   // vz = 1;
   // gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
   // p2 = CVector3(wx,wy,wz);
}

void Scene3D::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if (!m_animationOn || m_figure == NULL)
       return;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   float pos[4] = {5,0,5,0};
   glLightfv(GL_LIGHT0,GL_POSITION,pos);


  // glTranslatef(0.0f, 0, 0.0f);

   m_camera.apply();
   glScalef(m_nSca, m_nSca, m_nSca);

   if (m_axesVisible)
       drawAxis();
   m_figure->drawCells();
}

void Scene3D::mousePressEvent(QMouseEvent* _e)
{
   m_mousePosition = _e->pos();
   m_leftButtonPressed = _e->button() == Qt::LeftButton;
   this->setFocus();
   if (m_drawingOn)
   {
       Point3F p1, p_1;
       getCoord(_e->pos().x(), _e->pos().y(), &p1, &p_1);
       m_figure->selectAndPlus(p1, p_1, m_leftButtonPressed, m_currentModel);
       m_figure->refresh();
   }
   updateGL();
}

void Scene3D::mouseMoveEvent(QMouseEvent* _e)
{
    if (!m_drawingOn)
    {
        m_camera.rotate( 2.f * M_PI * (  _e->x() - m_mousePosition.x() ) / width(),
                         2.f * M_PI * ( -_e->y() + m_mousePosition.y() ) / height() );
        m_mousePosition = _e->pos();
    }
    else if (m_currentModel == NULL)
    {
        Point3F p1, p_1;
        getCoord(_e->pos().x(), _e->pos().y(), &p1, &p_1);

        m_figure->selectAndPlus(p1, p_1, m_leftButtonPressed);
        m_figure->refresh();
    }
   
   updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* _e)
{
   if (_e->delta() > 0)
       scalePlus();
   else if (_e->delta() < 0)
       scaleMinus();
   updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* _e)
{
    switch (_e->key())
    {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        scalePlus();
        break;

    case Qt::Key_Minus:
        scaleMinus();
        break;

    case Qt::Key_Up:
        m_camera.rotateUp();
        break;

    case Qt::Key_Down:
        m_camera.rotateDown();
        break;

    case Qt::Key_Left:
        m_camera.rotateLeft();
        break;

    case Qt::Key_Right:
        m_camera.rotateRight();
        break;

    case Qt::Key_Space:
        defaultScene();
        break;
    }
    QWidget::keyPressEvent(_e);
    updateGL();
}

void Scene3D::scalePlus()
{
    m_nSca *= 1.1;
}

void Scene3D::scaleMinus()
{
    m_nSca /= 1.1;
}

void Scene3D::defaultScene()
{
    m_nSca = 1;
}

void Scene3D::drawAxis()
{
  //auxSwapBuffers();
  
   glLineWidth(3.0f);

   glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
   glBegin(GL_LINES);
      glVertex3f( 2.0f,  0.0f,  0.0f);
      glVertex3f(-2.0f,  0.0f,  0.0f);
   glEnd();

   QColor halfGreen(0, 128, 0, 255);
   qglColor(halfGreen);
   glBegin(GL_LINES);
      glVertex3f( 0.0f,  2.0f,  0.0f);
      glVertex3f( 0.0f, -2.0f,  0.0f);

      glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
      glVertex3f( 0.0f,  0.0f,  2.0f);
      glVertex3f( 0.0f,  0.0f, -2.0f);
   glEnd();
}



