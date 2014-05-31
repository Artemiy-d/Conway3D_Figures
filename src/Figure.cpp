#include <assert.h>

#include "Figure.h"



Figure::Figure()
{
    m_gridEnable = false;
    m_lineWidth = 2.0f;
    m_cells = NULL;
    m_points = NULL;
    m_gridPoints = NULL;
    m_colorArray = NULL;
    m_pointsToDraw = NULL;
    m_normalsToCells = NULL;
    m_drawingIndices = NULL;
    m_cellsCount = m_pointsCount = m_usersCount = 0;
    m_stepNumber = 1;
    *((int*)&m_colorLive) = 0;
    *((int*)&m_colorDead) = ~0;
    *((int*)&m_colorGrid) = 51<<8;

    m_listGridId = 1;
    m_maxNeighborsCount = 8;
    defaultProbabilities();
}

void Figure::toFile(FILE * F)
{
    fwrite(&m_maxNeighborsCount,4,1,F);
    fwrite(m_probabilitiesLive,(m_maxNeighborsCount+1)*4,1,F);
    fwrite(m_probabilitiesDead,(m_maxNeighborsCount+1)*4,1,F);
    fwrite(&m_cellsCount,4,1,F);
    int key = 1, s = m_cellsCount/32+1;
    int * A = new int[s];
    for (int i = 0; i<s; i++)
        A[i] = 0;
    int * a = A;
    for (int i = 0; i<m_cellsCount; i++)
    {
        if (m_cells[i].livingStatusNow)
            (*a) |= key;
        if ( (key<<=1) == 0 )
        {
            a++;
            key = 1;
        }
    }
    fwrite(A,s*4,1,F);
    delete A;
}

void Figure::fromFile(FILE * F)
{
    int max_neigh, cnt;
    fread(&max_neigh,4,1,F);
    fread(m_probabilitiesLive,(m_maxNeighborsCount+1)*4,1,F);
    fread(m_probabilitiesDead,(m_maxNeighborsCount+1)*4,1,F);
    fread(&cnt,4,1,F);
    clearMap();
    if (cnt!=m_cellsCount)
        return;
    int key = 0, a;
    for (int i = 0; i<m_cellsCount; i++)
    {
        if ( key == 0 )
        {
            fread(&a,4,1,F);
            key = 1;
        }
        if (a & key)
            plus(i);
        key<<=1;
    }
    calcAllProbBool();
    refresh();
}

Figure::~Figure()
{
    delete m_cells->neighbors;
    delete m_cells;
    delete m_gridPoints;
    delete m_activeCellsNow;
    delete m_activeCellsNext;
    delete m_points;
    delete m_drawingIndices;
    delete m_normalsToCells;
    delete m_pointsToDraw;
    delete m_colorArray;
    delete m_gridColors;
}
void Figure::defaultProbabilities()
{
    for (int i = 0; i < 9; i++)
    {
        m_probabilitiesLive[i] = 0.;
        m_probabilitiesDead[i] = 1.;
    }
    m_probabilitiesDisabled = true;
    m_probabilitiesLive[3] = 1.;
    m_probabilitiesDead[2] = 0.;
    m_probabilitiesDead[3] = 0.;
}

void Figure::setProbabilities(double * p_live, double * p_dead)
{
    for (int i = 0; i < 9; i++)
    {
        m_probabilitiesLive[i] = p_live[i];
        m_probabilitiesDead[i] = p_dead[i];
    }
    calcAllProbBool();
}
void Figure::calcAllProbBool()
{
    m_probabilitiesDisabled = true;
    for (int i = 0; i < 9 && m_probabilitiesDisabled; i++)
    {
       // if ((m_probabilitiesLive[i]!=0 && m_probabilitiesLive[i] != RandomLCG::s_maxValue) ||
       //     (m_probabilitiesDead[i]!=0 && m_probabilitiesDead[i] != RandomLCG::s_maxValue) )
        if ( !m_probabilitiesLive[i].isLimit() || !m_probabilitiesDead[i].isLimit() )
            m_probabilitiesDisabled = false;
    }
    if (!m_probabilitiesDisabled)
    {
        for (int i = 0; i<m_cellsCount; i++)
            (m_activeCellsNow[i] = &m_cells[i])->steps = m_stepNumber;
        m_activeCountNow = m_cellsCount;
        refresh();
    }
}

void Figure::getProbabilities(double * p_live, double * p_dead)
{
    for (int i = 0; i<9; i++)
    {
        p_live[i] = m_probabilitiesLive[i].get();
        p_dead[i] = m_probabilitiesDead[i].get();
    }
}

