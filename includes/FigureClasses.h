#ifndef FIGURECLASSES_H
#define	FIGURECLASSES_H
typedef enum {figSurface, figTorus, figEllipsoid, figParallelepiped} FigureType;

#include "Figure.h"


class Torus : public BaseSurface
{
public:
    virtual ~Torus() {}
    Torus();
    Torus(FILE * F);
    Torus(int cnt_1, int cnt_2, int offset_1 = 0);
    void setPhisicSize(float s1, float s2);
    void createField(int cnt_1, int cnt_2, int offset_1 = 0);
    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);

private:
    int m_offset;
    GLfloat m_angle;
};

class Ellipsoid : public Figure
{
public:
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

private:
    float m_size;
    int m_firstSideCount, m_secondSideCount, m_thirdSideCount;
    IncludingSurface * m_surfaces[6];

    float m_scale;
    float m_firstSize, m_secondSize, m_thirdSize;
    bool m_ellipsoidForm;
};

class Surface : public BaseSurface
{
public:
    virtual ~Surface() {}
    Surface();
    Surface(int cnt_1, int cnt_2);
    Surface(FILE * _file);
    void createField(int cnt_1, int cnt_2);
    void setPhisicSize(float s, float);

    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);
};
#endif	/* FIGURECLASSES_H */

