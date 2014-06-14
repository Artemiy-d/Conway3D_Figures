#ifndef FIGURECLASSES_H
#define	FIGURECLASSES_H

#include "Figure.h"

class IncludingSurface;

class BaseSurface : public Figure
{
public:
    BaseSurface();
    BaseSurface(Index _firstSideCount, Index _secondSideCount);
    void addModel(Model * _model, Index _x, Index _y, bool _refresh = true);
    void addModel(Model * _model, Cell * _cell);
    void createAgar();

    void createGridMap();
    void setPhisicSize(float _s1, float _s2);
    void plus(Index _c1, Index _c2);
    void minus(Index _c1, Index _c2);

protected:
    void createField(Index _firstSideCount, Index _secondSideCount);
    void createIncludingSurface();

protected:
    IncludingSurface * m_includingSurface;
    Index m_firstSideCount, m_secondSideCount;
    float m_firstSize, m_secondSize;
};

class Torus : public BaseSurface
{
public:
    virtual ~Torus() {}
    Torus();
    Torus(FileManager::Reader * _reader);
    Torus(Index _firstSideCount, Index _secondSideCount, int _offset_1 = 0);
    void setPhisicSize(float _s1, float _s2);
    void createField(Index _firstSideCount, Index _secondSideCount, int _offset_1 = 0);
    virtual void toFile(FileManager::Writer * _writer);
    virtual bool fromFile(FileManager::Reader * _reader);
    virtual const char * getStringType() const;

public:
    static const char * const s_stringType;

protected:
    int m_offset;
    float m_angle;
};

class Surface : public BaseSurface
{
public:
    virtual ~Surface() {}
    Surface();
    Surface(Index _firstSideCount, Index _secondSideCount);
    Surface(FileManager::Reader * _reader);
    void createField(Index _firstSideCount, Index _secondSideCount);
    void setPhisicSize(float _s, float);

    virtual void toFile(FileManager::Writer * _writer);
    virtual bool fromFile(FileManager::Reader * _reader);
    virtual const char * getStringType() const;

public:
    static const char * const s_stringType;
};

class Parallelepiped : public Figure
{
public:
    virtual ~Parallelepiped() {}
    Parallelepiped();
    Parallelepiped(FileManager::Reader * _reader);
    Parallelepiped(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount);
    void createField(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount);
    virtual void setPhisicSize(float _s, float);
    void createAgar();
    void addModel(Model * _model, Cell * _cell);
    virtual void addModel(Model *, Index, Index, bool) { }
    virtual void toFile(FileManager::Writer * _writer);
    virtual bool fromFile(FileManager::Reader * _reader);
    virtual const char * getStringType() const;


public:
    static const char * const s_stringType;
    static const size_t s_surfacesCount = 6;

protected:

    float m_size;
    Index m_firstSideCount, m_secondSideCount, m_thirdSideCount;
    IncludingSurface * m_surfaces[s_surfacesCount];

    float m_scale;
    float m_firstSize, m_secondSize, m_thirdSize;
};


class Ellipsoid : public Parallelepiped
{
public:
    Ellipsoid();
    Ellipsoid(FileManager::Reader * _reader);
    Ellipsoid(Index _firstSideCount, Index _secondSideCount, Index _thirdSideCount);

    virtual const char * getStringType() const;

    virtual void setPhisicSize(float _s, float);

public:

    static const char * const s_stringType;
};

#endif	/* FIGURECLASSES_H */

