#ifndef FIGURECLASSES_H
#define	FIGURECLASSES_H
enum FigureType {figSurface, figTorus, figEllipsoid, figParallelepiped};

#include "Figure.h"


class Torus : public BaseSurface
{
private:
    int offset;
    GLfloat angle;
public:
    
    virtual ~Torus() {}
    Torus();
    Torus(FILE * F);
    Torus(int cnt_1, int cnt_2, int offset_1 = 0);
    void setPhisicSize(float s1, float s2);
    void createField(int cnt_1, int cnt_2, int offset_1 = 0);
    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);
};

class Ellipsoid : public Figure
{
private:
    float scale;
    float size_1, size_2, size_3;
    bool ellipseFormOn;
public:
    int count_first, count_second, count_three;
    IncludingSurface * surf[6];
    float Size;
    virtual ~Ellipsoid() {}
    Ellipsoid();
    Ellipsoid(FILE * F);
    Ellipsoid(int cnt_1, int cnt_2, int cnt_3, bool EllipsoidFormOn = true);
    void createField(int cnt_1, int cnt_2, int cnt_3);
    void setPhisicSize(float s, float);
    void createAgar();
    void setEllipsoidFormEnable(bool on);
    void addModel(Model * M, Cell * C);
    virtual void addModel(Model *, int, int, bool) { }
    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);
};

class Surface : public BaseSurface
{
public:
    virtual ~Surface() {}
    Surface();
    Surface(int cnt_1, int cnt_2);
    Surface(FILE * F);
    void createField(int cnt_1, int cnt_2);
    void setPhisicSize(float s, float);

    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);
};
#endif	/* FIGURECLASSES_H */

