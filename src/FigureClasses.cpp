#include "FigureClasses.h"

#include "IncludingSurface.h"


BaseSurface::BaseSurface() : Figure()
{
    m_firstSideCount = 0;
    m_secondSideCount = 0;
    m_firstSize = 0.2;
    m_secondSize = 1;
}
BaseSurface::BaseSurface(Index /*_firstSideCount*/, Index /*cnt_2*/) : Figure()
{
    m_firstSideCount = 0;
    m_secondSideCount = 0;
    m_firstSize = 0.2;
    m_secondSize = 1;
}
void BaseSurface::createIncludingSurface()
{
    if (m_cells == NULL || m_firstSideCount == 0)
        return;
    m_includingSurface = new IncludingSurface((Figure*)this, m_cells, m_firstSideCount, m_secondSideCount);
}

void BaseSurface::createAgar()
{
    m_includingSurface->createAgar();
}

void BaseSurface::addModel(Model * _model, Index _x, Index _y, bool _refresh)
{
    m_includingSurface->addModel(_model, _x, _y, _refresh);
}
void BaseSurface::addModel(Model * _model, Cell * _cell)
{
    m_includingSurface->addModel(_model, _cell, true);
}

void BaseSurface::createGridMap()
{
    Index c1 = (m_firstSideCount / 2) * 2,
          c2 = (m_secondSideCount / 2) * 2;
    for (Index i = 0; i < c1; ++i)
        for (Index j = 0; j < c2; ++j)
        {
            if ((i + j) % 3)
                plus(i, j);
        }
    refresh();
}

void BaseSurface::plus(Index _c1, Index _c2)
{
    Index c1 = (_c1 % m_firstSideCount + m_firstSideCount) % m_firstSideCount;
    Index c2 = (_c2 % m_secondSideCount + m_secondSideCount) % m_secondSideCount;
    Figure::plus(c1 * m_secondSideCount + c2);
}

void BaseSurface::setPhisicSize(float _s1, float _s2)
{
    m_firstSize = _s1;
    m_secondSize = _s2;
}

void BaseSurface::minus(Index _c1, Index _c2)
{
    Index c1 = (_c1 % m_firstSideCount + m_firstSideCount) % m_firstSideCount;
    Index c2 = (_c2 % m_secondSideCount + m_secondSideCount) % m_secondSideCount;
    Figure::minus(c1 * m_secondSideCount + c2);
}

void BaseSurface::createField(Index _firstSideCount, Index _secondSideCount)
{
    if (_firstSideCount < 1 || _secondSideCount < 1)
        return;
    Index c = (m_firstSideCount = _firstSideCount) * (m_secondSideCount = _secondSideCount);
    createCells(c, (m_firstSideCount + 1) * (m_secondSideCount + 1));
    createIncludingSurface();

    Index k = 0;
    for (Index i = 0; i < _firstSideCount; ++i)
        for (Index j = 0; j < _secondSideCount; ++j)
        {
            m_cells[k].neighborsCount = 0;
//            m_cells[k].m_points[0] = &m_points[i*cnt_2+j];
    //        m_cells[k].m_points[3] = &m_points[i*cnt_2+j];
    //        m_cells[k].m_points[2] = &m_points[i*cnt_2+j];
    //        m_cells[k++].m_points[1] = &m_points[i*cnt_2+j];
        }

    k = 0;
    for (Index i = 0; i < _firstSideCount; ++i)
        for (Index j = 0; j < _secondSideCount; ++j)
        {
            if (i != _firstSideCount - 1 && j != _secondSideCount - 1)
                m_cells[k].addNeighbor( &m_cells[(i + 1) * _secondSideCount + j + 1] );
            if (i != _firstSideCount - 1)
                m_cells[k].addNeighbor( &m_cells[(i + 1) * _secondSideCount + j] );
            if (i != _firstSideCount - 1 && j != 0)
                m_cells[k].addNeighbor( &m_cells[(i + 1) * _secondSideCount + j - 1] );
            if (j != _secondSideCount - 1)
                m_cells[k].addNeighbor( &m_cells[i * _secondSideCount + j + 1] );
            if (j != 0)
                m_cells[k].addNeighbor( &m_cells[i * _secondSideCount + j - 1] );
            if (i != 0 && j != _secondSideCount - 1)
                m_cells[k].addNeighbor( &m_cells[(i - 1) * _secondSideCount + j + 1] );
            if (i != 0)
                m_cells[k].addNeighbor( &m_cells[(i - 1) * _secondSideCount + j] );
            if (i != 0 && j != 0)
                m_cells[k].addNeighbor( &m_cells[(i - 1) * _secondSideCount + j - 1] );
            k++;
        }
}




Surface::Surface() : BaseSurface() { }

