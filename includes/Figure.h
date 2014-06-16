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


class Figure : public FileManager::ISerializable
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

    class Probabilities
    {
    public:
        typedef RandomLCGDefault::Probability Probability;
    public:
        Probabilities();
        Probabilities(const Probabilities & _other);
        Probabilities(const double * _live, const double * _death, size_t _count);
        Probabilities & operator = (const Probabilities & _other);
        ~Probabilities();

        Probability * getProbabilitiesLive();
        Probability * getProbabilitiesDeath();

        const Probability * getProbabilitiesLive() const;
        const Probability * getProbabilitiesDeath() const;

        size_t getCount();

        void setCount(size_t _count);

        bool isAllTrueFalse() const;

        void toDouble(double * _live, double * _death, size_t _count) const;
    protected:
        size_t getAllCount() const;
    public:
        static const size_t s_defaultCount = 9;
    private:
        size_t m_count;
        RandomLCGDefault::Probability * m_probabilities;

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
    void setProbabilities(const Probabilities & _probabilities = Probabilities());
    const Probabilities & getProbabilities() const;
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

    virtual void toFile(FileManager::Writer * _writer);
    virtual bool fromFile(FileManager::Reader * _reader);

    virtual const char * getStringType() const = 0;

protected:
    void createCells(Index _cellsCount, Index _pointsCount);
    void createGrid();
    void gridToList();
    void calcAllProbBool();

public:
    bool m_gridEnable;

protected:
    Index m_gridPointsCount;
    Index m_cellsCount, m_pointsCount;

    Point3F * m_points;
    Cell * m_cells;

private:

    bool m_probabilitiesDisabled;
    Probabilities m_probabilities;
    StepIndex m_stepNumber;
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

