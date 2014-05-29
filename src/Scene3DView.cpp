#include <GL/glu.h>

#include <math.h>

#include "Scene3DView.h"
#include "Figure.h"

const float Scene3DView::s_defaultAngle = 0.02f;
const float Scene3DView::s_defaultScaleChanging = 1.1f;


Scene3DView::Scene3DView() :
    m_right(1.f, 0.f, 0.f),
    m_up(0.f, 1.f, 0.f),
    m_scale(1.f)
{ }

void Scene3DView::setDefault()
{
    m_right = Point3F(1.f, 0.f, 0.f);
    m_up = Point3F(0.f, 1.f, 0.f);
    m_scale = 1.f;
}

void Scene3DView::init()
{
    glClearColor( 1.f, 1.f, 1.f, 1.f );
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float color[4] = { 0.7f, 0.7f, 1.f, 1.f };
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, color);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

void Scene3DView::resize(int _width, int _height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat ratio=(GLfloat)_height / (GLfloat)_width;

    if (_width >= _height)
        glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -5.0, 2.0);
    else
       glOrtho(-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -5.0, 2.0);

    glViewport(0, 0, (GLint)_width, (GLint)_height);
}

void Scene3DView::rotate( float _angleX, float _angleY )
{
    Point3F right(m_right);
    Point3F up(m_up);

    if ( _angleX != 0.f )
    {
        m_right = right * cos( _angleX ) + (right ^ up) * sin( _angleX );
        m_right.normalize();
    }
    if ( _angleY != 0.f )
    {
        m_up = up * cos( _angleY ) + (up ^ right) * sin( _angleY );
        m_up.normalize();
    }
}

void Scene3DView::getNearFarByPoints( int _x, int _y, Point3F * _pointNear, Point3F * _pointFar )
{
    GLint    viewport[4];
    GLdouble projection[16];
    GLdouble modelview[16];
    GLdouble wx,wy,wz;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

    GLdouble vx = _x;
    GLdouble vy = _y;

    GLdouble vz = -1.;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    _pointNear->x = wx;
    _pointNear->y = wy;
    _pointNear->z = wz;

    vz = 1.;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    _pointFar->x = wx;
    _pointFar->y = wy;
    _pointFar->z = wz;
}

void Scene3DView::rotateUp()
{
    rotate( 0.f, s_defaultAngle );
}

void Scene3DView::rotateDown()
{
    rotate( 0.f, -s_defaultAngle );
}

void Scene3DView::rotateLeft()
{
    rotate( -s_defaultAngle, 0.f );
}

void Scene3DView::rotateRight()
{
    rotate( s_defaultAngle, 0.f );
}

void Scene3DView::changeScale(float _coeffitient)
{
    m_scale *= _coeffitient;
}

void Scene3DView::scalePlus()
{
    m_scale *= s_defaultScaleChanging;
}

void Scene3DView::scaleMinus()
{
    m_scale /= s_defaultScaleChanging;
}

void Scene3DView::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene3DView::drawAxes()
{
    glLineWidth(3.0f);

    glBegin(GL_LINES);
        glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
        glVertex3f( 2.0f,  0.0f,  0.0f);
        glVertex3f(-2.0f,  0.0f,  0.0f);

        glColor4f(0.f, 0.5f, 0.f, 1.f);
        glVertex3f( 0.0f,  2.0f,  0.0f);
        glVertex3f( 0.0f, -2.0f,  0.0f);

        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f( 0.0f,  0.0f,  2.0f);
        glVertex3f( 0.0f,  0.0f, -2.0f);
    glEnd();
}

void Scene3DView::drawFigure(Figure * _figure)
{
    _figure->drawCells();
}

void Scene3DView::prepareDrawing()
{    
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    static float pos[4] = {5.f, 0.f, 5.f, 0.f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    Point3F product( m_up ^ m_right  );
    gluLookAt( product.x, product.y, product.z,
               0.f, 0.f, 0.f,
               m_up.x, m_up.y, m_up.z );

    glScalef(m_scale, m_scale, m_scale);
}
