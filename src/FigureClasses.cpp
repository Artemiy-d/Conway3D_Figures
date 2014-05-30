#include "FigureClasses.h"


Surface::Surface() : BaseSurface() { }

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

void Surface::createField(int cnt_1, int cnt_2)
{
    m_firstSideCount = cnt_1;
    m_secondSideCount = cnt_2;
    createCells(m_firstSideCount*m_secondSideCount,(m_secondSideCount+1)*(m_firstSideCount+1));
    createIncludingSurface();
    setPhisicSize(1.0f, 0);
    thisSurface->surfaceCellsConnect();

}
void Surface::setPhisicSize(float s, float)
{
    r = m_firstSideCount * s / (m_firstSideCount+m_secondSideCount);
    R = m_secondSideCount * s / (m_firstSideCount+m_secondSideCount);
    Point3F p0(-r,-R,0);
    Point3F p1(r,-R,0);
    Point3F p3(-r,R,0);
    int index = 0;
    this->thisSurface->createFullPlaneVertexes(p0,p1,p3,m_points,index);
    createGrid();
}


Torus::Torus() : BaseSurface() {}
Torus::Torus(int cnt_1, int cnt_2, int offset_1) : BaseSurface(cnt_1,cnt_2)
{
    R = 0.5;
    r = R * cnt_1 / cnt_2;
    if (r > 0.9 * R) r = 0.9 * R;
    createField(m_firstSideCount = cnt_1,m_secondSideCount = cnt_2, offset_1);
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
    for (int i = 0; i<m_firstSideCount; i++)
        for (int j = 0; j<m_secondSideCount; j++)
        {
            float b = i*2*M_PI/m_firstSideCount + j * m_angle, a = j*2*M_PI/m_secondSideCount;
            float cb = R + r*cos(b);
            int k = i * m_secondSideCount+ j;
            m_points[k].x = -cos(a)*cb;
            m_points[k].y = -sin(a)*cb;
            m_points[k].z = r * sin(b);
        }

}
void Torus::createField(int cnt_1, int cnt_2, int offset_1)
{
    if (cnt_1<1 || cnt_2<1) return;
  //  BaseSurface::createField(cnt_1,cnt_2);
    int c = (m_firstSideCount = cnt_1)*(m_secondSideCount = cnt_2);
    createCells(c,c);
    createIncludingSurface();
    int k = 0;
    m_offset = offset_1;
    m_angle = offset_1 * 2*M_PI/(cnt_1*cnt_2);

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
            m_cells[k].paintSides = Side01|Side12;

            m_cells[k].indices[0]=i*cnt_2+j;

            m_cells[k].indices[1]=i1*cnt_2+j1;
            m_cells[k].indices[2]=i2*cnt_2+j2;
            m_cells[k].indices[3]=i3*cnt_2+j3;
           
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
//            m_cells[k].neighbors[0] = &m_cells[i0*cnt_2+j0];
//            m_cells[k].neighbors[1] = &m_cells[i1*cnt_2+j1];
//            m_cells[k].neighbors[2] = &m_cells[i2*cnt_2+j2];
//            m_cells[k].neighbors[3] = &m_cells[i3*cnt_2+j3];
//            m_cells[k].neighbors[4] = &m_cells[i4*cnt_2+j4];
//            m_cells[k].neighbors[5] = &m_cells[i5*cnt_2+j5];
//            m_cells[k].neighbors[6] = &m_cells[i6*cnt_2+j6];
//            m_cells[k].neighbors[7] = &m_cells[i7*cnt_2+j7];
//            m_cells[k].neighboursCount = 8;
//        }
//    }

}


