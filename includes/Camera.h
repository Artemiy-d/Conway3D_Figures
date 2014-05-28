#ifndef CAMERA_H
#define CAMERA_H

#include "Point.h"

class Camera
{
public:
    Camera();

    void rotate( float m_angleX, float m_angleY );

    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();

    void apply();
private:
    Point3F m_right;
    Point3F m_up;

    static const float s_defaultAngle;
};

#endif // CAMERA_H
