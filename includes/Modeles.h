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

    virtual Model * clone() const;
    Model& operator = (const Model& _m);
    int getSize() const;
protected:
    bool * m_cells;
    int m_square;
    int m_size;
};


class ModelPlaner : public Model
{
public:
  ModelPlaner();
};

class ModelZSymbol : public Model
{
public:
  ModelZSymbol(int _s = 5);
};

class ModelXSymbol : public Model
{
public:
  ModelXSymbol(int _s = 5);
};

class ModelRect : public Model
{
public:
  ModelRect(int _a = 5);
  ModelRect(int _a, int _b);
};

class ModelShip : public Model
{
public:
  ModelShip(int _a = 5);
};

class ModelPentadecatron : public Model
{
public:
  ModelPentadecatron();
};

class ModelAcorn : public Model
{
public:
  ModelAcorn();
};

extern StringMap <Model*> modelCollection;
extern StringMap <Model*> currentModelCollection;
extern QString strPen;

#endif	/* MODELES_H */

