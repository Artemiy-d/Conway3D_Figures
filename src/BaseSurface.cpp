#include "Figure.h"

BaseSurface::BaseSurface() : Figure()
{
    m_firstSideCount = 0;
    r = 0.2;
    R = 1;
}
BaseSurface::BaseSurface(int /*cnt_1*/, int /*cnt_2*/) : Figure()
{
    m_firstSideCount = 0;
    r = 0.2;
    R = 1;
}
void BaseSurface::createIncludingSurface()
{
    if (m_cells == NULL || m_firstSideCount == 0)
        return;
    thisSurface = new IncludingSurface((Figure*)this,m_cells,m_firstSideCount,m_secondSideCount);
}

//virtual void createField(int cnt_1, int cnt_2);
void BaseSurface::createAgar()
{
    thisSurface->createAgar();
}

void BaseSurface::addModel(Model * m, int x, int y, bool refresh)
{
    thisSurface->addModel(m, x, y, refresh);
}
void BaseSurface::addModel(Model * m, Cell * c)
{
    thisSurface->addModel(m, c, true);
}

void BaseSurface::createGridMap()
{
    int c1 = (m_firstSideCount/2)*2, c2 = (m_secondSideCount/2)*2;
    for (int i = 0; i<c1; i++)
        for (int j = 0; j<c2; j++)
        {
            if ((i+j)%3)
                plus(i,j);
        }
    refresh();
}

void BaseSurface::plus(int c1, int c2)
{
    c1 = (c1%m_firstSideCount+m_firstSideCount)%m_firstSideCount;
    c2 = (c2%m_secondSideCount+m_secondSideCount)%m_secondSideCount;
    Figure::plus(c1*m_secondSideCount + c2);
}

void BaseSurface::setPhisicSize(float s1, float s2)
{
    r = s1;
    R = s2;
}

void BaseSurface::minus(int c1, int c2)
{
    c1 = (c1%m_firstSideCount+m_firstSideCount)%m_firstSideCount;
    c2 = (c2%m_secondSideCount+m_secondSideCount)%m_secondSideCount;
    Figure::minus(c1*m_secondSideCount + c2);
}

void BaseSurface::createField(int cnt_1, int cnt_2)
{
    if (cnt_1<1 || cnt_2<1)
        return;
    int c = (m_firstSideCount = cnt_1)*(m_secondSideCount = cnt_2);
    createCells(c,(m_firstSideCount+1)*(m_secondSideCount+1));
    createIncludingSurface();
    int k = 0;

    k=0;
    for (int i = 0; i<cnt_1; i++)
        for (int j = 0; j<cnt_2; j++)
        {
            m_cells[k].neighboursCount = 0;
//            m_cells[k].m_points[0] = &m_points[i*cnt_2+j];
    //        m_cells[k].m_points[3] = &m_points[i*cnt_2+j];
    ///        m_cells[k].m_points[2] = &m_points[i*cnt_2+j];
    //        m_cells[k++].m_points[1] = &m_points[i*cnt_2+j];
        }

    k=0;
    for (int i = 0; i<cnt_1; i++)
        for (int j = 0; j<cnt_2; j++)
        {
#define npp m_cells[k].neighboursCount++
            if (i!=cnt_1-1 && j!=cnt_2-1)
                m_cells[k].neighbors[npp] = &m_cells[(i+1)*cnt_2+j+1];
            if (i!=cnt_1-1)
                m_cells[k].neighbors[npp] = &m_cells[(i+1)*cnt_2+j];
            if (i!=cnt_1-1 && j!=0)
                m_cells[k].neighbors[npp] = &m_cells[(i+1)*cnt_2+j-1];
            if (j!=cnt_2-1)
                m_cells[k].neighbors[npp] = &m_cells[i*cnt_2+j+1];
            if (j!=0)
                m_cells[k].neighbors[npp] = &m_cells[i*cnt_2+j-1];
            if (i!=0 && j!=cnt_2-1)
                m_cells[k].neighbors[npp] = &m_cells[(i-1)*cnt_2+j+1];
            if (i!=0)
                m_cells[k].neighbors[npp] = &m_cells[(i-1)*cnt_2+j];
            if (i!=0 && j!=0)
                m_cells[k].neighbors[npp] = &m_cells[(i-1)*cnt_2+j-1];
            k++;
        }
}
