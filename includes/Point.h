#ifndef POINT_H
#define POINT_H

#include <math.h>

template <typename Type>
class Point
{
public:
    Type x, y, z;

    Point()
    { }
    Point(Type _x, Type _y, Type _z) :
        x(_x), y(_y), z(_z)
    { }

    friend Point<Type> operator - (const Point<Type> & v)
    {
        return Point(-v.x, -v.y, -v.z);
    }
    Type abs() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    void normalize()
    {
        Type a = abs();
        x /= a;
        y /= a;
        z /= a;
    }

    Point<Type> operator + (const Point<Type> & v) const
    {
        return Point<Type>(x + v.x, y + v.y, z + v.z);
    }
    Point<Type> operator - (const Point<Type> & v) const
    {
        return Point<Type>(x - v.x, y - v.y, z - v.z);
    }
    Type operator & (const Point<Type> & v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Point<Type> operator * (Type v) const
    {
        return Point<Type>(x * v, y * v, z * v);
    }
    Point<Type> operator ^ (const Point<Type>& v) const
    {
        return Point<Type>(y * v.z - z * v.y,
                           z * v.x - x * v.z,
                           x * v.y - y * v.x);
    }
};

typedef Point<float> Point3F;

#endif // POINT_H