Surface::Surface(Index _firstSideCount, Index _secondSideCount)
    : BaseSurface(_firstSideCount, _secondSideCount)
{
    createField(_firstSideCount, _secondSideCount);
}

Surface::Surface(FILE * F)
{
    fromFile(F);
}

void Surface::toFile(FILE * F)
{
    int sz_param = 8, type = (int) figSurface;
    fwrite(&type,4,1,F);
    fwrite(&sz_param,4,1,F);
    fwrite(&m_firstSideCount,4,1,F);
    fwrite(&m_secondSideCount,4,1,F);
    Figure::toFile(F);
}
void Surface::fromFile(FILE * F)
{
    int sz_param, cnt_1, cnt_2;
    fread(&sz_param,4,1,F);
    fread(&cnt_1,4,1,F);
    fread(&cnt_2,4,1,F);
    if (cnt_1 < 1 || cnt_2 < 1)
        return;
    createField(cnt_1, cnt_2);
    Figure::fromFile(F);
}

void Surface::createField(Index _firstSideCount, Index _secondSideCount)
{
    m_firstSideCount = _firstSideCount;
    m_secondSideCount = _secondSideCount;
    createCells(m_firstSideCount * m_secondSideCount, (m_secondSideCount + 1) * (m_firstSideCount + 1));
    createIncludingSurface();
    setPhisicSize(1.0f, 0);
    m_includingSurface->surfaceCellsConnect();

}
void Surface::setPhisicSize(float _s, float)
{
    m_firstSize = m_firstSideCount * _s / (m_firstSideCount + m_secondSideCount);
    m_secondSize = m_secondSideCount * _s / (m_firstSideCount + m_secondSideCount);
    Point3F p0(-m_firstSize, -m_secondSize, 0);
    Point3F p1(m_firstSize, -m_secondSize, 0);
    Point3F p3(-m_firstSize, m_secondSize, 0);
    Index index = 0;
    m_includingSurface->createFullPlaneVertexes(p0, p1, p3, m_points, index);
    createGrid();
}


Torus::Torus() : BaseSurface() {}
Torus::Torus(Index _firstSideCount, Index _secondSideCount, int _offset_1)
    : BaseSurface(_firstSideCount, _secondSideCount)
{
    m_secondSize = 0.5;
    m_firstSize = m_secondSize * _firstSideCount / _secondSideCount;
    if (m_firstSize > 0.9 * m_secondSize)
        m_firstSize = 0.9 * m_secondSize;
    createField(m_firstSideCount = _firstSideCount,
                m_secondSideCount = _secondSideCount,
                _offset_1);
}
Torus::Torus(FILE * F) : BaseSurface()
{
    fromFile(F);
}

void Torus::toFile(FILE * F)
{
    int sz_param = 12, type = (int) figTorus;
    fwrite(&type,4,1,F);
    fwrite(&sz_param,4,1,F);
    fwrite(&m_firstSideCount,4,1,F);
    fwrite(&m_secondSideCount,4,1,F);
    fwrite(&m_offset,4,1,F);
    Figure::toFile(F);
}

void Torus::fromFile(FILE * F)
{
    int sz_param, cnt_1, cnt_2, offs;
    fread(&sz_param,4,1,F);
    fread(&cnt_1,4,1,F);
    fread(&cnt_2,4,1,F);
    fread(&offs,4,1,F);
    if (cnt_1 < 1 || cnt_2 < 1)
        return;
    m_secondSize = 0.5;
    m_firstSize = m_secondSize * cnt_1 / cnt_2;
    if (m_firstSize > 0.9 * m_secondSize)
        m_firstSize = 0.9 * m_secondSize;
    createField(cnt_1, cnt_2, offs);
    Figure::fromFile(F);
}

