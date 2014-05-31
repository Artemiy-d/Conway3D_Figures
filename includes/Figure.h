#ifndef FIGURE_H
#define	FIGURE_H

#include <stdio.h>
#include <GL/gl.h>

#include "RandomLCG.h"
#include "Point.h"

class Model;

enum CellSides {Side01 = 1, Side12 = 2, Side23 = 4, Side30 = 8 };

static const CellSides ArrayCellSides[] = {Side01, Side12, Side23, Side30};


struct Color3F {GLfloat r,g,b;};
struct Color4B {GLubyte r,g,b,a;};
struct Cell
{
    GLuint indices[4];
    Cell ** neighbors;
    bool livingStatusNow, livingStatusNext;
    int paintSides;
    int currentCountActiveNeighbours, nextCountActiveNeighbours;
    int steps;
    int neighboursCount;
};


class Figure
{
public:
    Figure();
    virtual ~Figure();
    virtual void setPhisicSize(float s1, float s2) = 0;
    virtual void addModel(Model * m, int x, int y, bool refresh = true) = 0;
    virtual void addModel(Model * m, Cell * c) = 0;
    virtual void createAgar() {}
    void setLineWidth(GLfloat width);
    void drawList();
    void setProbabilities(double * p_live, double * p_dead);
    void getProbabilities(double * p_live, double * p_dead);
    void createRandomMap(float p);
    void clearMap();
    void refresh();
    void plus(Cell * c);
    void plus(int ind);
    void minus(Cell * c);
    int getLivingCellsCount();
    void minus(int ind);
    void step();
    void initBegin();
    void drawCells();
    void drawActiveCells();
    void selectAndPlus(const Point3F & _p1, const Point3F & _p_1, bool _plus, Model * _m = NULL);
    void defaultProbabilities();
    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);

protected:
    void createCells(int cnt, int cnt_pnt);
    void createGrid();
    void gridToList();
    void calcAllProbBool();

public:
    bool m_gridEnable;

protected:
    int len_grid_points;
    int m_cellsCount, m_pointsCount;
    Point3F * m_points;
    Cell * m_cells;

private:

    bool m_probabilitiesDisabled;
    RandomLCGDefault::Probability m_probabilitiesLive[9], m_probabilitiesDead[9];
    int m_stepNumber;
    int m_maxNeighborsCount;
    int m_activeCountNow, m_activeCountNext;
    GLuint * m_drawingIndices, * m_gridPoints;
    int m_usersCount;
    Cell ** m_activeCellsNow, ** m_activeCellsNext;
    GLfloat m_lineWidth;
    GLuint m_listGridId;
    Point3F * m_normalsToCells;
    Point3F * m_pointsToDraw;
    Color4B * m_colorArray, * m_gridColors;

    RandomLCGDefault m_random;

    Color4B m_colorLive, m_colorDead, m_colorGrid;
};


class IncludingSurface
{
private:

    static void GetBorderPoints(CellSides side, int &p1,int &p2);
    static Cell ** GetArrayBySide(IncludingSurface * surf,
                                  CellSides side,
                                  int offset = 0,
                                  bool invert = false);
public:
    IncludingSurface(Figure * prnt);
    IncludingSurface(Figure * prnt, Cell * c, int c1, int c2);
    void plus(int x, int y);
    void minus(int x, int y);
    void createAgar();
    Cell * getEndCell();
    void addModel(Model * m, int x, int y, bool refresh = true);
    bool addModel(Model * m, Cell * c, bool refresh = true);
    void surfaceCellsConnect();
    void createVertexesByMain();
    void createFullPlaneVertexes(Point3F &pnt0,
                                 Point3F &pnt1,
                                 Point3F &pnt3,
                                 Point3F * pointArray,
                                 int &firstIndex);

    static bool ConnectPointsOfSurf(IncludingSurface * surf1,
                                    CellSides side1,
                                    IncludingSurface * surf2,
                                    CellSides side2,
                                    int offset = 0,
                                    bool invert = false);

    static bool ConnectSurfacesByPoints(IncludingSurface * surf1,
                                        CellSides side1,
                                        IncludingSurface * surf2,
                                        CellSides side2);
private:
    Figure * m_parent;
    Cell * m_cells;
    int m_firstSideCount, m_secondSideCount;
};


class BaseSurface : public Figure
{
public:
    BaseSurface();
    BaseSurface(int cnt_1, int cnt_2);
    void addModel(Model * m, int x, int y, bool refresh = true);
    void addModel(Model * m, Cell * c);
    void createAgar();

    void createGridMap();
    void setPhisicSize(float s1, float s2);
    void plus(int c1, int c2);
    void minus(int c1, int c2);

protected:
    void createField(int cnt_1, int cnt_2);
    void createIncludingSurface();

public:
    int m_firstSideCount, m_secondSideCount;
    IncludingSurface * thisSurface;
protected:
    GLfloat r, R;
};

#endif	/* FIGURE_H */

