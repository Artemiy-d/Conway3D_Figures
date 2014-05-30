#include "Figure.h"
#include "Modeles.h"


IncludingSurface::IncludingSurface(Figure * _parent) :
    m_parent( _parent ),
    m_cells( NULL ),
    m_firstSideCount( 0 ),
    m_secondSideCount( 0 )
{ }

IncludingSurface::IncludingSurface(Figure * _parent, Cell * _cells, int _firstCount, int _secondCount) :
    m_parent( _parent ),
    m_cells( _cells ),
    m_firstSideCount( _firstCount ),
    m_secondSideCount( _secondCount )
{
    int s = Side01 | Side30;
    for (int i = 0; i < m_firstSideCount; i++)
        for (int j = 0; j < m_secondSideCount; j++)
            _cells[i * m_secondSideCount + j].paintSides = s;

    for (int i = 0; i < m_firstSideCount; i++)
        _cells[i * m_secondSideCount + m_secondSideCount - 1].paintSides |= Side23;

    for (int j = 0; j<m_secondSideCount; j++)
        _cells[(m_firstSideCount - 1) * m_secondSideCount + j].paintSides |= Side12;
}

void IncludingSurface::createFullPlaneVertexes(Point3F &pnt0, Point3F &pnt1, Point3F &pnt3,Point3F * pointArray,int &firstIndex)
{
    if (m_firstSideCount <= 0)
        return;
#define get4(x0,x1,x3,x2) (x2).x=(x1).x+(x3).x-(x0).x; (x2).y=(x1).y+(x3).y-(x0).y; (x2).z=(x1).z+(x3).z-(x0).z;
#define divSegment(p1,p2,q,len, px) (px).x=((p1).x*(q)+((len)-(q))*(p2).x)/(len); (px).y=((p1).y*(q)+((len)-(q))*(p2).y)/(len); (px).z=((p1).z*(q)+((len)-(q))*(p2).z)/(len);
    Point3F pnt2,pnt,pnt1_new,pnt3_new,pnt2_new;
    get4(pnt0,pnt1,pnt3,pnt2)
    int index = firstIndex;
    for (int i = 0; i<=m_firstSideCount; i++)
    {
        divSegment(pnt0,pnt1,i,m_firstSideCount,pnt1_new)
        for (int j = 0; j<=m_secondSideCount; j++)
        {
            divSegment(pnt0,pnt3,j,m_secondSideCount,pnt3_new)
            get4(pnt0,pnt1_new,pnt3_new,pointArray[index])
            index++;
        }
    }

    int k = 0;
    for (int i = 0; i<m_firstSideCount; i++)
        for (int j = 0; j<m_secondSideCount; j++)
        {
            m_cells[k].indices[0]=i*(m_secondSideCount+1)+j+firstIndex;
            m_cells[k].indices[1]=(i+1)*(m_secondSideCount+1)+j+firstIndex;
            m_cells[k].indices[2]=(i+1)*(m_secondSideCount+1)+j+1+firstIndex;
            m_cells[k++].indices[3]=i*(m_secondSideCount+1)+j+1+firstIndex;
        }
    firstIndex = index;
    surfaceCellsConnect();
}

Cell * IncludingSurface::getEndCell()
{
    return m_cells + m_secondSideCount * m_firstSideCount;
}

void IncludingSurface::plus(int x, int y)
{
    x = (x%m_firstSideCount + m_firstSideCount)%m_firstSideCount;
    y = (y%m_secondSideCount + m_secondSideCount)%m_secondSideCount;
    m_parent->plus(&m_cells[x*m_secondSideCount+y]);
}
void IncludingSurface::minus(int x, int y)
{
    x = (x%m_firstSideCount + m_firstSideCount)%m_firstSideCount;
    y = (y%m_secondSideCount + m_secondSideCount)%m_secondSideCount;
    m_parent->minus(&m_cells[x*m_secondSideCount]);
}
void IncludingSurface::createAgar()
{
    int c1 = (m_firstSideCount/3)*3, c2 = (m_secondSideCount/3)*3;
    for (int i = 0; i<c1; i++)
        for (int j = 0; j<c2; j++)
        {
            if (i%3 && j%3)
                plus(i,j);
            else
                minus(i,j);
        }
    m_parent->refresh();
}

