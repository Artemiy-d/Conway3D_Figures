/* 
* File:   Figure.cpp
* Author: artyom
* 
* Created on 1 Ноябрь 2011 г., 21:35
*/

#include "complect_headers.h"
#include "Figure.h"


Figure::Figure()
{
    srand(time(0));
    m_gridEnable = false;
    line_width = 2.0f;
    cells = NULL;
    points = NULL;
    gridPoints = NULL;
    color_array = NULL;
    points_for_draw = NULL;
    normalsToCells = NULL;
    drawIndexes = NULL;
    cnt_cells = cnt_points = cnt_users = 0;
    stepNmb = 1;
    *((int*)&color_live) = 0;
    *((int*)&color_dead) = ~0;
    *((int*)&color_grid) = 51<<8;

    listGrid = 1;
    maxNeighbors = 8;
    defaultProbabilities();
}

void Figure::toFile(FILE * F)
{
    fwrite(&maxNeighbors,4,1,F);
    fwrite(probabilities_live,(maxNeighbors+1)*4,1,F);
    fwrite(probabilities_dead,(maxNeighbors+1)*4,1,F);
    fwrite(&cnt_cells,4,1,F);
    int key = 1, s = cnt_cells/32+1;
    int * A = new int[s];
    for (int i = 0; i<s; i++) A[i] = 0;
    int * a = A;
    for (int i = 0; i<cnt_cells; i++)
    {
        if (cells[i].livingStatusNow) (*a) |= key;
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
    fread(probabilities_live,(maxNeighbors+1)*4,1,F);
    fread(probabilities_dead,(maxNeighbors+1)*4,1,F);
    fread(&cnt,4,1,F);
    clearMap();
    if (cnt!=cnt_cells)
        return;
    int key = 0, a;
    for (int i = 0; i<cnt_cells; i++)
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
    delete cells->neighbors;
    delete cells;
    delete gridPoints;
    delete ActiveCellNow;
    delete ActiveCellNext;
    delete points;
    delete drawIndexes;
    delete normalsToCells;
    delete points_for_draw;
    delete color_array;
    delete grid_colors;
}
void Figure::defaultProbabilities()
{
    for (int i = 0; i < 9; i++)
    {
        probabilities_live[i] = 0;
        probabilities_dead[i] = RAND_MAX;
    }
    all_prob_live_bool = all_prob_dead_bool = all_prob_bool = true;
    probabilities_live[3] = RAND_MAX;
    probabilities_dead[2]=probabilities_dead[3]=0;
}

void Figure::setProbabilities(double * p_live, double * p_dead)
{
    for (int i = 0; i<9; i++)
    {
        probabilities_live[i] = floor(p_live[i]*RAND_MAX+0.2);
        probabilities_dead[i] = floor(p_dead[i]*RAND_MAX+0.2);
    }
    calcAllProbBool();
}
void Figure::calcAllProbBool()
{
    all_prob_bool = true;
    for (int i = 0; i<9; i++)
    {
        if ((probabilities_live[i]!=0 && probabilities_live[i]!=RAND_MAX) ||
            (probabilities_dead[i]!=0 && probabilities_dead[i]!=RAND_MAX) )
        all_prob_bool = false;
    }
    if (!all_prob_bool)
    {
        for (int i = 0; i<cnt_cells; i++)
            (ActiveCellNow[i] = &cells[i])->step_flag = stepNmb;
        cnt_act_now = cnt_cells;
        refresh();
    }
}

void Figure::getProbabilities(double * p_live, double * p_dead)
{
    for (int i = 0; i<9; i++)
    {
        p_live[i] = 1.*probabilities_live[i]/RAND_MAX;
        p_dead[i] = 1.*probabilities_dead[i]/RAND_MAX;
    }
}

void Figure::selectAndPlus(fpoint p1, fpoint p_1, bool plus_on, Model * M)
{
    float min_r = 1000000;
    int min_ind = -1;
    for (int i = 0; i<cnt_cells; i++)
    {
        float r1 = normalsToCells[i*4+3] & (p1 - points_for_draw[i*4]);
        float r2 = normalsToCells[i*4+3] & (p_1 - points_for_draw[i*4]);
        if (r1*r2<0)
        {
            fpoint ip = p1-(p_1-p1)*(r1/(r2-r1));
            for (int j = 0; j<4; j++)
            {
                if ( (( (points_for_draw[i*4+(j+1)%4] - points_for_draw[i*4+j]) ^
                        (ip - points_for_draw[i*4+j])) & normalsToCells[i*4+3]) < 0)
                    goto m5;
            }
            float R = (ip-p1).abs();
            if (R<min_r)
            {
                min_r = R;
                min_ind = i;
            }
        }
       
        m5: {}
    }
    if (min_ind!=-1)
    {
        if (M == NULL)
        {
            if (plus_on)
                plus(min_ind);
            else
                minus(min_ind);
        }
        else
        {
            addModel(M,&cells[min_ind]);
        }
    }
}

int Figure::getLivingCellsCount()
{
    return cnt_users;
}

void Figure::createGrid()
{
    delete gridPoints;
    gridPoints = new GLuint[cnt_cells*8];

    len_grid_points = 0;

    for (int i = 0; i<cnt_cells; i++)
    {
        int i4 = i*4;
        GLuint * I = cells[i].indexes;
        for (int j = 0; j<4; j++)
        {
            grid_colors[i4+j] = color_grid;
            points_for_draw[i4+j] = points[I[j]];
            drawIndexes[i4+j] = i4+j;
            if (ArrayCellSides[j] & cells[i].paintSides)
            {
                gridPoints[len_grid_points++] = i4+(j+1)%4;
                gridPoints[len_grid_points++] = i4+j;
            }
            int j1 = (j+1)%4, j_1 = (j+3)%4;
            normalsToCells[i4+j] = (points[I[j1]] - points[I[j]]) ^
                    ( points[I[j_1]] - points[I[j]] );
        }

    }

   // gridToList();
}

void Figure::setLineWidth(GLfloat width)
{
    line_width = width;
    gridToList();
}


void Figure::createCells(int cnt, int cnt_pnt)
{
    if (cnt<1 || cnt_pnt<4)
        return;
    if (cells!=NULL)
    {
        delete cells->neighbors;
        delete ActiveCellNow;
        delete ActiveCellNext;
        delete drawIndexes;
        delete color_array;
        delete points_for_draw;
        delete normalsToCells;
        delete grid_colors;

    }
    cells = new Cell[cnt];
    cells->neighbors = new Cell*[cnt*maxNeighbors];
    for (int i = 1; i<cnt; i++)
    {
        cells[i].neighbors = cells->neighbors + i*maxNeighbors;
    }

    points = new fpoint[cnt_pnt];
    normalsToCells = new fpoint[cnt*4];

    points_for_draw = new fpoint[cnt*4];

    color_array = new bcolor[cnt*4];
    grid_colors = new bcolor[cnt*4];

    ActiveCellNow = new Cell*[cnt];
    ActiveCellNext = new Cell*[cnt];

    drawIndexes = new GLuint[cnt*4];
  //  nrm = new

    cnt_cells = cnt;
    cnt_points = cnt_pnt;

 //   normalsToCells = norm;

    initBegin();
}

void Figure::refresh()
{
    for (int i = 0; i<cnt_act_now; i++)
    {
        ActiveCellNow[i]->livingStatusNext= ActiveCellNow[i]->livingStatusNow;
        ActiveCellNow[i]->cnt_active_neighbors_next = ActiveCellNow[i]->cnt_active_neighbors_now;
    }
}

void Figure::plus(Cell * c)
{
    if (c->livingStatusNow)
        return;
    c->livingStatusNow = true;
    cnt_users++;
    for (int j = 0; j<c->cnt_neighbors; j++)
    {
        c->neighbors[j]->cnt_active_neighbors_now++;
        if (c->neighbors[j]->step_flag!= stepNmb)
            (ActiveCellNow[cnt_act_now++] = c->neighbors[j])->step_flag = stepNmb;
    }
    if (c->step_flag!= stepNmb)
        (ActiveCellNow[cnt_act_now++] = c)->step_flag = stepNmb;
}

void Figure::plus(int ind)
{
    plus(&cells[(ind+cnt_cells*4)%cnt_cells]);
}

void Figure::minus(Cell * c)
{
    if (!c->livingStatusNow)
        return;
    c->livingStatusNow = false;
    cnt_users--;
    for (int j = 0; j<c->cnt_neighbors; j++)
    {
        c->neighbors[j]->cnt_active_neighbors_now--;
        if (c->neighbors[j]->step_flag!= stepNmb)
            (ActiveCellNow[cnt_act_now++] = c->neighbors[j])->step_flag = stepNmb;
    }
    if (c->step_flag!= stepNmb)
        (ActiveCellNow[cnt_act_next++] = c)->step_flag = stepNmb;
}
void Figure::minus(int ind)
{
    minus(&cells[(ind+cnt_cells*4)%cnt_cells]);
}

void Figure::step()
{
    stepNmb++;
    cnt_act_next = 0;
    Cell * c;
    int j, u;

    for (int i = 0; i<cnt_act_now; i++)
    {
        c = ActiveCellNow[i];
        if (c->livingStatusNow)
        {
            u = probabilities_dead[c->cnt_active_neighbors_now];
            if (u!=0 && (u == RAND_MAX || rand()<u))
            {
                c->livingStatusNext = false;
                cnt_users--;
                for (j = 0; j<c->cnt_neighbors; j++)
                {
                    c->neighbors[j]->cnt_active_neighbors_next--;
                    if (c->neighbors[j]->step_flag!= stepNmb)
                        (ActiveCellNext[cnt_act_next++] = c->neighbors[j])->step_flag = stepNmb;
                }
                if (c->step_flag!= stepNmb)
                    (ActiveCellNext[cnt_act_next++] = c)->step_flag = stepNmb;
            }
        }
        else
        {
            u = probabilities_live[c->cnt_active_neighbors_now];
            if (u!=0 && (u == RAND_MAX || rand()<u))
            {
                c->livingStatusNext = true;
                cnt_users++;
                for (j = 0; j<c->cnt_neighbors; j++)
                {
                    c->neighbors[j]->cnt_active_neighbors_next++;
                    if (c->neighbors[j]->step_flag!= stepNmb)
                        (ActiveCellNext[cnt_act_next++] = c->neighbors[j])->step_flag = stepNmb;
                }
                if (c->step_flag!= stepNmb)
                    (ActiveCellNext[cnt_act_next++] = c)->step_flag = stepNmb;
            }
        }

    }

    if (!all_prob_bool)
        for (int i = 0; i<cnt_act_now; i++)
        {
            u = ActiveCellNow[i]->cnt_active_neighbors_now;
            if (ActiveCellNow[i]->step_flag!=stepNmb &&
                    (probabilities_dead[u]!=0 && probabilities_dead[u]!=RAND_MAX ||
                    probabilities_live[u]!=0 && probabilities_live[u]!=RAND_MAX))
            {
                (ActiveCellNext[cnt_act_next++] = ActiveCellNow[i])->step_flag = stepNmb;
            }
        }
//    else
//    {
//        for (int i = 0; i<cnt_act_next; i++)
//            if (ActiveCellNext[i]->livingStatusNow == ActiveCellNext[i]->livingStatusNext &&
//                    ActiveCellNext[i]->cnt_active_neighbors_now == ActiveCellNext[i]->cnt_active_neighbors_next)
//            {
//                ActiveCellNext[i--] = ActiveCellNext[--cnt_act_next];
//            }
//    }

    for (int i = 0; i<cnt_act_next; i++)
    {
        ActiveCellNext[i]->livingStatusNow = ActiveCellNext[i]->livingStatusNext;
        ActiveCellNext[i]->cnt_active_neighbors_now = ActiveCellNext[i]->cnt_active_neighbors_next;
    }

    Cell ** acn = ActiveCellNow;
    ActiveCellNow = ActiveCellNext;
    ActiveCellNext = acn;
    cnt_act_now = cnt_act_next;

}

void Figure::clearMap()
{
    for (int i = 0; i<cnt_cells; i++) minus(i);
    refresh();
}

void Figure::createRandomMap(float p)
{
    for (int i = 0; i<cnt_cells; i++) if (1.0*rand()/RAND_MAX<p) plus(i);
    refresh();
}

void Figure::initBegin()
{
    cnt_act_now = cnt_act_next = cnt_users = 0;
    stepNmb = 1;
    for (int i = 0; i<cnt_cells; i++)
    {
        cells[i].paintSides = ~0;
        cells[i].livingStatusNext = cells[i].livingStatusNow = false;
        cells[i].cnt_active_neighbors_next = cells[i].cnt_active_neighbors_now = cells[i].step_flag = 0;
        cells[i].cnt_neighbors = 8;
    }

}

void Figure::drawList()
{
    glCallList(1);
}

void Figure::gridToList()
{
    if (line_width!=0.0f)
    {
        glNewList(listGrid,GL_COMPILE);
        glEnableClientState(GL_VERTEX_ARRAY);

        glLineWidth(line_width);
        glColor3f(0.0f,0.0f,1.0f);
        glVertexPointer(3, GL_FLOAT, 0, points);
        glDrawElements(GL_LINES,len_grid_points,GL_UNSIGNED_INT,gridPoints);
        glEndList();
    }
}

bool g = false;
void Figure::drawCells()
{

   // glNewList(1,GL_COMPILE);

    for (int i = 0; i<cnt_cells; i++)
    {
        int i4 = i*4;
        color_array[i4+3] = cells[i].livingStatusNow?color_live:color_dead;
        //color_array[i4+1] = color_array[i4+2] = color_array[i4+3] = color_array[i4];
    }

    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_NORMALIZE);

    glVertexPointer(3, GL_FLOAT, 0, points_for_draw);
    glNormalPointer(GL_FLOAT,0,normalsToCells);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, color_array);

    glDrawElements(GL_QUADS,cnt_cells*4,GL_UNSIGNED_INT,drawIndexes);

   // glDisableClientState(GL_NORMAL_ARRAY);
  //  if (line_width!=0.0f) glCallList(listGrid);
    if (m_gridEnable && line_width!=0.0f)
    {
        glDisable(GL_LIGHTING);
       // glDisableClientState(GL_NORMAL_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, grid_colors);
        glLineWidth(line_width);
        glDrawElements(GL_LINES,len_grid_points,GL_UNSIGNED_INT,gridPoints);
    }

        
