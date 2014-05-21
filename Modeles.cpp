/* 
 * File:   Modeles.cpp
 * Author: artyom
 * 
 * Created on 3 Ноябрь 2011 г., 20:05
 */

#include "Modeles.h"
#include "Figure.h"


Model::Model(int sz)
{
    cellEnable = NULL;
    createField(sz);
}

Model::Model(char * fn)
{
    cellEnable = NULL;
    openFromFile(fn);
}

Model::Model(Model& orig)
{
    cellEnable = NULL;
    operator = (orig);
}

Model::~Model()
{
    delete cellEnable;
}

void Model::setCell(int i, int j, bool value)
{
    cellEnable[i*size_m+j]=value;
}

void Model::createField(int sz)
{
    if (sz<1 || sz == size_m)
        return;
    bool * new_points = new bool[all_size = sz*sz];
    for (int i = 0; i<all_size; i++)
        new_points[i] = false;
    if (cellEnable != NULL)
    {
        int m_sz = std::min(sz,size_m);
        for (int i = 0; i<m_sz; i++)
            for (int j = 0; j<m_sz;j++)
                new_points[i*sz+j] = cellEnable[i*size_m+j];
        delete cellEnable;
    }
    cellEnable = new_points;
    size_m = sz;
}

bool Model::isCell(int i, int j)
{
    return cellEnable[i*size_m+j];
}

void Model::mirrorX()
{
    int s = size_m/2;
    for (int i = 0; i<s; i++)
        for (int j = 0; j<size_m; j++)
        {
            bool b = cellEnable[i*size_m+j];
            cellEnable[i*size_m+j] = cellEnable[(size_m-i-1)*size_m+j];
            cellEnable[(size_m-i-1)*size_m+j] = b;
        }
}

void Model::mirrorY()
{
    int s = size_m/2;
    for (int i = 0; i<s; i++)
        for (int j = 0; j<size_m; j++)
        {
            bool b = cellEnable[j*size_m+i];
            cellEnable[j*size_m+i] = cellEnable[(size_m-j-1)*size_m+i];
            cellEnable[(size_m-j-1)*size_m+i] = b;
        }
}

void Model::rotate(int nmb)
{
#define rtt(a,b) { int c = (a); (a)= size_m-(b)-1; (b) = (c); }
    nmb = ((nmb%4+4))%4;
    if (cellEnable == NULL)
        return;
    bool * v = new bool[all_size];
    int k;
    for (int i = 0; i<size_m; i++)
        for (int j = 0; j<size_m; j++)
        {
            int a = i, b = j;
            for (k = 0; k<nmb; k++) rtt(a,b)
            v[a*size_m+b] = cellEnable[i*size_m + j];
        }
    bool * ce = cellEnable;
    cellEnable = v;
    delete ce;
}
Model& Model::operator = (Model& M)
{
    createField(M.size_m);
    for (int i = 0; i<all_size; i++)
        cellEnable[i] = M.cellEnable[i];
    return *this;
}

bool Model::isFileValid(char* fn)
{
    FILE * F;
    if ( (F = fopen(fn,"rb") ) != NULL )
    {
        char s;
        fseek(F,0,SEEK_END);
        int sz = ftell(F)-4;
        int sum = -1234;
        if (sz<=0)
        {
            fclose(F);
            return false;
        }
        rewind(F);

        while (sz--)
        {
            fread(&s,1,1,F);
            sum += s;
        }
        fread(&sz,4,1,F);
        fclose(F);

        if (sz == sum)
            return true;

    }
    return false;
}
bool Model::saveToFile(char * fn)
{
    FILE * F;
    if ( (F = fopen(fn,"w+b") ) != NULL )
    {
        fwrite(&size_m,4,1,F);
        fwrite(cellEnable,all_size,1,F);
        int sz = ftell(F);
        rewind(F);
        char s;
        int sum = -1234;
        while (sz--)
        {
            fread(&s,1,1,F);
            sum += s;
        }
        fwrite(&sum,4,1,F);
        fclose(F);
        return true;
    }
    return false;
}
bool Model::openFromFile(char * fn)
{
   // if (!isFileValid(fn)) return false;
    FILE * F;
    if ( (F = fopen(fn,"rb") ) != NULL )
    {
        int sz;
        fread(&sz,4,1,F);
        createField(sz);
        fread(cellEnable,all_size,1,F);
        fclose(F);
        return true;
    }
    return false;
}

///--------------------------------------------------


modelPlaner::modelPlaner() : Model(3)
{
    this->setCell(0,0);
    this->setCell(0,1);
    this->setCell(0,2);
    this->setCell(1,0);
    this->setCell(2,1);
}

modelZSymbol::modelZSymbol(int s) : Model(s)
{
    for (int i = 0; i<s; i++)
    {
        this->setCell(s-1,i);
        this->setCell(i,i);
        this->setCell(0,i);
    }
}

modelXSymbol::modelXSymbol(int s) : Model(s)
{
    for (int i = 0; i<s; i++)
    {
        this->setCell(i,i);
        this->setCell(s - i - 1,i);
    }
}

modelRect::modelRect(int a) : Model(a)
{
    for (int i = 0; i<a; i++)
        for (int j = 0; j<a; j++)
            setCell(i,j);
}

modelRect::modelRect(int a, int b) : Model((a>b)?a:b)
{
    for (int i = 0; i<a; i++)
        for (int j = 0; j<b; j++)
            setCell(i,j);
}

modelShip::modelShip(int a) : Model(a)
{
    for (int i = 1; i<a; i++) setCell(0,i);
    this->setCell(1,0);
    this->setCell(3,0);
    this->setCell(1,a-1);
    this->setCell(2,a-1);
    this->setCell(3,a-2);
    for (int i = 2; i<a-3; i++)
        setCell(4,i);
}

modelPentadecatron::modelPentadecatron() : Model(10)
{
    setCell(1,0);
    setCell(1,1);
    setCell(0,2);
    setCell(2,2);
    setCell(1,3);
    setCell(1,4);
    setCell(1,5);
    setCell(1,6);
    setCell(0,7);
    setCell(2,7);
    setCell(1,8);
    setCell(1,9);
}


modelAcorn::modelAcorn() : Model(7) 
{
    setCell(0,0);
    setCell(0,1);
    setCell(0,4);
    setCell(0,5);
    setCell(0,6);
    setCell(0,1);
    setCell(1,3);
    setCell(2,1);
}


MyCollection <Model*> modelCollection;
MyCollection <Model*> currentModelCollection;
QString strPen;