void Figure::selectAndPlus(const Point3F & _p1, const Point3F & _p_1, bool _plus, Model * _m)
{
    float minR = 1000000;
    int minInd = -1;
    for (int i = 0; i<m_cellsCount; i++)
    {
        float r1 = m_normalsToCells[i*4+3] & (_p1 - m_pointsToDraw[i*4]);
        float r2 = m_normalsToCells[i*4+3] & (_p_1 - m_pointsToDraw[i*4]);
        if (r1*r2<0)
        {
            Point3F ip = _p1 - (_p_1 - _p1) * ( r1 / (r2 - r1));
            bool cellFound = true;
            for (int j = 0; j < 4 && cellFound; j++)
            {
                if ( (( (m_pointsToDraw[i*4+(j+1)%4] - m_pointsToDraw[i*4+j]) ^
                        (ip - m_pointsToDraw[i*4+j])) & m_normalsToCells[i*4+3]) < 0)
                {
                    cellFound = false;
                }
            }
            if ( cellFound )
            {
                float r = (ip - _p1).abs();
                if (r < minR)
                {
                    minR = r;
                    minInd = i;
                }
            }
        }
    }
    if (minInd != -1)
    {
        if (_m == NULL)
        {
            if (_plus)
                plus(minInd);
            else
                minus(minInd);
        }
        else
        {
            addModel(_m, &m_cells[minInd]);
        }
    }
}

int Figure::getLivingCellsCount()
{
    return m_usersCount;
}

void Figure::createGrid()
{
    delete m_gridPoints;
    m_gridPoints = new GLuint[m_cellsCount*8];

    len_grid_points = 0;

    for (int i = 0; i<m_cellsCount; i++)
    {
        int i4 = i*4;
        GLuint * I = m_cells[i].indices;
        for (int j = 0; j<4; j++)
        {
            m_gridColors[i4+j] = m_colorGrid;
            m_pointsToDraw[i4+j] = m_points[I[j]];
            m_drawingIndices[i4+j] = i4+j;
            if (ArrayCellSides[j] & m_cells[i].paintSides)
            {
                m_gridPoints[len_grid_points++] = i4+(j+1)%4;
                m_gridPoints[len_grid_points++] = i4+j;
            }
            int j1 = (j+1)%4, j_1 = (j+3)%4;
            m_normalsToCells[i4+j] = (m_points[I[j1]] - m_points[I[j]]) ^
                    ( m_points[I[j_1]] - m_points[I[j]] );
        }

    }

   // gridToList();
}

void Figure::setLineWidth(GLfloat width)
{
    m_lineWidth = width;
    gridToList();
}


void Figure::createCells(int cnt, int cnt_pnt)
{
    if (cnt<1 || cnt_pnt<4)
        return;
    if (m_cells!=NULL)
    {
        delete m_cells->neighbors;
        delete m_activeCellsNow;
        delete m_activeCellsNext;
        delete m_drawingIndices;
        delete m_colorArray;
        delete m_pointsToDraw;
        delete m_normalsToCells;
        delete m_gridColors;

    }
    m_cells = new Cell[cnt];
    m_cells->neighbors = new Cell*[cnt*m_maxNeighborsCount];
    for (int i = 1; i<cnt; i++)
    {
        m_cells[i].neighbors = m_cells->neighbors + i*m_maxNeighborsCount;
    }

    m_points = new Point3F[cnt_pnt];
    m_normalsToCells = new Point3F[cnt*4];

    m_pointsToDraw = new Point3F[cnt*4];

    m_colorArray = new Color4B[cnt*4];
    m_gridColors = new Color4B[cnt*4];

    m_activeCellsNow = new Cell*[cnt];
    m_activeCellsNext = new Cell*[cnt];

    m_drawingIndices = new GLuint[cnt*4];
  //  nrm = new

    m_cellsCount = cnt;
    m_pointsCount = cnt_pnt;

 //   m_normalsToCells = norm;

    initBegin();
}

void Figure::refresh()
{
    for (int i = 0; i<m_activeCountNow; i++)
    {
        m_activeCellsNow[i]->livingStatusNext = m_activeCellsNow[i]->livingStatusNow;
        m_activeCellsNow[i]->nextCountActiveNeighbours = m_activeCellsNow[i]->currentCountActiveNeighbours;
    }
}

void Figure::plus(Cell * c)
{
    if (c->livingStatusNow)
        return;
    c->livingStatusNow = true;
    m_usersCount++;
    for (int j = 0; j<c->neighboursCount; j++)
    {
        c->neighbors[j]->currentCountActiveNeighbours++;
        if (c->neighbors[j]->steps!= m_stepNumber)
            (m_activeCellsNow[m_activeCountNow++] = c->neighbors[j])->steps = m_stepNumber;
    }
    if (c->steps!= m_stepNumber)
        (m_activeCellsNow[m_activeCountNow++] = c)->steps = m_stepNumber;
}

