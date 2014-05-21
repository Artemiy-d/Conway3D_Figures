/*
 * File:   FigureClasses.cpp
 * Author: artyom
 * 
 * Created on 4 Ноябрь 2011 г., 1:55
 */

#include "FigureClasses.h"


Surface::Surface() : BaseSurface() {}

Surface::Surface(int cnt_1, int cnt_2) : BaseSurface(cnt_1,cnt_2)
{
    createField(cnt_1,cnt_2);
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
    fwrite(&count_first,4,1,F);
    fwrite(&count_second,4,1,F);
    Figure::toFile(F);
}
void Surface::fromFile(FILE * F)
{
    int sz_param, cnt_1, cnt_2;
    fread(&sz_param,4,1,F);
    fread(&cnt_1,4,1,F);
    fread(&cnt_2,4,1,F);
    if (cnt_1 < 1 || cnt_2 < 1) return;
    createField(cnt_1, cnt_2);
    Figure::fromFile(F);
}

void Surface::createField(int cnt_1, int cnt_2)
{
    count_first = cnt_1;
    count_second = cnt_2;
    createCells(count_first*count_second,(count_second+1)*(count_first+1));
    createIncludingSurface();
    setPhisicSize(1.0f, 0);
    thisSurface->surfaceCellsConnect();

}
void Surface::setPhisicSize(float s, float)
{
    r = count_first * s / (count_first+count_second);
    R = count_second * s / (count_first+count_second);
    fpoint p0(-r,-R,0);
    fpoint p1(r,-R,0);
    fpoint p3(-r,R,0);
    int index = 0;
    this->thisSurface->createFullPlaneVertexes(p0,p1,p3,points,index);
    createGrid();
}


Torus::Torus() : BaseSurface() {}
Torus::Torus(int cnt_1, int cnt_2, int offset_1) : BaseSurface(cnt_1,cnt_2)
{
    R = 0.5;
    r = R * cnt_1 / cnt_2;
    if (r > 0.9 * R) r = 0.9 * R;
    createField(count_first = cnt_1,count_second = cnt_2, offset_1);
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
    fwrite(&count_first,4,1,F);
    fwrite(&count_second,4,1,F);
    fwrite(&offset,4,1,F);
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
    R = 0.5;
    r = R * cnt_1 / cnt_2;
    if (r > 0.9 * R) r = 0.9 * R;
    createField(cnt_1, cnt_2, offs);
    Figure::fromFile(F);
}

void Torus::setPhisicSize(float s1, float s2)
{
    r = s1;
    R = s2;
    for (int i = 0; i<count_first; i++)
        for (int j = 0; j<count_second; j++)
        {
            float b = i*2*M_PI/count_first + j * angle, a = j*2*M_PI/count_second;
            float cb = R + r*cos(b);
            int k = i * count_second+ j;
            points[k].x = -cos(a)*cb;
            points[k].y = -sin(a)*cb;
            points[k].z = r * sin(b);
        }

}
void Torus::createField(int cnt_1, int cnt_2, int offset_1)
{
    if (cnt_1<1 || cnt_2<1) return;
  //  BaseSurface::createField(cnt_1,cnt_2);
    int c = (count_first = cnt_1)*(count_second = cnt_2);
    createCells(c,c);
    createIncludingSurface();
    int k = 0;
    offset = offset_1;
    angle = offset_1 * 2*M_PI/(cnt_1*cnt_2);

    setPhisicSize(r,R);

    for (int i = 0; i<cnt_1; i++)
    {
        int i1 = (i+1)%cnt_1, i3 = i, i2 = i1;
        for (int j = 0; j<cnt_2; j++)
        {
            int j1 = j, j2=(j+1)%cnt_2 ,j3=j2;
            if (j2==0)
            {
                i3 = (i+offset_1) % cnt_1;
                i2 = (i3 + 1) % cnt_1;
            }
            k = i * cnt_2 + j;
            cells[k].paintSides = Side01|Side12;

            cells[k].indexes[0]=i*cnt_2+j;

            cells[k].indexes[1]=i1*cnt_2+j1;
            cells[k].indexes[2]=i2*cnt_2+j2;
            cells[k].indexes[3]=i3*cnt_2+j3;
           
        }
    }

  //  this->thisSurface->createVertexesByMain();
    IncludingSurface::ConnectPointsOfSurf(thisSurface,Side23,thisSurface,Side01,offset_1);
    IncludingSurface::ConnectPointsOfSurf(thisSurface,Side12,thisSurface,Side30);

    
    this->thisSurface->surfaceCellsConnect();
    IncludingSurface::ConnectSurfacesByPoints(thisSurface,Side01,thisSurface,Side23);
    IncludingSurface::ConnectSurfacesByPoints(thisSurface,Side30,thisSurface,Side12);

    createGrid();


//    k=0;
//    int i0,i1,i2,i3,i4,i5,i6,i7;
//    for (int i = 0; i<cnt_1; i++)
//    {
//        i0 = i3 = i5 = (i - 1 + cnt_1) % cnt_1;
//        i1 = i6 = i;
//        i7 = i4 = i2 = (i+1) % cnt_1;
//        for (int j = 0; j<cnt_2; j++)
//        {
//            int j0 = (j-1 + cnt_2) % cnt_2, j1 = j0, j2 = j0, j3 = j, j4 = j,
//                    j5 = (j+1)%cnt_2, j6 = j5, j7 = j5;
//
//            k = i * cnt_2 + j;
//            if (j == cnt_2-1)
//            {
//
//                i6 = (i + kangle) % cnt_1;
//                i5 = (i6 - 1 + cnt_1) % cnt_1;
//                i7 = (i6 + 1+cnt_1) % cnt_1;
//            }
//            else if (j == 0)
//            {
//                i1 = (i - kangle+ cnt_1) % cnt_1;
//                i0 = (i1 - 1 + cnt_1) % cnt_1;
//                i2 = (i1 + 1+ cnt_1) % cnt_1;
//            }
//            else
//            {
//                i0 = i3 = i5 = (i - 1 + cnt_1) % cnt_1;
//                i1 = i6 = i;
//                i7 = i4 = i2 = (i+1) % cnt_1;
//            }
//
//            cells[k].neighbors[0] = &cells[i0*cnt_2+j0];
//            cells[k].neighbors[1] = &cells[i1*cnt_2+j1];
//            cells[k].neighbors[2] = &cells[i2*cnt_2+j2];
//            cells[k].neighbors[3] = &cells[i3*cnt_2+j3];
//            cells[k].neighbors[4] = &cells[i4*cnt_2+j4];
//            cells[k].neighbors[5] = &cells[i5*cnt_2+j5];
//            cells[k].neighbors[6] = &cells[i6*cnt_2+j6];
//            cells[k].neighbors[7] = &cells[i7*cnt_2+j7];
//            cells[k].cnt_neighbors = 8;
//        }
//    }

}


