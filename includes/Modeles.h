/* 
 * File:   Modeles.h
 * Author: artyom
 *
 * Created on 3 Ноябрь 2011 г., 20:05
 */

#ifndef MODELES_H
#define	MODELES_H
//#include "stddef.h"
#include "MyCollection.h"

class Model
{
protected:
    int all_size;

public:
    bool * cellEnable;
    int size_m;
    static bool isFileValid(char * fn);
    Model(int sz);
    Model(Model& orig);
    Model(char * fn);
    bool saveToFile(char * fn);
    bool openFromFile(char * fn);
    virtual ~Model();
    void setCell(int i, int j, bool value = true);
    virtual void createField(int sz);
    bool isCell(int i, int j);
    void rotate(int nmb = 1);
    void mirrorX();
    void mirrorY();
    Model& operator = (Model& M);
};


class modelPlaner : public Model
{
public:
  modelPlaner();
};

class modelZSymbol : public Model
{
public:
  modelZSymbol(int s = 5);
};

class modelXSymbol : public Model
{
public:
  modelXSymbol(int s = 5);
};

class modelRect : public Model
{
public:
  modelRect(int a = 5);
  modelRect(int a, int b);
};

class modelShip : public Model
{
public:
  modelShip(int a = 5);
};

class modelPentadecatron : public Model
{
public:
  modelPentadecatron();
};

class modelAcorn : public Model
{
public:
  modelAcorn();
};

extern MyCollection <Model*> modelCollection;
extern MyCollection <Model*> currentModelCollection;
extern QString strPen;

#endif	/* MODELES_H */

