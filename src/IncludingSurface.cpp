#include "Figure.h"
#include "Modeles.h"


IncludingSurface::IncludingSurface(Figure * prnt)
{
    parent = prnt;
}

IncludingSurface::IncludingSurface(Figure * prnt, Cell * c, int c1, int c2)
{
    parent = prnt;
    cells = c;
    count_first = c1;
    count_second = c2;
    int s = Side01 | Side30;
    for (int i = 0; i<count_first; i++)
        for (int j = 0; j<count_second; j++) c[i*count_second+j].paintSides = s;
    for (int i = 0; i<count_first; i++)
        c[i*count_second+count_second - 1].paintSides |= Side23;
    for (int j = 0; j<count_second; j++)
        c[(count_first - 1)*count_second+j].paintSides |= Side12;
}

void IncludingSurface::createFullPlaneVertexes(Point3F &pnt0, Point3F &pnt1, Point3F &pnt3,Point3F * pointArray,int &firstIndex)
{
    if (count_first<=0)
        return;
#define get4(x0,x1,x3,x2) (x2).x=(x1).x+(x3).x-(x0).x; (x2).y=(x1).y+(x3).y-(x0).y; (x2).z=(x1).z+(x3).z-(x0).z;
#define divSegment(p1,p2,q,len, px) (px).x=((p1).x*(q)+((len)-(q))*(p2).x)/(len); (px).y=((p1).y*(q)+((len)-(q))*(p2).y)/(len); (px).z=((p1).z*(q)+((len)-(q))*(p2).z)/(len);
    Point3F pnt2,pnt,pnt1_new,pnt3_new,pnt2_new;
    get4(pnt0,pnt1,pnt3,pnt2)
    int index = firstIndex;
    for (int i = 0; i<=count_first; i++)
    {
        divSegment(pnt0,pnt1,i,count_first,pnt1_new)
        for (int j = 0; j<=count_second; j++)
        {
            divSegment(pnt0,pnt3,j,count_second,pnt3_new)
            get4(pnt0,pnt1_new,pnt3_new,pointArray[index])
            index++;
        }
    }

    int k = 0;
    for (int i = 0; i<count_first; i++)
        for (int j = 0; j<count_second; j++)
        {
            cells[k].indexes[0]=i*(count_second+1)+j+firstIndex;
            cells[k].indexes[1]=(i+1)*(count_second+1)+j+firstIndex;
            cells[k].indexes[2]=(i+1)*(count_second+1)+j+1+firstIndex;
            cells[k++].indexes[3]=i*(count_second+1)+j+1+firstIndex;
        }
    firstIndex = index;
    surfaceCellsConnect();
}

Cell * IncludingSurface::getEndCell()
{
    return cells + count_second * count_first;
}

void IncludingSurface::plus(int x, int y)
{
    x = (x%count_first + count_first)%count_first;
    y = (y%count_second + count_second)%count_second;
    parent->plus(&cells[x*count_second+y]);
}
void IncludingSurface::minus(int x, int y)
{
    x = (x%count_first + count_first)%count_first;
    y = (y%count_second + count_second)%count_second;
    parent->minus(&cells[x*count_second]);
}
void IncludingSurface::createAgar()
{
    int c1 = (count_first/3)*3, c2 = (count_second/3)*3;
    for (int i = 0; i<c1; i++)
        for (int j = 0; j<c2; j++)
        {
            if (i%3 && j%3)
                plus(i,j);
            else
                minus(i,j);
        }
    parent->refresh();
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

    parent->refresh();
}

bool IncludingSurface::addModel(Model * _m, Cell * _c, bool refresh)
{
    Cell * c = cells;
    for (int x = 0; x<count_first; x++)
        for (int y = 0; y<count_second; y++)
            if (_c == c++)
            {
                addModel(_m,x,y,refresh);
                return true;
            }
    return false;
}

void IncludingSurface::createVertexesByMain()
{
    if (count_first<=0)
        return;

    for (int i = 0; i<count_first - 1; i++)
        for (int j = 0; j<count_second - 1; j++)
        {
            int k = i*count_second+j;
            cells[k].indexes[1] = cells[(i+1)*count_second+j].indexes[0];
            cells[k].indexes[2] = cells[(i+1)*count_second+j+1].indexes[0];
            cells[k].indexes[3] = cells[i*count_second+j+1].indexes[0];
        }
    for (int i = 0; i<count_first - 1; i++)
    {
        int j = count_second - 1;
        cells[i*count_second+j].indexes[1] = cells[(i+1)*count_second+j].indexes[0];
    }

    for (int j = 0; j<count_second - 1; j++)
    {
        int i = count_first - 1;
        cells[i*count_second+j].indexes[3] = cells[i*count_second+j+1].indexes[0];
    }
}

