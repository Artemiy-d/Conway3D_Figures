#include <GL/glu.h>

#include <QTimer>
#include <QMouseEvent>
#include <time.h>

#include "Scene3D.h"
#include "FigureClasses.h"



Scene3D::StatisticWidget::StatisticWidget(QWidget * parent) : QWidget(parent)
{
    m_text[STEPS] = tr("Step 0");
    m_text[LIVING_CELLS] = tr("Users: ");
    m_font.setWeight(10);
}
void Scene3D::Scene3D::StatisticWidget::resizeEvent(QResizeEvent * /*e*/)
{
    if (height() != (m_font.weight() + 3) * STATISTIC_ITEMS_COUNT + 10)
        setGeometry(pos().x(), pos().y(), width(), (m_font.weight() + 3) * STATISTIC_ITEMS_COUNT + 10);
}
void Scene3D::StatisticWidget::paintEvent(QPaintEvent * /*e*/)
{
    m_painter.begin(this);
    m_painter.setFont(m_font);
    m_painter.fillRect(0, 0, width(), height(), QColor(0, 255, 0));
    for (int i = 0; i < STATISTIC_ITEMS_COUNT; i++)
        m_painter.drawText(5, (m_font.weight() + 3) * (i + 1), m_text[i]);
    m_painter.end();
}
void Scene3D::StatisticWidget::setText(const QString & _text, int stringNumber, bool refresh)
{
    if (stringNumber < 0 || stringNumber >= STATISTIC_ITEMS_COUNT)
        return;
    m_text[stringNumber] = _text;
    if (refresh)
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

Scene3D::Scene3D(QWidget* parent) : QGLWidget(parent)
{
    m_stepsNumber = 0;
    m_statisticWidget = new StatisticWidget(this);
    m_statisticWidget->setGeometry(0, 0, 100, 20);
    m_executed = false;
    m_animationOn = true;
    m_axesVisible = true;
    m_gridEnable = false;
    m_statisticVisible = true;
    m_xRot = 0; m_yRot = 0; m_zRot = 0; m_zTra = 0; m_nSca = 1;
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

void Scene3D::setGridEnable(int on)
{
    m_figure->m_gridEnable = m_gridEnable = on != 0;
    this->updateGL();
}
void Scene3D::setDrawingEnable(int on)
{
    m_drawingOn = on != 0;
    QCursor C;
    C.setShape(m_drawingOn
               ? Qt::PointingHandCursor
               : Qt::OpenHandCursor);
    this->setCursor(C);
}

void Scene3D::setAnimationEnable(int on)
{
    m_animationOn = on != 0;
    this->updateGL();
}
void Scene3D::setStatisticVisible(int on)
{
    m_statisticWidget->setVisible(m_statisticVisible = (on != 0));
}
void Scene3D::setAxesVisible(int on)
{
    m_axesVisible = on != 0;
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


void Scene3D::createFigure(FigureType typeFigure, int * sizeParams, float * /*phizParams*/, bool copy_settings)
{
    double p_live[9], p_dead[9];
    bool grid_on = false;

    if (m_figure != NULL)
    {
        if (copy_settings)
        {
            m_figure->getProbabilities(p_live, p_dead);
            grid_on = m_figure->m_gridEnable;
        }
        delete m_figure;
        m_figure = NULL;
    }
    else
    {
        copy_settings = false;
    }

    switch (typeFigure)
    {
        case figSurface:
            m_figure = new Surface(sizeParams[0], sizeParams[1]);
            break;
        case figTorus:
            m_figure = new Torus(sizeParams[0], sizeParams[1], sizeParams[2]);
            break;
        case figEllipsoid:
            m_figure = new Ellipsoid(sizeParams[0], sizeParams[1], sizeParams[2]);
            break;
        case figParallelepiped:
            m_figure = new Ellipsoid(sizeParams[0], sizeParams[1], sizeParams[2], false);
            break;
    }

    m_stepsNumber = 0;
    if (copy_settings)
    {
        m_figure->m_gridEnable = grid_on;
        m_figure->setProbabilities(p_live, p_dead);
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

void Scene3D::hideEvent(QHideEvent * /*E*/)
{
    if (m_savedParent == NULL)
        return;
    this->setParent(m_savedParent);
    m_savedParent->resize(m_savedParent->width() + 1, m_savedParent->height());
    this->setVisible(true);
}


void Scene3D::mouseDoubleClickEvent(QMouseEvent * /*E*/)
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
   // float sp[4] = {2,2,2,2};
  //  float mat_specular[] = {1,1,1,1};
    

   // int prm[3] = {50,100,0};
 //   this->createFigure(figThor,prm,NULL,true);
    m_figure = new Torus(50,100,20);

  //  ModelShip * ship = new ModelShip();
  //  ModelPlaner * p = new ModelPlaner();

   // m_figure->refresh();

    drawStatistic();
}

bool Scene3D::isExecute()
{
    return m_executed;
}

void Scene3D::start(int interval)
{
    m_timer->start(interval);
    m_executed = true;
}
void Scene3D::stop()
{
    m_timer->stop();
    m_executed = false;
}
void Scene3D::setInterval(int interval)
{
    m_timer->setInterval(interval);
}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio=(GLfloat)nHeight / (GLfloat)nWidth;

   if (nWidth >= nHeight)
      glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -5.0, 2.0);
   else
      glOrtho(-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -5.0, 2.0);

   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void Scene3D::setCurrentModel(Model * _model)
{
    m_currentModel = _model;
}

void Scene3D::getCoord(int mouse_x, int mouse_y, Point3F * point_1, Point3F * point1)
{
    GLint    viewport[4];    // ��������� viewport-a.
    GLdouble projection[16]; // ������� ��������.
    GLdouble modelview[16];  // ������� �������.
    GLdouble vx,vy,vz;       // ���������� ������� ���� � ������� ��������� viewport-a.
    GLdouble wx,wy,wz;

    glGetIntegerv(GL_VIEWPORT,viewport);           // ������ ��������� viewport-a.
    glGetDoublev(GL_PROJECTION_MATRIX,projection); // ������ ������� ��������.
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // ������ ������� �������.
    // ��������� ������� ���������� ������� � ������� ��������� viewport-a.
    vx = mouse_x;
    vy = height() - mouse_y - 1; // ��� height - ������� ������ ����.


    // ��������� ������� ����� �������������� �������.
 //   glReadPixels(vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);
    vz = -1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    point_1->x = wx;
    point_1->y = wy;
    point_1->z = wz;

    vz = 1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    point1->x = wx;
    point1->y = wy;
    point1->z = wz;
   // ((QWidget*)this->parent())->setWindowTitle(QString::number(point_1->x)+" "+QString::number(point_1->y)+" "+QString::number(point_1->z));
   // p1 = CVector3(wx,wy,wz);
    // ��������� ������� ����� �������������� �������.
   // vz = 1;
   /// gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
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

   glScalef(m_nSca, m_nSca, m_nSca);
   glTranslatef(0.0f, 0, 0.0f);

   glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);
 //  glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);
   glRotatef(m_zRot, 0.0f, 0.0f, 1.0f);
   if (m_axesVisible)
       drawAxis();
   m_figure->drawCells();
}

void Scene3D::mousePressEvent(QMouseEvent* pe)
{
   m_mousePosition = pe->pos();
   m_leftButtonPressed = pe->button() == Qt::LeftButton;
   this->setFocus();
   if (m_drawingOn)
   {
       Point3F p1,p_1;
       getCoord(pe->pos().x(), pe->pos().y(), &p1, &p_1);
       m_figure->selectAndPlus(p1, p_1, m_leftButtonPressed, m_currentModel);
       m_figure->refresh();
   }
   updateGL();
}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{

    if (!m_drawingOn)
    {
        m_xRot += 180 / m_nSca * (GLfloat)(pe->y() - m_mousePosition.y()) / height();
        m_zRot += 180 / m_nSca * (GLfloat)(pe->x() - m_mousePosition.x()) / width();
        m_mousePosition = pe->pos();
    }
    else if (m_currentModel == NULL)
    {
        Point3F p1, p_1;
        getCoord(pe->pos().x(), pe->pos().y(), &p1, &p_1);

        m_figure->selectAndPlus(p1, p_1, m_leftButtonPressed);
        m_figure->refresh();
    }
   
   updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* pe)
{
   if (pe->delta() > 0)
       scalePlus();
   else if (pe->delta() < 0)
       scaleMinus();
   updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* pe)
{
   switch (pe->key())
   {
      case Qt::Key_Plus:
         scalePlus();
      break;

      case Qt::Key_Equal:
         scalePlus();
      break;

      case Qt::Key_Minus:
         scaleMinus();
      break;

      case Qt::Key_Up:
         rotateUp();
      break;

      case Qt::Key_Down:
         rotateDown();
      break;

      case Qt::Key_Left:
        rotateLeft();
      break;

      case Qt::Key_Right:
         rotateRight();
      break;

      case Qt::Key_Z:
         translateDown();
      break;

      case Qt::Key_X:
         translateUp();
      break;

      case Qt::Key_Space:
         defaultScene();
      break;

      case Qt::Key_Escape:
         this->close();
      break;
   }
   QWidget::keyPressEvent(pe);
   updateGL();
}

void Scene3D::scalePlus()
{
   m_nSca = m_nSca * 1.1;
}

void Scene3D::scaleMinus()
{
   m_nSca = m_nSca / 1.1;
}

void Scene3D::rotateUp()
{
   m_xRot += 1.0;
}

void Scene3D::rotateDown()
{
   m_xRot -= 1.0;
}

void Scene3D::rotateLeft()
{
   m_zRot += 1.0;
}

void Scene3D::rotateRight()
{
   m_zRot -= 1.0;
}

void Scene3D::translateDown()
{
   m_zTra -= 0.05;
}

void Scene3D::translateUp()
{
   m_zTra += 0.05;
}

void Scene3D::defaultScene()
{
   m_xRot = 0; m_yRot = 0; m_zRot = 0; m_zTra = 0; m_nSca = 1;
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