void IncludingSurface::addModel(Model * _m, int x, int y, bool refresh)
{
    int sz = _m->getSize();
    if (refresh)
    {
        for (int i = 0; i < sz; i++)
            for (int j = 0; j < sz; j++)
                minus(x + i, j + y);
    }

    for (int i = 0; i < sz; i++)
        for (int j = 0; j < sz; j++)
        {
            if (_m->isCellFilled(i, j))
                plus(x + i, j + y);
        }

    m_parent->refresh();
}

bool IncludingSurface::addModel(Model * _m, Cell * _c, bool _refresh)
{
    Cell * c = m_cells;
    for (int x = 0; x < m_firstSideCount; x++)
        for (int y = 0; y < m_secondSideCount; y++)
            if (_c == c++)
            {
                addModel(_m, x, y, _refresh);
                return true;
            }
    return false;
}

void IncludingSurface::createVertexesByMain()
{
    if (m_firstSideCount<=0)
        return;

    for (int i = 0; i<m_firstSideCount - 1; i++)
        for (int j = 0; j<m_secondSideCount - 1; j++)
        {
            int k = i*m_secondSideCount+j;
            m_cells[k].indices[1] = m_cells[(i+1)*m_secondSideCount+j].indices[0];
            m_cells[k].indices[2] = m_cells[(i+1)*m_secondSideCount+j+1].indices[0];
            m_cells[k].indices[3] = m_cells[i*m_secondSideCount+j+1].indices[0];
        }
    for (int i = 0; i<m_firstSideCount - 1; i++)
    {
        int j = m_secondSideCount - 1;
        m_cells[i*m_secondSideCount+j].indices[1] = m_cells[(i+1)*m_secondSideCount+j].indices[0];
    }

    for (int j = 0; j<m_secondSideCount - 1; j++)
    {
        int i = m_firstSideCount - 1;
        m_cells[i*m_secondSideCount+j].indices[3] = m_cells[i*m_secondSideCount+j+1].indices[0];
    }
}

void IncludingSurface::surfaceCellsConnect()
{
    if (m_firstSideCount<=0)
        return;

    int k=0;
    for (int i = 0; i<m_firstSideCount; i++)
        for (int j = 0; j<m_secondSideCount; j++)
        {
    #define npp m_cells[k].neighboursCount++
            m_cells[k].neighboursCount = 0;
            if (i!=m_firstSideCount-1 && j!=m_secondSideCount-1) m_cells[k].neighbors[npp] = &m_cells[(i+1)*m_secondSideCount+j+1];
            if (i!=m_firstSideCount-1) m_cells[k].neighbors[npp] = &m_cells[(i+1)*m_secondSideCount+j];
            if (i!=m_firstSideCount-1 && j!=0) m_cells[k].neighbors[npp] = &m_cells[(i+1)*m_secondSideCount+j-1];
            if (j!=m_secondSideCount-1) m_cells[k].neighbors[npp] = &m_cells[i*m_secondSideCount+j+1];
            if (j!=0) m_cells[k].neighbors[npp] = &m_cells[i*m_secondSideCount+j-1];
            if (i!=0 && j!=m_secondSideCount-1) m_cells[k].neighbors[npp] = &m_cells[(i-1)*m_secondSideCount+j+1];
            if (i!=0) m_cells[k].neighbors[npp] = &m_cells[(i-1)*m_secondSideCount+j];
            if (i!=0 && j!=0) m_cells[k].neighbors[npp] = &m_cells[(i-1)*m_secondSideCount+j-1];
            k++;
        }

}




//static
void IncludingSurface::GetBorderPoints(CellSides side, int &p1,int &p2)
{
    switch (side)
    {
        case Side01:
            p1 = 0; p2 = 1;
            break;
        case Side12:
            p1 = 1; p2 = 2;
            break;
        case Side23:
            p1 = 3; p2 = 2;
            break;
        case Side30:
            p1 = 0; p2 = 3;
            break;
    }
}

