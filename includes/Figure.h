/* 
 * File:   Figure.h
 * Author: artyom
 *
 * Created on 1 Ноябрь 2011 г., 21:35
 */

#ifndef FIGURE_H
#define	FIGURE_H


#include "Modeles.h"


enum CellSides {Side01 = 1, Side12 = 2, Side23 = 4, Side30 = 8 };

static const CellSides ArrayCellSides[] = {Side01, Side12, Side23, Side30};

//struct fpoint {GLfloat x,y,z;};
struct dpoint {GLdouble x,y,z;};


class fpoint
{
public:
    float x, y, z;
    fpoint()
    { }
    fpoint(float _x, float _y, float _z) :
        x(_x), y(_y), z(_z)
    { }

    friend fpoint operator - (const fpoint& v)
    {
        return fpoint(-v.x,-v.y,-v.z);
    }
    float abs()
    {
        return sqrt(x*x+y*y+z*z);
    }
    fpoint operator + (const fpoint& v)
    {
        return fpoint(x+v.x,y+v.y,z+v.z);
    }
    fpoint operator - (const fpoint& v)
    {
        return fpoint(x-v.x,y-v.y,z-v.z);
    }
    float operator & (const fpoint& v)
    {
        return x*v.x+y*v.y+z*v.z;
    }
    fpoint operator * (float v)
    {
        return fpoint(x*v,y*v,z*v);
    }
    fpoint operator ^ (const fpoint& v)
    {
        return fpoint(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
    }
};

struct fcolor {GLfloat r,g,b;};
struct bcolor {GLubyte r,g,b,a;};
struct Cell
{

    GLuint indexes[4];
    Cell ** neighbors;
    bool livingStatusNow, livingStatusNext;
    int paintSides;
    int cnt_active_neighbors_now, cnt_active_neighbors_next;
    int step_flag;
    int cnt_neighbors;
    int fict_neighbors;
};


class Figure
{
private:

    bool all_prob_live_bool,all_prob_dead_bool,all_prob_bool;
    unsigned int probabilities_live[9],probabilities_dead[9];
    int stepNmb;
    int maxNeighbors;
    int cnt_act_now, cnt_act_next;
    GLuint * drawIndexes, * gridPoints;
    int cnt_users;
    Cell ** ActiveCellNow, ** ActiveCellNext;
    GLfloat line_width;
    GLuint listGrid;
    fpoint * normalsToCells;
    fpoint * points_for_draw;
    bcolor * color_array, * grid_colors;

protected:
    int len_grid_points;
    void createCells(int cnt, int cnt_pnt);
    void createGrid();
    void gridToList();
    void calcAllProbBool();
    
public:

    Cell * cells;
    fpoint * points;
    bool m_gridEnable;
    int cnt_cells, cnt_points;
    bcolor color_live, color_dead, color_grid;
    Figure();
    virtual ~Figure();
    virtual void setPhisicSize(float s1, float s2) = 0;
    virtual void addModel(Model * M, int x, int y, bool refresh = true) = 0;
    virtual void addModel(Model * M, Cell * C) = 0;
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
    void selectAndPlus(fpoint p1, fpoint p_1, bool plus_on, Model * m = NULL);
    void defaultProbabilities();
    virtual void toFile(FILE * F);
    virtual void fromFile(FILE * F);
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
    Cell * cells;
    int count_first, count_second;
    Figure * parent;
    IncludingSurface(Figure * prnt);
    IncludingSurface(Figure * prnt, Cell * c, int c1, int c2);
    void plus(int x, int y);
    void minus(int x, int y);
    void createAgar();
    Cell * getEndCell();
    void addModel(Model * m, int x, int y, bool refresh = true);
    bool addModel(Model * m, Cell * C, bool refresh = true);
    void surfaceCellsConnect();
    void createVertexesByMain();
    void createFullPlaneVertexes(fpoint &pnt0,
                                 fpoint &pnt1,
                                 fpoint &pnt3,
                                 fpoint * pointArray,
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
};


class BaseSurface : public Figure
{
protected:
    GLfloat r,R;
public:
    int count_first, count_second;
    IncludingSurface * thisSurface;
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
};

#endif	/* FIGURE_H */