Ellipsoid::Ellipsoid() : Figure()
{
    ellipseFormOn = true;
    Size = 1.0;
}
Ellipsoid::Ellipsoid(int cnt_1, int cnt_2, int cnt_3, bool EllipsoidFormOn) : Figure()
{
    ellipseFormOn = EllipsoidFormOn;
    Size = 1.0;
    createField(cnt_1,cnt_2,cnt_3);
}

Ellipsoid::Ellipsoid(FILE * F) : Figure()
{
    Size = 1.0;
    fromFile(F);
}

void Ellipsoid::toFile(FILE * F)
{
    int sz_param = 13, type = (int) ellipseFormOn?figEllipsoid:figParallelepiped;
    fwrite(&type,4,1,F);
    fwrite(&sz_param,4,1,F);
    fwrite(&count_first,4,1,F);
    fwrite(&count_second,4,1,F);
    fwrite(&count_three,4,1,F);
    fwrite(&ellipseFormOn,1,1,F);
    Figure::toFile(F);
}

void Ellipsoid::fromFile(FILE * F)
{
    int sz_param, cnt_1, cnt_2, cnt_3;
    fread(&sz_param,4,1,F);
    fread(&cnt_1,4,1,F);
    fread(&cnt_2,4,1,F);
    fread(&cnt_3,4,1,F);
    fread(&ellipseFormOn,1,1,F);
    if (cnt_1 == 0 || cnt_2 == 0 || cnt_3 == 0)
        return;
    createField(cnt_1, cnt_2, cnt_3);
    Figure::fromFile(F);
}


