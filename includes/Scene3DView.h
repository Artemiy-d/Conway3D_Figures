#ifndef CAMERA_H
#define CAMERA_H

#include "Point.h"

class Figure;

class Scene3DView
{
public:
    Scene3DView();

    void init();

    void rotate( float _angleX, float _angleY );

    void resize( int _width, int _height );

    void clear();

    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();

    void changeScale( float _coeffitient );

    void scalePlus();
    void scaleMinus();

    void setDefault();

    void getNearFarByPoints( int _x, int _y, Point3F * _pointNear, Point3F * _pointFar );

    void drawAxes();

    void drawFigure(Figure * _figure);

    void prepareDrawing();
private:

    Point3F m_right;
    Point3F m_up;

    float m_scale;

    static const float s_defaultAngle;
    static const float s_defaultScaleChanging;
};

#endif // CAMERA_H
