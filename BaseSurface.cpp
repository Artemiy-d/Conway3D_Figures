
#include "Figure.h"

BaseSurface::BaseSurface() : Figure()
{
    count_first = 0;
    r = 0.2;
    R = 1;
}
BaseSurface::BaseSurface(int /*cnt_1*/, int /*cnt_2*/) : Figure()
{
    count_first = 0;
    r = 0.2;
    R = 1;
}
void BaseSurface::createIncludingSurface()
{
    if (cells == NULL || count_first == 0)
        return;
    thisSurface = new IncludingSurface((Figure*)this,cells,count_first,count_second);
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
    int c1 = (count_first/2)*2, c2 = (count_second/2)*2;
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
    c1 = (c1%count_first+count_first)%count_first;
    c2 = (c2%count_second+count_second)%count_second;
    Figure::plus(c1*count_second + c2);
}

void BaseSurface::setPhisicSize(float s1, float s2)
{
    r = s1;
    R = s2;
}

void BaseSurface::minus(int c1, int c2)
{
    c1 = (c1%count_first+count_first)%count_first;
    c2 = (c2%count_second+count_second)%count_second;
    Figure::minus(c1*count_second + c2);
}

void BaseSurface::createField(int cnt_1, int cnt_2)
{
    if (cnt_1<1 || cnt_2<1)
        return;
    int c = (count_first = cnt_1)*(count_second = cnt_2);
    createCells(c,(count_first+1)*(count_second+1));
    createIncludingSurface();
    int k = 0;

    k=0;
    for (int i = 0; i<cnt_1; i++)
        for (int j = 0; j<cnt_2; j++)
        {
            cells[k].cnt_neighbors = 0;
//            cells[k].points[0] = &points[i*cnt_2+j];
    //        cells[k].points[3] = &points[i*cnt_2+j];
    ///        cells[k].points[2] = &points[i*cnt_2+j];
    //        cells[k++].points[1] = &points[i*cnt_2+j];
        }

    k=0;
    for (int i = 0; i<cnt_1; i++)
        for (int j = 0; j<cnt_2; j++)
        {
#define npp cells[k].cnt_neighbors++
            if (i!=cnt_1-1 && j!=cnt_2-1)
                cells[k].neighbors[npp] = &cells[(i+1)*cnt_2+j+1];
            if (i!=cnt_1-1)
                cells[k].neighbors[npp] = &cells[(i+1)*cnt_2+j];
            if (i!=cnt_1-1 && j!=0)
                cells[k].neighbors[npp] = &cells[(i+1)*cnt_2+j-1];
            if (j!=cnt_2-1)
                cells[k].neighbors[npp] = &cells[i*cnt_2+j+1];
            if (j!=0)
                cells[k].neighbors[npp] = &cells[i*cnt_2+j-1];
            if (i!=0 && j!=cnt_2-1)
                cells[k].neighbors[npp] = &cells[(i-1)*cnt_2+j+1];
            if (i!=0)
                cells[k].neighbors[npp] = &cells[(i-1)*cnt_2+j];
            if (i!=0 && j!=0)
                cells[k].neighbors[npp] = &cells[(i-1)*cnt_2+j-1];
            k++;
        }
}