void Ellipsoid::setPhisicSize(float s, float)
{
    Size = s;
    scale = 3*Size / (count_first+ count_second + count_three);
    int c =  2*(count_first * count_second + count_first * count_three + count_second * count_three);
    int cp =  2*((count_first+1) * (count_second+1) + (count_first+1) * (count_three+1) + (count_second+1) * (count_three+1));
    createCells(c,cp);
    size_1 = count_first * scale;
    size_2 = count_second * scale;
    size_3 = count_three * scale;
    fpoint pnts[8];
    pnts[0].y = pnts[1].y = pnts[2].y = pnts[3].y = -size_2/2;
    pnts[4].y = pnts[5].y = pnts[6].y = pnts[7].y = size_2/2;

    pnts[0].x = pnts[1].x = pnts[4].x = pnts[5].x = -size_1/2;
    pnts[3].x = pnts[2].x = pnts[7].x = pnts[6].x = size_1/2;

    pnts[0].z = pnts[3].z = pnts[4].z = pnts[7].z = size_3/2;
    pnts[1].z = pnts[2].z = pnts[5].z = pnts[6].z = -size_3/2;

    int index = 0;
    Cell * C = cells;
    surf[0] = new IncludingSurface(this,C,count_three,count_first);
    surf[0]->createFullPlaneVertexes(pnts[0],pnts[1],pnts[3],points,index);
    C += count_three * count_first;
    surf[1] = new IncludingSurface(this,C,count_second,count_first);
    surf[1]->createFullPlaneVertexes(pnts[1],pnts[5],pnts[2],points,index);
    C += count_second * count_first;
    surf[2] = new IncludingSurface(this,C,count_three,count_first);
    surf[2]->createFullPlaneVertexes(pnts[4],pnts[5],pnts[7],points,index);
    C += count_three * count_first;
    surf[3] = new IncludingSurface(this,C,count_second,count_first);
    surf[3]->createFullPlaneVertexes(pnts[0],pnts[4],pnts[3],points,index);
    C += count_second * count_first;
    surf[4] = new IncludingSurface(this,C,count_second,count_three);
    surf[4]->createFullPlaneVertexes(pnts[3],pnts[7],pnts[2],points,index);
    C += count_three * count_second;
    surf[5] = new IncludingSurface(this,C,count_three,count_second);
    surf[5]->createFullPlaneVertexes(pnts[0],pnts[1],pnts[4],points,index);
    C += count_three * count_second;

    if (ellipseFormOn)
    {
        for (int i = 0; i<cnt_points; i++)
        {
            float K = Size/sqrt(pow(points[i].x/size_1,2)+pow(points[i].y/size_2,2)+pow(points[i].z/size_3,2));
            points[i].x*=K;
            points[i].y*=K;
            points[i].z*=K;
        }
    }

}

void Ellipsoid::setEllipsoidFormEnable(bool on)
{
    ellipseFormOn = on;
    setPhisicSize(Size, 0);
}

void Ellipsoid::createAgar()
{
    for (int i = 0; i<6; i++)
        surf[i]->createAgar();
}
void Ellipsoid::addModel(Model * M, Cell * C)
{
    for (int i = 0; i<6; i++)
    {
        if (surf[i]->addModel(M,C))
            return;
    }
}

void Ellipsoid::createField(int cnt_1, int cnt_2, int cnt_3)
{
    if (cnt_1 <= 0 || cnt_2 <= 0 || cnt_3 <= 0)
        return;
    count_first = cnt_1;
    count_second = cnt_2;
    count_three  = cnt_3;
    setPhisicSize(1, 0);


    IncludingSurface::ConnectPointsOfSurf(surf[0],Side30,surf[1],Side12);
    IncludingSurface::ConnectPointsOfSurf(surf[1],Side30,surf[2],Side30);
    IncludingSurface::ConnectPointsOfSurf(surf[2],Side12,surf[3],Side30);
    IncludingSurface::ConnectPointsOfSurf(surf[3],Side12,surf[0],Side12);

    IncludingSurface::ConnectPointsOfSurf(surf[0],Side01,surf[4],Side12);
    IncludingSurface::ConnectPointsOfSurf(surf[1],Side01,surf[4],Side01);
    IncludingSurface::ConnectPointsOfSurf(surf[3],Side01,surf[4],Side23);
    IncludingSurface::ConnectPointsOfSurf(surf[5],Side23,surf[0],Side23);

    IncludingSurface::ConnectPointsOfSurf(surf[5],Side30,surf[1],Side23);
    IncludingSurface::ConnectPointsOfSurf(surf[5],Side12,surf[3],Side23);
    IncludingSurface::ConnectPointsOfSurf(surf[5],Side01,surf[2],Side23);
    IncludingSurface::ConnectPointsOfSurf(surf[2],Side01,surf[4],Side30);


    for (int i = 0; i<6; i++) surf[i]->surfaceCellsConnect();

    IncludingSurface::ConnectSurfacesByPoints(surf[0],Side30,surf[1],Side12);
    IncludingSurface::ConnectSurfacesByPoints(surf[1],Side30,surf[2],Side30);
    IncludingSurface::ConnectSurfacesByPoints(surf[2],Side12,surf[3],Side30);
    IncludingSurface::ConnectSurfacesByPoints(surf[3],Side12,surf[0],Side12);
    IncludingSurface::ConnectSurfacesByPoints(surf[0],Side01,surf[4],Side12);
    IncludingSurface::ConnectSurfacesByPoints(surf[1],Side01,surf[4],Side01);
    IncludingSurface::ConnectSurfacesByPoints(surf[3],Side01,surf[4],Side23);
    IncludingSurface::ConnectSurfacesByPoints(surf[5],Side23,surf[0],Side23);
    IncludingSurface::ConnectSurfacesByPoints(surf[5],Side30,surf[1],Side23);
    IncludingSurface::ConnectSurfacesByPoints(surf[5],Side12,surf[3],Side23);
    IncludingSurface::ConnectSurfacesByPoints(surf[5],Side01,surf[2],Side23);
    IncludingSurface::ConnectSurfacesByPoints(surf[2],Side01,surf[4],Side30);

    createGrid();

}



