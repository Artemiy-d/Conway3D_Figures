
#include <math.h>
#include <GL/glu.h>
#include "Camera.h"

const float Camera::s_defaultAngle = 0.02;


Camera::Camera() :
    m_right(1.f, 0.f, 0.f),
    m_up(0.f, 1.f, 0.f)
{

}

void Camera::rotate( float m_angleX, float m_angleY )
{
    Point3F right(m_right);
    Point3F up(m_up);

    if ( m_angleX != 0.f )
    {
        m_right = right * cos( m_angleX ) + (right ^ up) * sin( m_angleX );
        m_right.normalize();
    }
    if ( m_angleY != 0.f )
    {
        m_up = up * cos( m_angleY ) + (up ^ right) * sin( m_angleY );
        m_up.normalize();
    }
}

void Camera::rotateUp()
{
    rotate( 0.f, s_defaultAngle );
}

void Camera::rotateDown()
{
    rotate( 0.f, -s_defaultAngle );
}

void Camera::rotateLeft()
{
    rotate( -s_defaultAngle, 0.f );
}

void Camera::rotateRight()
{
    rotate( s_defaultAngle, 0.f );
}

void Camera::apply()
{
    Point3F product( m_up ^ m_right  );
    gluLookAt( product.x, product.y, product.z, 0, 0, 0, m_up.x, m_up.y, m_up.z );
}