//    glBegin(GL_LINES);
//    glColor3f(0.0f,0.0f,1.0f);
//    glLineWidth(2.0);
//    for (int i = 0; i<cnt_cells; i++)
//    {
//
//        glVertex3fv((GLfloat*)cells[i].points[0]);
//        glVertex3fv((GLfloat*)cells[i].points[1]);
//        glVertex3fv((GLfloat*)cells[i].points[1]);
//        glVertex3fv((GLfloat*)cells[i].points[2]);
//     //   glVertex3fv((GLfloat*)cells[i].points[2]);
//     //   glVertex3fv((GLfloat*)cells[i].points[3]);
//    //    glVertex3fv((GLfloat*)cells[i].points[3]);
//    //    glVertex3fv((GLfloat*)cells[i].points[0]);
//
//    }
 //   glEnd();

    
  //  glEndList();
 //   glCallList(1);
    
    
}

void Figure::drawActiveCells()
{
   if (cells == NULL) return;

    //glNewList(1,GL_COMPILE);

    glLineWidth(line_width);

    glEnableClientState(GL_VERTEX_ARRAY);


    int a = 0, b = cnt_cells*4 - 1;
    for (int i = 0; i<cnt_act_now; i++)
    {
        Cell * c = ActiveCellNow[i];
        if (cells[i].livingStatusNow)
        {
            drawIndexes[a++] = c->indexes[0];
            drawIndexes[a++] = c->indexes[1];
            drawIndexes[a++] = c->indexes[2];
            drawIndexes[a++] = c->indexes[3];
        }
        else
        {
            drawIndexes[b--] = c->indexes[3];
            drawIndexes[b--] = c->indexes[2];
            drawIndexes[b--] = c->indexes[1];
            drawIndexes[b--] = c->indexes[0];
        }

    }
    glColor3fv((GLfloat*)&color_live);
    glVertexPointer(3, GL_FLOAT, 0, points);
    glDrawElements(GL_QUADS, a, GL_UNSIGNED_INT, drawIndexes);


    glColor3fv((GLfloat*)&color_dead);
    glDrawElements(GL_QUADS, cnt_cells*4 - 1 - b, GL_UNSIGNED_INT, drawIndexes+b+1);

  //  glColor3f(0.0f,0.0f,1.0f);
   // glDrawElements(GL_LINES,len_grid_points,GL_UNSIGNED_INT,gridPoints);
}


