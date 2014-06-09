#ifndef FIGURE_H
#define	FIGURE_H

#include <stdio.h>
#include <GL/gl.h>

#include "FileManager.h"
#include "RandomLCG.h"
#include "Point.h"

class Model;

enum CellSides {Side01 = 1, Side12 = 2, Side23 = 4, Side30 = 8 };

static const CellSides ArrayCellSides[] = {Side01, Side12, Side23, Side30};


struct Color3F {GLfloat r,g,b;};
struct Color4B {GLubyte r,g,b,a;};


class Figure
{
public:
    typedef int Index;
    typedef int StepIndex;

    struct Cell
    {
        GLuint indices[4];
        Cell ** neighbors;
        bool livingStatusNow, livingStatusNext;
        Index paintSides;
        Index currentCountActiveNeighbors, nextCountActiveNeighbors;
        StepIndex steps;
        Index neighborsCount;

        void addNeighbor(Cell * _neighbor)
        {
            neighbors[neighborsCount++] = _neighbor;
        }
    };


public:
    Figure();
    virtual ~Figure();
    virtual void setPhisicSize(float _s1, float _s2) = 0;
    virtual void addModel(Model * _model, Index _x, Index _y, bool _refresh = true) = 0;
    virtual void addModel(Model * _model, Cell * _cell) = 0;
    virtual void createAgar() {}
    void setLineWidth(GLfloat _width);
    void drawList();
    void setProbabilities(const double * _pLive, const double * _pDead, Index _neighborsCount);
    void getProbabilities(double * _pLive, double * _pDead) const;
    void createRandomMap(float _p);
    void clearMap();
    void refresh();
    void plus(Cell * _cell);
    void plus(Index _index);
    void minus(Cell * _cell);
    Index getLivingCellsCount() const;
    void minus(Index _index);
    void step();
    void initBegin();
    void drawCells();
    void drawActiveCells();
    void selectAndPlus(const Point3F & _p1, const Point3F & _p_1, bool _plus, Model * _model = NULL);
    void defaultProbabilities();
    virtual void toFile(FileManager::Writer * _writer);
    virtual bool fromFile(FileManager::Reader * _reader);

    virtual const char * getStringType() const = 0;

protected:
    void createCells(Index _cellsCount, Index _pointsCount);
    void createGrid();
    void gridToList();
    void calcAllProbBool();

    void createProbabilities( Index _neighborsCount );

public:
    bool m_gridEnable;

    static const Index s_defaultNeighborsCount;

protected:
    Index m_gridPointsCount;
    Index m_cellsCount, m_pointsCount;

    Point3F * m_points;
    Cell * m_cells;

private:

    bool m_probabilitiesDisabled;
    RandomLCGDefault::Probability * m_probabilitiesLive, * m_probabilitiesDead;
    StepIndex m_stepNumber;
    Index m_maxNeighborsCount;
    Index m_activeCountNow, m_activeCountNext;
    GLuint * m_drawingIndices, * m_gridPoints;
    Index m_usersCount;
    Cell ** m_activeCellsNow, ** m_activeCellsNext;
    GLfloat m_lineWidth;
    GLuint m_listGridId;
    Point3F * m_normalsToCells;
    Point3F * m_pointsToDraw;
    Color4B * m_colorArray, * m_gridColors;

    RandomLCGDefault m_random;

    Color4B m_colorLive, m_colorDead, m_colorGrid;

    static const char * const s_stringType;
};


#endif	/* FIGURE_H */