void Figure::plus(int ind)
{
    plus(&m_cells[(ind + m_cellsCount * 4) % m_cellsCount]);
}

void Figure::minus(Cell * c)
{
    if (!c->livingStatusNow)
        return;
    c->livingStatusNow = false;
    m_usersCount--;
    for (int j = 0; j<c->neighboursCount; j++)
    {
        c->neighbors[j]->currentCountActiveNeighbours--;
        if (c->neighbors[j]->steps!= m_stepNumber)
            (m_activeCellsNow[m_activeCountNow++] = c->neighbors[j])->steps = m_stepNumber;
    }
    if (c->steps!= m_stepNumber)
        (m_activeCellsNow[m_activeCountNext++] = c)->steps = m_stepNumber;
}
void Figure::minus(int ind)
{
    minus(&m_cells[(ind+m_cellsCount*4)%m_cellsCount]);
}

void Figure::step()
{
    m_stepNumber++;
    m_activeCountNext = 0;
    Cell * c;
    int j;
    unsigned int u;

    for (int i = 0; i<m_activeCountNow; i++)
    {
        c = m_activeCellsNow[i];
        if (c->livingStatusNow)
        {
           // u = m_probabilitiesDead[c->currentCountActiveNeighbours];
            if ( m_probabilitiesDead[c->currentCountActiveNeighbours].simulate( m_random ) )//(u != 0 && (u == RandomLCG::s_maxValue || m_random.next() < u))
            {
                c->livingStatusNext = false;
                --m_usersCount;
                for (j = 0; j<c->neighboursCount; j++)
                {
                    c->neighbors[j]->nextCountActiveNeighbours--;
                    if (c->neighbors[j]->steps!= m_stepNumber)
                        (m_activeCellsNext[m_activeCountNext++] = c->neighbors[j])->steps = m_stepNumber;
                }
                if (c->steps != m_stepNumber)
                    (m_activeCellsNext[m_activeCountNext++] = c)->steps = m_stepNumber;
            }
        }
        else
        {
            if ( m_probabilitiesLive[c->currentCountActiveNeighbours].simulate( m_random ) )
            {
                c->livingStatusNext = true;
                ++m_usersCount;
                for (j = 0; j<c->neighboursCount; j++)
                {
                    c->neighbors[j]->nextCountActiveNeighbours++;
                    if (c->neighbors[j]->steps!= m_stepNumber)
                        (m_activeCellsNext[m_activeCountNext++] = c->neighbors[j])->steps = m_stepNumber;
                }
                if (c->steps!= m_stepNumber)
                    (m_activeCellsNext[m_activeCountNext++] = c)->steps = m_stepNumber;
            }
        }

    }

    if (!m_probabilitiesDisabled)
        for (int i = 0; i < m_activeCountNow; i++)
        {
            u = m_activeCellsNow[i]->currentCountActiveNeighbours;
            if (m_activeCellsNow[i]->steps != m_stepNumber &&
                    !( m_probabilitiesDead[u].isLimit() && m_probabilitiesLive[u].isLimit() ) )
            {
                (m_activeCellsNext[m_activeCountNext++] = m_activeCellsNow[i])->steps = m_stepNumber;
            }
        }
//    else
//    {
//        for (int i = 0; i<m_activeCountNext; i++)
//            if (m_activeCellsNext[i]->livingStatusNow == m_activeCellsNext[i]->livingStatusNext &&
//                    m_activeCellsNext[i]->currentCountActiveNeighbours == m_activeCellsNext[i]->nextCountActiveNeighbours)
//            {
//                m_activeCellsNext[i--] = m_activeCellsNext[--m_activeCountNext];
//            }
//    }

    for (int i = 0; i < m_activeCountNext; i++)
    {
        m_activeCellsNext[i]->livingStatusNow = m_activeCellsNext[i]->livingStatusNext;
        m_activeCellsNext[i]->currentCountActiveNeighbours = m_activeCellsNext[i]->nextCountActiveNeighbours;
    }


    Cell ** acn = m_activeCellsNow;
    m_activeCellsNow = m_activeCellsNext;
    m_activeCellsNext = acn;
    m_activeCountNow = m_activeCountNext;

}

void Figure::clearMap()
{
    for (int i = 0; i < m_cellsCount; i++)
        minus(i);
    refresh();
}

void Figure::createRandomMap(float _p)
{
    RandomLCGDefault::Probability p(_p);
    for (int i = 0; i < m_cellsCount; i++)
    {
        if ( p.simulate( m_random ) )
            plus(i);
    }
    refresh();
}

