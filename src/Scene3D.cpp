/* 
 * File:   Scene3D.cpp
 * Author: artyom
 * 
 * Created on 1 Ноябрь 2011 г., 15:57
 */



#include <GL/glu.h>

#include "Scene3D.h"
#include "complect_headers.h"
#include "FigureClasses.h"



Scene3D::StatisticWidget::StatisticWidget(QWidget * parent) : QWidget(parent)
{
    text[STEPS] = tr("Step 0");
    text[LIVING_CELLS] = tr("Users: ");
    font.setWeight(10);
}
void Scene3D::Scene3D::StatisticWidget::resizeEvent(QResizeEvent * /*e*/)
{
    if (height() != (font.weight() + 3) * STATISTIC_ITEMS_COUNT + 10)
        setGeometry(pos().x(), pos().y(), width(), (font.weight() + 3) * STATISTIC_ITEMS_COUNT + 10);
}
void Scene3D::StatisticWidget::paintEvent(QPaintEvent * /*e*/)
{
    painter.begin(this);
    painter.setFont(font);
    painter.fillRect(0, 0, width(), height(), QColor(0, 255, 0));
    for (int i = 0; i < STATISTIC_ITEMS_COUNT; i++)
        painter.drawText(5, (font.weight() + 3) * (i + 1), text[i]);
    painter.end();
}
void Scene3D::StatisticWidget::setText(const QString & _text, int stringNumber, bool refresh)
{
    if (stringNumber < 0 || stringNumber >= STATISTIC_ITEMS_COUNT)
        return;
    text[stringNumber] = _text;
    if (refresh)
        this->repaint();
}


void Scene3D::stepFigure()
{
    if (figure == NULL)
        return;
    figure->step();

    if (statistVisible)
        drawStatistic();
    if (AnimationOn)
        this->updateGL();

    stp++;
}

Scene3D::Scene3D(QWidget* parent) : QGLWidget(parent)
{
    stp = 0;
    textWidget = new StatisticWidget(this);
    textWidget->setGeometry(0, 0, 100, 20);
    Executing = false;
    AnimationOn = true;
    AxesVisible = true;
    gridEnable = false;
    statistVisible = true;
    disposing = false;
    xRot = 0; yRot = 0; zRot = 0; zTra = 0; nSca = 1;
    setDrawingEnable(false);
    srand(time(0));
    figure = NULL;
    tm = new QTimer(this);
    tm->stop();
    connect(tm, SIGNAL(timeout()), this, SLOT(stepFigure()));
    currentModel = NULL;
}

Scene3D::~Scene3D()
{
    disposing = true;
    delete tm;
    delete textWidget;
    delete figure;
}

void Scene3D::createAgar()
{
    figure->createAgar();
    updateGL();
}
void Scene3D::clearMap()
{
    figure->clearMap();
    updateGL();
}
void Scene3D::createRandomMap()
{
    figure->createRandomMap(0.15);
    updateGL();
}

void Scene3D::setGridEnable(int on)
{
    figure->gridEnable = gridEnable = on != 0;
    this->updateGL();
}
void Scene3D::setDrawingEnable(int on)
{
    DrawingOn = on != 0;
    QCursor C;
    C.setShape(DrawingOn
               ? Qt::PointingHandCursor
               : Qt::OpenHandCursor);
    this->setCursor(C);
}

void Scene3D::setAnimationEnable(int on)
{
    AnimationOn = on != 0;
    this->updateGL();
}
void Scene3D::setStatisticVisible(int on)
{
    textWidget->setVisible(statistVisible = (on != 0));
}
void Scene3D::setAxesVisible(int on)
{
    AxesVisible = on != 0;
    updateGL();
}

void Scene3D::drawStatistic()
{
    if (figure != NULL)
    {
        textWidget->setText("Step " + QString::number(stp), StatisticWidget::STEPS, false);
        textWidget->setText("Users: "+ QString::number(figure->getLivingCellsCount()), StatisticWidget::LIVING_CELLS);
    }
}


void Scene3D::createFigure(FigureType typeFigure, int * sizeParams, float * /*phizParams*/, bool copy_settings)
{
    double p_live[9], p_dead[9];
    bool grid_on = false;

    if (figure != NULL)
    {
        if (copy_settings)
        {
            figure->getProbabilities(p_live, p_dead);
            grid_on = figure->gridEnable;
        }
        delete figure;
        figure = NULL;
    }
    else
    {
        copy_settings = false;
    }

    switch (typeFigure)
    {
        case figSurface:
            figure = new Surface(sizeParams[0], sizeParams[1]);
            break;
        case figTorus:
            figure = new Torus(sizeParams[0], sizeParams[1], sizeParams[2]);
            break;
        case figEllipsoid:
            figure = new Ellipsoid(sizeParams[0], sizeParams[1], sizeParams[2]);
            break;
        case figParallelepiped:
            figure = new Ellipsoid(sizeParams[0], sizeParams[1], sizeParams[2], false);
            break;
    }

    stp = 0;
    if (copy_settings)
    {
        figure->gridEnable = grid_on;
        figure->setProbabilities(p_live, p_dead);
    }

    updateGL();

}

