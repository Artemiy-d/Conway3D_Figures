#ifndef INCLUDINGSURFACE_H
#define INCLUDINGSURFACE_H

#include "Figure.h"

class Model;

class IncludingSurface
{
public:
    typedef Figure::Cell Cell;
    typedef Figure::Index Index;
public:
    IncludingSurface(Figure * _parent);
    IncludingSurface(Figure * _parent, Cell * _c, Index _c1, Index _c2);
    void plus(Index _x, Index _y);
    void minus(Index _x, Index _y);
    void createAgar();
    Cell * getEndCell() const;
    void addModel(Model * _m, Index _x, Index _y, bool _refresh = true);
    bool addModel(Model * _m, Cell * _c, bool _refresh = true);
    void surfaceCellsConnect();
    void createVertexesByMain();
    void createFullPlaneVertexes(Point3F &_pnt0,
                                 Point3F &_pnt1,
                                 Point3F &_pnt3,
                                 Point3F * _pointArray,
                                 Index &_firstIndex);

    static bool ConnectPointsOfSurf(IncludingSurface * _surf1,
                                    CellSides _side1,
                                    IncludingSurface * _surf2,
                                    CellSides _side2,
                                    int _offset = 0,
                                    bool _invert = false);

    static bool ConnectSurfacesByPoints(IncludingSurface * _surf1,
                                        CellSides _side1,
                                        IncludingSurface * _surf2,
                                        CellSides _side2);
protected:
    Index getSideCellsCount(CellSides _side) const;
private:

    static void GetBorderPoints(CellSides _side, Index &_p1,Index &_p2);
    static Cell ** GetArrayBySide(IncludingSurface * _surf,
                                  CellSides _side,
                                  int _offset = 0,
                                  bool _invert = false);
private:
    Figure * m_parent;
    Cell * m_cells;
    Index m_firstSideCount, m_secondSideCount;
};

#endif // INCLUDINGSURFACE_H