void Torus::setPhisicSize(float _s1, float _s2)
{
    m_firstSize = _s1;
    m_secondSize = _s2;
    for (Index i = 0; i < m_firstSideCount; ++i)
        for (Index j = 0; j < m_secondSideCount; ++j)
        {
            float b = i * 2 * M_PI / m_firstSideCount + j * m_angle,
                  a = j * 2 * M_PI / m_secondSideCount;
            float cb = m_secondSize + m_firstSize * cos(b);
            Index k = i * m_secondSideCount + j;
            m_points[k].x = -cos(a) * cb;
            m_points[k].y = -sin(a) * cb;
            m_points[k].z = m_firstSize * sin(b);
        }

}
void Torus::createField(Index _firstSideCount, Index _secondSideCount, int _offset_1)
{
    if (_firstSideCount < 1 || _secondSideCount < 1)
        return;
  //  BaseSurface::createField(cnt_1,cnt_2);
    Index c = (m_firstSideCount = _firstSideCount) * (m_secondSideCount = _secondSideCount);
    createCells(c, c);
    createIncludingSurface();
    Index k = 0;
    m_offset = _offset_1;
    m_angle = _offset_1 * 2 * M_PI / c;

    setPhisicSize(m_firstSize, m_secondSize);

    for (Index i = 0; i < _firstSideCount; i++)
    {
        Index i1 = (i + 1) % _firstSideCount, i3 = i, i2 = i1;
        for (Index j = 0; j < _secondSideCount; j++)
        {
            Index j1 = j, j2 = (j + 1) % _secondSideCount, j3 = j2;
            if (j2 == 0)
            {
                i3 = (i + _offset_1) % _firstSideCount;
                i2 = (i3 + 1) % _firstSideCount;
            }
            k = i * _secondSideCount + j;
            m_cells[k].paintSides = Side01 | Side12;

            m_cells[k].indices[0] = i * _secondSideCount + j;

            m_cells[k].indices[1] = i1 * _secondSideCount + j1;
            m_cells[k].indices[2] = i2 * _secondSideCount + j2;
            m_cells[k].indices[3] = i3 * _secondSideCount + j3;
           
        }
    }

  //  this->m_includingSurface->createVertexesByMain();
    IncludingSurface::ConnectPointsOfSurf(m_includingSurface, Side23,
                                          m_includingSurface, Side01,
                                          _offset_1);
    IncludingSurface::ConnectPointsOfSurf(m_includingSurface, Side12,
                                          m_includingSurface, Side30);

    
    m_includingSurface->surfaceCellsConnect();
    IncludingSurface::ConnectSurfacesByPoints(m_includingSurface, Side01,
                                              m_includingSurface, Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_includingSurface, Side30,
                                              m_includingSurface, Side12);

    createGrid();
}


Ellipsoid::Ellipsoid() : Figure()
{
    m_ellipsoidForm = true;
    m_size = 1.0;
}
Ellipsoid::Ellipsoid(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount, bool _ellipsoidFormOn) : Figure()
{
    m_ellipsoidForm = _ellipsoidFormOn;
    m_size = 1.0;
    createField(_firstSideCount, _secondSideCount, _thirdSideCount);
}

Ellipsoid::Ellipsoid(FILE * F) : Figure()
{
    m_size = 1.0;
    fromFile(F);
}

void Ellipsoid::toFile(FILE * F)
{
    int sz_param = 13, type = (int) m_ellipsoidForm?figEllipsoid:figParallelepiped;
    fwrite(&type,4,1,F);
    fwrite(&sz_param,4,1,F);
    fwrite(&m_firstSideCount,4,1,F);
    fwrite(&m_secondSideCount,4,1,F);
    fwrite(&m_thirdSideCount,4,1,F);
    fwrite(&m_ellipsoidForm,1,1,F);
    Figure::toFile(F);
}

void Ellipsoid::fromFile(FILE * F)
{
    int sz_param, cnt_1, cnt_2, cnt_3;
    fread(&sz_param,4,1,F);
    fread(&cnt_1,4,1,F);
    fread(&cnt_2,4,1,F);
    fread(&cnt_3,4,1,F);
    fread(&m_ellipsoidForm,1,1,F);
    if (cnt_1 == 0 || cnt_2 == 0 || cnt_3 == 0)
        return;
    createField(cnt_1, cnt_2, cnt_3);
    Figure::fromFile(F);
}


