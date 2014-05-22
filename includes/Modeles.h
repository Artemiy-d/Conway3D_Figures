#ifndef MODELES_H
#define	MODELES_H

#include "StringMap.h"

class Model
{
public:

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
    Model& operator = (const Model& M);
    int getSize() const;
protected:
    int all_size;
    int size_m;
    bool * cellEnable;
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

extern StringMap <Model*> modelCollection;
extern StringMap <Model*> currentModelCollection;
extern QString strPen;

#endif	/* MODELES_H */