void IncludingSurface::surfaceCellsConnect()
{
    if (count_first<=0)
        return;

    int k=0;
    for (int i = 0; i<count_first; i++)
        for (int j = 0; j<count_second; j++)
        {
    #define npp cells[k].cnt_neighbors++
            cells[k].cnt_neighbors = 0;
            if (i!=count_first-1 && j!=count_second-1) cells[k].neighbors[npp] = &cells[(i+1)*count_second+j+1];
            if (i!=count_first-1) cells[k].neighbors[npp] = &cells[(i+1)*count_second+j];
            if (i!=count_first-1 && j!=0) cells[k].neighbors[npp] = &cells[(i+1)*count_second+j-1];
            if (j!=count_second-1) cells[k].neighbors[npp] = &cells[i*count_second+j+1];
            if (j!=0) cells[k].neighbors[npp] = &cells[i*count_second+j-1];
            if (i!=0 && j!=count_second-1) cells[k].neighbors[npp] = &cells[(i-1)*count_second+j+1];
            if (i!=0) cells[k].neighbors[npp] = &cells[(i-1)*count_second+j];
            if (i!=0 && j!=0) cells[k].neighbors[npp] = &cells[(i-1)*count_second+j-1];
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
    int cnt = (side == Side01) || (side == Side23) ? surf->count_first : surf->count_second;
    if (cnt<=0)
        return NULL;
    Cell ** ret = new Cell*[cnt];
    offset = (offset % cnt + cnt) % cnt;
    int i = offset, di = invert?-1:1;
    if (invert)
        i=(i+cnt-1)%cnt;

#define ACT(index) for (int j = 0; j<cnt; j++) { ret[j]=&surf->cells[(index)]; i+=di; if (i==-1) i=cnt-1; else if (i==cnt) i=0; }
    switch (side)
    {
        case Side01:
            ACT(i*surf->count_second);
            break;
        case Side12:
            ACT((surf->count_first-1)*surf->count_second+i);
            break;
        case Side23:
            ACT(i*surf->count_second+surf->count_second-1);
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
    int cnt_1 = (side1 == Side01) || (side1 == Side23) ? surf1->count_first : surf1->count_second;
    int cnt_2 = (side2 == Side01) || (side2 == Side23) ? surf2->count_first : surf2->count_second;
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
        arrCell1[i]->indexes[first1] = arrCell2[i]->indexes[second1];
        arrCell1[i]->indexes[first2] = arrCell2[i]->indexes[second2];
    }

    delete arrCell1;
    delete arrCell2;
    return true;
}

bool IncludingSurface::ConnectSurfacesByPoints(IncludingSurface * surf1, CellSides side1, IncludingSurface * surf2, CellSides side2)
{
    int cnt_1 = (side1 == Side01) || (side1 == Side23) ? surf1->count_first : surf1->count_second;
    int cnt_2 = (side2 == Side01) || (side2 == Side23) ? surf2->count_first : surf2->count_second;
    if (cnt_1 != cnt_2)
        return false;

    Cell ** arrCell1 = GetArrayBySide(surf1,side1);
    Cell ** arrCell2 = GetArrayBySide(surf2,side2);

    int k,l,j;
    for (int i = 0; i<cnt_1; i++)
        for (j = 0; j<cnt_1 && arrCell1[i]->cnt_neighbors<8; j++)
        {
            if (arrCell2[j]->cnt_neighbors==8)
                goto m2;
            for (k = 0; k < 4; k++)
                for (l = 0; l < 4; l++)
                {
                    if (arrCell1[i]->indexes[k]==arrCell2[j]->indexes[l])
                        goto m1;
                }
            goto m2;
            m1:
            for (k = 0; k<arrCell1[i]->cnt_neighbors; k++)
            {
                if (arrCell1[i]->neighbors[k]==arrCell2[j])
                    goto m2;
            }
          //  for (k = 0; k<arrCell2[j]->cnt_neighbors; k++) if (arrCell2[j]->neighbors[k]==arrCell1[i]) goto m2;
            arrCell1[i]->neighbors[arrCell1[i]->cnt_neighbors++] = arrCell2[j];
            arrCell2[j]->neighbors[arrCell2[j]->cnt_neighbors++] = arrCell1[i];
            m2: {}
        }

    delete arrCell1;
    delete arrCell2;
 
    return true;
}
