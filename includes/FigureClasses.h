#ifndef FIGURECLASSES_H
#define	FIGURECLASSES_H
typedef enum {figSurface, figTorus, figEllipsoid, figParallelepiped} FigureType;

#include "Figure.h"

class IncludingSurface;

class BaseSurface : public Figure
{
public:
    BaseSurface();
    BaseSurface(Index _firstSideCount, Index _secondSideCount);
    void addModel(Model * _model, Index _x, Index _y, bool _refresh = true);
    void addModel(Model * _model, Cell * _cell);
    void createAgar();

    void createGridMap();
    void setPhisicSize(float _s1, float _s2);
    void plus(Index _c1, Index _c2);
    void minus(Index _c1, Index _c2);

protected:
    void createField(Index _firstSideCount, Index _secondSideCount);
    void createIncludingSurface();

protected:
    IncludingSurface * m_includingSurface;
    Index m_firstSideCount, m_secondSideCount;
    float m_firstSize, m_secondSize;
};

class Torus : public BaseSurface
{
public:
    virtual ~Torus() {}
    Torus();
    Torus(FILE * F);
    Torus(Index _firstSideCount, Index _secondSideCount, int _offset_1 = 0);
    void setPhisicSize(float _s1, float _s2);
    void createField(Index _firstSideCount, Index _secondSideCount, int _offset_1 = 0);
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
    Ellipsoid(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount, bool _ellipsoidFormOn = true);
    void createField(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount);
    void setPhisicSize(float _s, float);
    void createAgar();
    void setEllipsoidFormEnable(bool _on);
    void addModel(Model * _model, Cell * _cell);
    virtual void addModel(Model *, Index, Index, bool) { }
    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);

private:
    float m_size;
    Index m_firstSideCount, m_secondSideCount, m_thirdSideCount;
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
    Surface(Index _firstSideCount, Index _secondSideCount);
    Surface(FILE * _file);
    void createField(Index _firstSideCount, Index _secondSideCount);
    void setPhisicSize(float _s, float);

    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);
};
#endif	/* FIGURECLASSES_H */

