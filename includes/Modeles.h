#ifndef MODELES_H
#define	MODELES_H

#include "StringMap.h"

class Model
{
public:

    static bool isFileValid(const char * _fn);

    Model(int _sz);
    Model(const Model& _orig);
    Model(const char * _fn);

    virtual ~Model();

    bool saveToFile(const char * _fn) const;
    bool openFromFile(const char * _fn);


    void setCellFilled(int _i, int _j, bool _value = true);
    virtual void createField(int _sz);
    bool isCellFilled(int _i, int _j) const;
    void rotate(int _nmb = 1);
    void mirrorX();
    void mirrorY();
    Model& operator = (const Model& _m);
    int getSize() const;
protected:
    int m_square;
    int m_size;
    bool * m_cells;
};


class modelPlaner : public Model
{
public:
  modelPlaner();
};

class modelZSymbol : public Model
{
public:
  modelZSymbol(int _s = 5);
};

class modelXSymbol : public Model
{
public:
  modelXSymbol(int _s = 5);
};

class modelRect : public Model
{
public:
  modelRect(int _a = 5);
  modelRect(int _a, int _b);
};

class modelShip : public Model
{
public:
  modelShip(int _a = 5);
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