void Ellipsoid::setPhisicSize(float _s, float)
{
    m_size = _s;
    m_scale = 3 * m_size / (m_firstSideCount + m_secondSideCount + m_thirdSideCount);
    Index c =  2 * (m_firstSideCount * m_secondSideCount +
                    m_firstSideCount * m_thirdSideCount +
                    m_secondSideCount * m_thirdSideCount);
    Index cp =  2 * ((m_firstSideCount + 1) * (m_secondSideCount + 1) +
                     (m_firstSideCount + 1) * (m_thirdSideCount + 1) +
                     (m_secondSideCount + 1) * (m_thirdSideCount + 1));
    createCells(c, cp);
    m_firstSize = m_firstSideCount * m_scale;
    m_secondSize = m_secondSideCount * m_scale;
    m_thirdSize = m_thirdSideCount * m_scale;
    Point3F pnts[8];
    pnts[0].y = pnts[1].y = pnts[2].y = pnts[3].y = -m_secondSize / 2;
    pnts[4].y = pnts[5].y = pnts[6].y = pnts[7].y = m_secondSize / 2;

    pnts[0].x = pnts[1].x = pnts[4].x = pnts[5].x = -m_firstSize / 2;
    pnts[3].x = pnts[2].x = pnts[7].x = pnts[6].x = m_firstSize / 2;

    pnts[0].z = pnts[3].z = pnts[4].z = pnts[7].z = m_thirdSize / 2;
    pnts[1].z = pnts[2].z = pnts[5].z = pnts[6].z = -m_thirdSize / 2;

    Index index = 0;
    Cell * C = m_cells;
    m_surfaces[0] = new IncludingSurface(this, C, m_thirdSideCount, m_firstSideCount);
    m_surfaces[0]->createFullPlaneVertexes(pnts[0], pnts[1], pnts[3], m_points, index);
    C += m_thirdSideCount * m_firstSideCount;
    m_surfaces[1] = new IncludingSurface(this, C, m_secondSideCount, m_firstSideCount);
    m_surfaces[1]->createFullPlaneVertexes(pnts[1], pnts[5], pnts[2], m_points, index);
    C += m_secondSideCount * m_firstSideCount;
    m_surfaces[2] = new IncludingSurface(this, C, m_thirdSideCount, m_firstSideCount);
    m_surfaces[2]->createFullPlaneVertexes(pnts[4], pnts[5], pnts[7], m_points, index);
    C += m_thirdSideCount * m_firstSideCount;
    m_surfaces[3] = new IncludingSurface(this, C, m_secondSideCount, m_firstSideCount);
    m_surfaces[3]->createFullPlaneVertexes(pnts[0], pnts[4], pnts[3], m_points, index);
    C += m_secondSideCount * m_firstSideCount;
    m_surfaces[4] = new IncludingSurface(this, C, m_secondSideCount, m_thirdSideCount);
    m_surfaces[4]->createFullPlaneVertexes(pnts[3], pnts[7], pnts[2], m_points, index);
    C += m_thirdSideCount * m_secondSideCount;
    m_surfaces[5] = new IncludingSurface(this, C, m_thirdSideCount, m_secondSideCount);
    m_surfaces[5]->createFullPlaneVertexes(pnts[0], pnts[1], pnts[4], m_points, index);
    C += m_thirdSideCount * m_secondSideCount;

    if (m_ellipsoidForm)
    {
        for (Index i = 0; i < m_pointsCount; ++i)
        {
            float coeff = m_size / sqrt(pow(m_points[i].x / m_firstSize, 2) +
                                        pow(m_points[i].y / m_secondSize, 2) +
                                        pow(m_points[i].z / m_thirdSize, 2));
            m_points[i].x *= coeff;
        }
    }

}

void Ellipsoid::setEllipsoidFormEnable(bool _on)
{
    m_ellipsoidForm = _on;
    setPhisicSize(m_size, 0);
}

void Ellipsoid::createAgar()
{
    for (int i = 0; i < 6; ++i)
        m_surfaces[i]->createAgar();
}
void Ellipsoid::addModel(Model * _model, Cell * _cell)
{
    for (int i = 0; i < 6; ++i)
    {
        if (m_surfaces[i]->addModel(_model, _cell))
            return;
    }
}

void Ellipsoid::createField(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount)
{
    if (_firstSideCount <= 0 || _secondSideCount <= 0 || _thirdSideCount <= 0)
        return;
    m_firstSideCount = _firstSideCount;
    m_secondSideCount = _secondSideCount;
    m_thirdSideCount  = _thirdSideCount;
    setPhisicSize(1, 0);


    IncludingSurface::ConnectPointsOfSurf(m_surfaces[0], Side30, m_surfaces[1], Side12);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[1], Side30, m_surfaces[2], Side30);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[2], Side12, m_surfaces[3], Side30);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[3], Side12, m_surfaces[0], Side12);

    IncludingSurface::ConnectPointsOfSurf(m_surfaces[0], Side01, m_surfaces[4], Side12);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[1], Side01, m_surfaces[4], Side01);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[3], Side01, m_surfaces[4], Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5], Side23, m_surfaces[0], Side23);

    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5], Side30, m_surfaces[1], Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5], Side12, m_surfaces[3], Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5], Side01, m_surfaces[2], Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[2], Side01, m_surfaces[4], Side30);


    for (int i = 0; i < 6; ++i)
        m_surfaces[i]->surfaceCellsConnect();

    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[0], Side30, m_surfaces[1], Side12);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[1], Side30, m_surfaces[2], Side30);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[2], Side12, m_surfaces[3], Side30);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[3], Side12, m_surfaces[0], Side12);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[0], Side01, m_surfaces[4], Side12);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[1], Side01, m_surfaces[4], Side01);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[3], Side01, m_surfaces[4], Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5], Side23, m_surfaces[0], Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5], Side30, m_surfaces[1], Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5], Side12, m_surfaces[3], Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5], Side01, m_surfaces[2], Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[2], Side01, m_surfaces[4], Side30);

    createGrid();

}