void Scene3D::setFigure(Figure * _figure)
{
    delete figure;
    figure = _figure;
    updateGL();
}

Figure * Scene3D::getFigure()
{
    return figure;
}

void Scene3D::hideEvent(QHideEvent * /*E*/)
{
    if (prnt == NULL)
        return;
    this->setParent(prnt);
    prnt->resize(prnt->width() + 1, prnt->height());
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
    prnt = (QWidget*)this->parent();
    this->setParent(0);
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
    figure = new Torus(50,100,20);

  //  modelShip * ship = new modelShip();
  //  modelPlaner * p = new modelPlaner();

   // figure->refresh();

    drawStatistic();
}

bool Scene3D::isExecute()
{
    return Executing;
}

void Scene3D::start(int interval)
{
    tm->start(interval);
    Executing = true;
}
void Scene3D::stop()
{
    tm->stop();
    Executing = false;
}
void Scene3D::setInterval(int interval)
{
    tm->setInterval(interval);
}

void Scene3D::changeDrawModel(const QString& name)
{
    currentModel = name == strPen
            ? NULL
            :currentModelCollection[name.toLocal8Bit().data()];
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

void Scene3D::getCoord(int mouse_x, int mouse_y, fpoint * point_1, fpoint * point1)
{
    GLint    viewport[4];    // параметры viewport-a.
    GLdouble projection[16]; // матрица проекции.
    GLdouble modelview[16];  // видовая матрица.
    GLdouble vx,vy,vz;       // координаты курсора мыши в системе координат viewport-a.
    GLdouble wx,wy,wz;

    glGetIntegerv(GL_VIEWPORT,viewport);           // узнаём параметры viewport-a.
    glGetDoublev(GL_PROJECTION_MATRIX,projection); // узнаём матрицу проекции.
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // узнаём видовую матрицу.
    // переводим оконные координаты курсора в систему координат viewport-a.
    vx = mouse_x;
    vy = height() - mouse_y - 1; // где height - текущая высота окна.


    // вычисляем ближний конец селектирующего отрезка.
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
    // вычисляем дальний конец селектирующего отрезка.
   // vz = 1;
   /// gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
   // p2 = CVector3(wx,wy,wz);
}

void Scene3D::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if (!AnimationOn || figure == NULL) return;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   float pos[4] = {5,0,5,0};
   glLightfv(GL_LIGHT0,GL_POSITION,pos);

   glScalef(nSca, nSca, nSca);
   glTranslatef(0.0f, 0, 0.0f);

   glRotatef(xRot, 1.0f, 0.0f, 0.0f);
 //  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
   glRotatef(zRot, 0.0f, 0.0f, 1.0f);
   if (AxesVisible) drawAxis();
   figure->drawCells();
}


Qt::MouseButton mpress;
void Scene3D::mousePressEvent(QMouseEvent* pe)
{
   ptrMousePosition = pe->pos();
   mpress = pe->button();
   this->setFocus();
   if (DrawingOn)
   {
       fpoint p1,p_1;
       getCoord(pe->pos().x(), pe->pos().y(), &p1, &p_1);
       figure->selectAndPlus(p1, p_1, mpress == Qt::LeftButton, currentModel);
       figure->refresh();
   }
   updateGL();
}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{

    if (!DrawingOn)
    {
        xRot += 180 / nSca * (GLfloat)(pe->y()-ptrMousePosition.y()) / height();
        zRot += 180 / nSca * (GLfloat)(pe->x()-ptrMousePosition.x()) / width();
        ptrMousePosition = pe->pos();
    }
    else if (currentModel == NULL)
    {
        fpoint p1, p_1;
        getCoord(pe->pos().x(), pe->pos().y(), &p1, &p_1);
        figure->selectAndPlus(p1, p_1, mpress == Qt::LeftButton);
        figure->refresh();
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
   nSca = nSca * 1.1;
}

void Scene3D::scaleMinus()
{
   nSca = nSca / 1.1;
}

void Scene3D::rotateUp()
{
   xRot += 1.0;
}

void Scene3D::rotateDown()
{
   xRot -= 1.0;
}

void Scene3D::rotateLeft()
{
   zRot += 1.0;
}

void Scene3D::rotateRight()
{
   zRot -= 1.0;
}

void Scene3D::translateDown()
{
   zTra -= 0.05;
}

void Scene3D::translateUp()
{
   zTra += 0.05;
}

void Scene3D::defaultScene()
{
   xRot = 0; yRot = 0; zRot = 0; zTra = 0; nSca = 1;
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