Ellipsoid::Ellipsoid() : Figure()
{
    m_ellipsoidForm = true;
    m_size = 1.0;
}
Ellipsoid::Ellipsoid(int cnt_1, int cnt_2, int cnt_3, bool EllipsoidFormOn) : Figure()
{
    m_ellipsoidForm = EllipsoidFormOn;
    m_size = 1.0;
    createField(cnt_1,cnt_2,cnt_3);
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


void Ellipsoid::setPhisicSize(float s, float)
{
    m_size = s;
    m_scale = 3*m_size / (m_firstSideCount+ m_secondSideCount + m_thirdSideCount);
    int c =  2*(m_firstSideCount * m_secondSideCount + m_firstSideCount * m_thirdSideCount + m_secondSideCount * m_thirdSideCount);
    int cp =  2*((m_firstSideCount+1) * (m_secondSideCount+1) + (m_firstSideCount+1) * (m_thirdSideCount+1) + (m_secondSideCount+1) * (m_thirdSideCount+1));
    createCells(c,cp);
    m_firstSize = m_firstSideCount * m_scale;
    m_secondSize = m_secondSideCount * m_scale;
    m_thirdSize = m_thirdSideCount * m_scale;
    Point3F pnts[8];
    pnts[0].y = pnts[1].y = pnts[2].y = pnts[3].y = -m_secondSize/2;
    pnts[4].y = pnts[5].y = pnts[6].y = pnts[7].y = m_secondSize/2;

    pnts[0].x = pnts[1].x = pnts[4].x = pnts[5].x = -m_firstSize/2;
    pnts[3].x = pnts[2].x = pnts[7].x = pnts[6].x = m_firstSize/2;

    pnts[0].z = pnts[3].z = pnts[4].z = pnts[7].z = m_thirdSize/2;
    pnts[1].z = pnts[2].z = pnts[5].z = pnts[6].z = -m_thirdSize/2;

    int index = 0;
    Cell * C = m_cells;
    m_surfaces[0] = new IncludingSurface(this,C,m_thirdSideCount,m_firstSideCount);
    m_surfaces[0]->createFullPlaneVertexes(pnts[0],pnts[1],pnts[3],m_points,index);
    C += m_thirdSideCount * m_firstSideCount;
    m_surfaces[1] = new IncludingSurface(this,C,m_secondSideCount,m_firstSideCount);
    m_surfaces[1]->createFullPlaneVertexes(pnts[1],pnts[5],pnts[2],m_points,index);
    C += m_secondSideCount * m_firstSideCount;
    m_surfaces[2] = new IncludingSurface(this,C,m_thirdSideCount,m_firstSideCount);
    m_surfaces[2]->createFullPlaneVertexes(pnts[4],pnts[5],pnts[7],m_points,index);
    C += m_thirdSideCount * m_firstSideCount;
    m_surfaces[3] = new IncludingSurface(this,C,m_secondSideCount,m_firstSideCount);
    m_surfaces[3]->createFullPlaneVertexes(pnts[0],pnts[4],pnts[3],m_points,index);
    C += m_secondSideCount * m_firstSideCount;
    m_surfaces[4] = new IncludingSurface(this,C,m_secondSideCount,m_thirdSideCount);
    m_surfaces[4]->createFullPlaneVertexes(pnts[3],pnts[7],pnts[2],m_points,index);
    C += m_thirdSideCount * m_secondSideCount;
    m_surfaces[5] = new IncludingSurface(this,C,m_thirdSideCount,m_secondSideCount);
    m_surfaces[5]->createFullPlaneVertexes(pnts[0],pnts[1],pnts[4],m_points,index);
    C += m_thirdSideCount * m_secondSideCount;

    if (m_ellipsoidForm)
    {
        for (int i = 0; i<m_pointsCount; i++)
        {
            float K = m_size / sqrt(pow(m_points[i].x/m_firstSize,2)+pow(m_points[i].y/m_secondSize,2)+pow(m_points[i].z/m_thirdSize,2));
            m_points[i].x*=K;
            m_points[i].y*=K;
            m_points[i].z*=K;
        }
    }

}

void Ellipsoid::setEllipsoidFormEnable(bool on)
{
    m_ellipsoidForm = on;
    setPhisicSize(m_size, 0);
}

void Ellipsoid::createAgar()
{
    for (int i = 0; i<6; i++)
        m_surfaces[i]->createAgar();
}
void Ellipsoid::addModel(Model * M, Cell * C)
{
    for (int i = 0; i<6; i++)
    {
        if (m_surfaces[i]->addModel(M,C))
            return;
    }
}

void Ellipsoid::createField(int cnt_1, int cnt_2, int cnt_3)
{
    if (cnt_1 <= 0 || cnt_2 <= 0 || cnt_3 <= 0)
        return;
    m_firstSideCount = cnt_1;
    m_secondSideCount = cnt_2;
    m_thirdSideCount  = cnt_3;
    setPhisicSize(1, 0);


    IncludingSurface::ConnectPointsOfSurf(m_surfaces[0],Side30,m_surfaces[1],Side12);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[1],Side30,m_surfaces[2],Side30);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[2],Side12,m_surfaces[3],Side30);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[3],Side12,m_surfaces[0],Side12);

    IncludingSurface::ConnectPointsOfSurf(m_surfaces[0],Side01,m_surfaces[4],Side12);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[1],Side01,m_surfaces[4],Side01);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[3],Side01,m_surfaces[4],Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5],Side23,m_surfaces[0],Side23);

    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5],Side30,m_surfaces[1],Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5],Side12,m_surfaces[3],Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[5],Side01,m_surfaces[2],Side23);
    IncludingSurface::ConnectPointsOfSurf(m_surfaces[2],Side01,m_surfaces[4],Side30);


    for (int i = 0; i<6; i++)
        m_surfaces[i]->surfaceCellsConnect();

    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[0],Side30,m_surfaces[1],Side12);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[1],Side30,m_surfaces[2],Side30);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[2],Side12,m_surfaces[3],Side30);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[3],Side12,m_surfaces[0],Side12);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[0],Side01,m_surfaces[4],Side12);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[1],Side01,m_surfaces[4],Side01);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[3],Side01,m_surfaces[4],Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5],Side23,m_surfaces[0],Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5],Side30,m_surfaces[1],Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5],Side12,m_surfaces[3],Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[5],Side01,m_surfaces[2],Side23);
    IncludingSurface::ConnectSurfacesByPoints(m_surfaces[2],Side01,m_surfaces[4],Side30);

    createGrid();

}