//static
Cell ** IncludingSurface::GetArrayBySide(IncludingSurface * surf, CellSides side, int offset, bool invert)
{
    int cnt = (side == Side01) || (side == Side23) ? surf->m_firstSideCount : surf->m_secondSideCount;
    if (cnt<=0)
        return NULL;
    Cell ** ret = new Cell*[cnt];
    offset = (offset % cnt + cnt) % cnt;
    int i = offset, di = invert?-1:1;
    if (invert)
        i=(i+cnt-1)%cnt;

#define ACT(index) for (int j = 0; j<cnt; j++) { ret[j]=&surf->m_cells[(index)]; i+=di; if (i==-1) i=cnt-1; else if (i==cnt) i=0; }
    switch (side)
    {
        case Side01:
            ACT(i*surf->m_secondSideCount);
            break;
        case Side12:
            ACT((surf->m_firstSideCount-1)*surf->m_secondSideCount+i);
            break;
        case Side23:
            ACT(i*surf->m_secondSideCount+surf->m_secondSideCount-1);
            break;
        case Side30:
            ACT(i);
            break;
    }
    return ret;
}

//static
bool IncludingSurface::ConnectPointsOfSurf(IncludingSurface * surf1, CellSides side1, IncludingSurface * surf2, CellSides side2, int offset, bool invert)
{
    int cnt_1 = (side1 == Side01) || (side1 == Side23) ? surf1->m_firstSideCount : surf1->m_secondSideCount;
    int cnt_2 = (side2 == Side01) || (side2 == Side23) ? surf2->m_firstSideCount : surf2->m_secondSideCount;
    if (cnt_1 != cnt_2) //return false;
    {
        return false;
    }

    offset = (offset % cnt_1 + cnt_1) % cnt_1;
//    if (side1 == Side01) side1 = Side23;
//    if (side2 == Side12) side1 = Side30;
    Cell ** arrCell1 = GetArrayBySide(surf1,side1);
    Cell ** arrCell2 = GetArrayBySide(surf2,side2,offset,invert);
    int first1 = 0, first2 = 0,
        second1 = 0, second2 = 0;
    GetBorderPoints(side1,first1,first2);
    GetBorderPoints(side2,second1,second2);

    for (int i = 0; i < cnt_1; i++)
    {
        arrCell1[i]->indices[first1] = arrCell2[i]->indices[second1];
        arrCell1[i]->indices[first2] = arrCell2[i]->indices[second2];
    }

    delete arrCell1;
    delete arrCell2;
    return true;
}

bool IncludingSurface::ConnectSurfacesByPoints(IncludingSurface * surf1, CellSides side1, IncludingSurface * surf2, CellSides side2)
{
    int cnt_1 = (side1 == Side01) || (side1 == Side23) ? surf1->m_firstSideCount : surf1->m_secondSideCount;
    int cnt_2 = (side2 == Side01) || (side2 == Side23) ? surf2->m_firstSideCount : surf2->m_secondSideCount;
    if (cnt_1 != cnt_2)
        return false;

    Cell ** arrCell1 = GetArrayBySide(surf1,side1);
    Cell ** arrCell2 = GetArrayBySide(surf2,side2);

    int k,l,j;
    for (int i = 0; i<cnt_1; i++)
        for (j = 0; j<cnt_1 && arrCell1[i]->neighboursCount<8; j++)
        {
            if (arrCell2[j]->neighboursCount==8)
                goto m2;
            for (k = 0; k < 4; k++)
                for (l = 0; l < 4; l++)
                {
                    if (arrCell1[i]->indices[k]==arrCell2[j]->indices[l])
                        goto m1;
                }
            goto m2;
            m1:
            for (k = 0; k<arrCell1[i]->neighboursCount; k++)
            {
                if (arrCell1[i]->neighbors[k]==arrCell2[j])
                    goto m2;
            }
          //  for (k = 0; k<arrCell2[j]->neighboursCount; k++) if (arrCell2[j]->neighbors[k]==arrCell1[i]) goto m2;
            arrCell1[i]->neighbors[arrCell1[i]->neighboursCount++] = arrCell2[j];
            arrCell2[j]->neighbors[arrCell2[j]->neighboursCount++] = arrCell1[i];
            m2: {}
        }

    delete arrCell1;
    delete arrCell2;
 
    return true;
}
