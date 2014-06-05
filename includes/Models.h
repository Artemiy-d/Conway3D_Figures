#ifndef MODELES_H
#define	MODELES_H

class Model
{
public:
    typedef int Index;
public:

    Model(Index _sz);
    Model(const Model& _orig);
    Model(const char * _fn);

    virtual ~Model();

    bool saveToFile(const char * _fn) const;
    bool openFromFile(const char * _fn);


    void setCellFilled(Index _i, Index _j, bool _value = true);
    virtual void createField(Index _sz);
    bool isCellFilled(Index _i, Index _j) const;
    void rotate(Index _nmb = 1);
    void mirrorX();
    void mirrorY();

    virtual Model * clone() const;
    Model& operator = (const Model& _m);
    int getSize() const;
protected:
    bool * m_cells;
    Index m_square;
    Index m_size;
};


class ModelPlaner : public Model
{
public:
  ModelPlaner();
};

class ModelZSymbol : public Model
{
public:
  ModelZSymbol(Index _s = 5);
};

class ModelXSymbol : public Model
{
public:
  ModelXSymbol(Index _s = 5);
};

class ModelRect : public Model
{
public:
  ModelRect(Index _a = 5);
  ModelRect(Index _a, Index _b);
};

class ModelShip : public Model
{
public:
  ModelShip(Index _a = 5);
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


#endif	/* MODELES_H */