void Figure::initBegin()
{
    m_activeCountNow = m_activeCountNext = m_usersCount = 0;
    m_stepNumber = 1;
    for (int i = 0; i<m_cellsCount; i++)
    {
        m_cells[i].paintSides = ~0;
        m_cells[i].livingStatusNext = m_cells[i].livingStatusNow = false;
        m_cells[i].nextCountActiveNeighbours = m_cells[i].currentCountActiveNeighbours = m_cells[i].steps = 0;
        m_cells[i].neighboursCount = 8;
    }

}

void Figure::drawList()
{
    glCallList(1);
}

void Figure::gridToList()
{
    if (m_lineWidth!=0.0f)
    {
        glNewList(m_listGridId, GL_COMPILE);
        glEnableClientState(GL_VERTEX_ARRAY);

        glLineWidth(m_lineWidth);
        glColor3f(0.0f,0.0f,1.0f);
        glVertexPointer(3, GL_FLOAT, 0, m_points);
        glDrawElements(GL_LINES,len_grid_points,GL_UNSIGNED_INT,m_gridPoints);
        glEndList();
    }
}

void Figure::drawCells()
{

   // glNewList(1,GL_COMPILE);

    for (int i = 0; i<m_cellsCount; i++)
    {
        int i4 = i*4;
        m_colorArray[i4+3] = m_cells[i].livingStatusNow?m_colorLive:m_colorDead;
        //m_colorArray[i4+1] = m_colorArray[i4+2] = m_colorArray[i4+3] = m_colorArray[i4];
    }

    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_NORMALIZE);

    glVertexPointer(3, GL_FLOAT, 0, m_pointsToDraw);
    glNormalPointer(GL_FLOAT,0,m_normalsToCells);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_colorArray);

    glDrawElements(GL_QUADS,m_cellsCount*4,GL_UNSIGNED_INT,m_drawingIndices);

   // glDisableClientState(GL_NORMAL_ARRAY);
  //  if (m_lineWidth!=0.0f) glCallList(m_listGridId);
    if (m_gridEnable && m_lineWidth != 0.0f)
    {
        glDisable(GL_LIGHTING);
       // glDisableClientState(GL_NORMAL_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_gridColors);
        glLineWidth(m_lineWidth);
        glDrawElements(GL_LINES,len_grid_points,GL_UNSIGNED_INT,m_gridPoints);
    }

        
//    glBegin(GL_LINES);
//    glColor3f(0.0f,0.0f,1.0f);
//    glLineWidth(2.0);
//    for (int i = 0; i<m_cellsCount; i++)
//    {
//
//        glVertex3fv((GLfloat*)m_cells[i].m_points[0]);
//        glVertex3fv((GLfloat*)m_cells[i].m_points[1]);
//        glVertex3fv((GLfloat*)m_cells[i].m_points[1]);
//        glVertex3fv((GLfloat*)m_cells[i].m_points[2]);
//     //   glVertex3fv((GLfloat*)m_cells[i].m_points[2]);
//     //   glVertex3fv((GLfloat*)m_cells[i].m_points[3]);
//    //    glVertex3fv((GLfloat*)m_cells[i].m_points[3]);
//    //    glVertex3fv((GLfloat*)m_cells[i].m_points[0]);
//
//    }
 //   glEnd();

    
  //  glEndList();
 //   glCallList(1);
    
    
}

void Figure::drawActiveCells()
{
   if (m_cells == NULL)
       return;

    //glNewList(1,GL_COMPILE);

    glLineWidth(m_lineWidth);

    glEnableClientState(GL_VERTEX_ARRAY);


    int a = 0, b = m_cellsCount*4 - 1;
    for (int i = 0; i<m_activeCountNow; i++)
    {
        Cell * c = m_activeCellsNow[i];
        if (m_cells[i].livingStatusNow)
        {
            m_drawingIndices[a++] = c->indices[0];
            m_drawingIndices[a++] = c->indices[1];
            m_drawingIndices[a++] = c->indices[2];
            m_drawingIndices[a++] = c->indices[3];
        }
        else
        {
            m_drawingIndices[b--] = c->indices[3];
            m_drawingIndices[b--] = c->indices[2];
            m_drawingIndices[b--] = c->indices[1];
            m_drawingIndices[b--] = c->indices[0];
        }

    }
    glColor3fv((GLfloat*)&m_colorLive);
    glVertexPointer(3, GL_FLOAT, 0, m_points);
    glDrawElements(GL_QUADS, a, GL_UNSIGNED_INT, m_drawingIndices);


    glColor3fv((GLfloat*)&m_colorDead);
    glDrawElements(GL_QUADS, m_cellsCount*4 - 1 - b, GL_UNSIGNED_INT, m_drawingIndices+b+1);

  //  glColor3f(0.0f,0.0f,1.0f);
   // glDrawElements(GL_LINES,len_grid_points,GL_UNSIGNED_INT,m_gridPoints);
}


