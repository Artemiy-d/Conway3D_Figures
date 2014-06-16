#include "Models.h"

#include "Figure.h"
#include "FileManager.h"


Model::Model(Index _sz)
    : m_cells(NULL),
      m_square(0),
      m_size(0)
{
    createField(_sz);
}

Model::Model(const char * _fn)
    : m_cells(NULL),
      m_square(0),
      m_size(0)
{
    openFromFile(_fn);
}

Model::Model(const Model& _orig)
    : m_cells(NULL),
      m_square(0),
      m_size(0)
{
    operator = (_orig);
}

Model::~Model()
{
    delete m_cells;
}

void Model::setCellFilled(Index _i, Index _j, bool _value)
{
    m_cells[_i * m_size + _j] = _value;
}

void Model::createField(Index _sz)
{
    if (_sz < 1 || _sz == m_size)
        return;
    bool * newCells = new bool[m_square = _sz * _sz];
    for (Index i = 0; i < m_square; ++i)
        newCells[i] = false;
    if (m_cells != NULL)
    {
        Index minSize = std::min(_sz, m_size);
        for (Index i = 0; i < minSize; ++i)
            for (Index j = 0; j < minSize; ++j)
                newCells[i * _sz + j] = m_cells[i * m_size + j];
        delete m_cells;
    }
    m_cells = newCells;
    m_size = _sz;
}

bool Model::isCellFilled(Index _i, Index _j) const
{
    return m_cells[_i * m_size + _j];
}

void Model::mirrorX()
{
    Index s = m_size / 2;
    for (Index i = 0; i < s; ++i)
        for (Index j = 0; j < m_size; ++j)
        {
            bool b = m_cells[i * m_size + j];
            m_cells[i * m_size + j] = m_cells[(m_size - i - 1) * m_size + j];
            m_cells[(m_size - i - 1) * m_size + j] = b;
        }
}

void Model::mirrorY()
{
    Index s = m_size / 2;
    for (Index i = 0; i < s; ++i)
        for (Index j = 0; j < m_size; ++j)
        {
            bool b = m_cells[j * m_size + i];
            m_cells[j * m_size + i] = m_cells[(m_size - j - 1) * m_size + i];
            m_cells[(m_size - j - 1) * m_size + i] = b;
        }
}

Model * Model::clone() const
{
    return new Model(*this);
}

void Model::rotate(int _nmb)
{
    int nmb = (_nmb % 4 + 4) % 4;
    if (m_cells == NULL)
        return;
    bool * v = new bool[m_square];
    int k;
    for (Index i = 0; i < m_size; ++i)
        for (Index j = 0; j < m_size; ++j)
        {
            int a = i, b = j;
            for (k = 0; k < nmb; k++)
            {
                int c = a;
                a = m_size - b - 1;
                b = c;
            }
            v[a * m_size + b] = m_cells[i * m_size + j];
        }
    bool * ce = m_cells;
    m_cells = v;
    delete ce;
}
Model& Model::operator = (const Model& _m)
{
    createField(_m.m_size);
    for (Index i = 0; i < m_square; ++i)
        m_cells[i] = _m.m_cells[i];
    return *this;
}

int Model::getSize() const
{
    return m_size;
}

bool Model::saveToFile(const char * _fn) const
{
    FileManager::Writer writer(_fn);

    if ( !writer.isOpen() )
        return false;

    writer.openTag("Model");
    writer.writeData("Field", m_cells, m_square);
    writer.closeTag();

    return true;
}
bool Model::openFromFile(const char * _fn)
{
    FileManager::Reader reader( _fn );

    if ( !reader.isOpen() )
        return false;

    if ( !reader.openTag( "Model" ) )
        return false;

    unsigned int size = 0;
    if ( !reader.openData( "Field", size ) )
        return false;

    createField( sqrt( size ) );
    if ( !reader.readData( m_cells ) )
        return false;

    reader.closeTag();

    return true;
}

///--------------------------------------------------


ModelPlaner::ModelPlaner() : Model(3)
{
    setCellFilled(0, 0);
    setCellFilled(0, 1);
    setCellFilled(0, 2);
    setCellFilled(1, 0);
    setCellFilled(2, 1);
}

ModelZSymbol::ModelZSymbol(Index _s) : Model(_s)
{
    for (Index i = 0; i < _s; ++i)
    {
        setCellFilled(_s - 1, i);
        setCellFilled(i, i);
        setCellFilled(0, i);
    }
}

ModelXSymbol::ModelXSymbol(Index _s) : Model(_s)
{
    for (Index i = 0; i < _s; ++i)
    {
        setCellFilled(i, i);
        setCellFilled(_s - i - 1, i);
    }
}

ModelRect::ModelRect(Index _a) : Model(_a)
{
    for (Index i = 0; i < _a; ++i)
        for (Index j = 0; j < _a; ++j)
            setCellFilled(i, j);
}

ModelRect::ModelRect(Index _a, Index _b) : Model( std::max(_a, _b) )
{
    for (Index i = 0; i < _a; ++i)
        for (Index j = 0; j < _b; ++j)
            setCellFilled(i, j);
}

ModelShip::ModelShip(Index _a) : Model(_a)
{
    for (Index i = 1; i < _a; ++i)
        setCellFilled(0, i);
    setCellFilled(1, 0);
    setCellFilled(3, 0);
    setCellFilled(1, _a - 1);
    setCellFilled(2, _a - 1);
    setCellFilled(3, _a - 2);
    for (Index i = 2; i < _a - 3; ++i)
        setCellFilled(4, i);
}

ModelPentadecatron::ModelPentadecatron() : Model(10)
{
    setCellFilled(1, 0);
    setCellFilled(1, 1);
    setCellFilled(0, 2);
    setCellFilled(2, 2);
    setCellFilled(1, 3);
    setCellFilled(1, 4);
    setCellFilled(1, 5);
    setCellFilled(1, 6);
    setCellFilled(0, 7);
    setCellFilled(2, 7);
    setCellFilled(1, 8);
    setCellFilled(1, 9);
}


ModelAcorn::ModelAcorn() : Model(7) 
{
    setCellFilled(0, 0);
    setCellFilled(0, 1);
    setCellFilled(0, 4);
    setCellFilled(0, 5);
    setCellFilled(0, 6);
    setCellFilled(0, 1);
    setCellFilled(1, 3);
    setCellFilled(2, 1);
}

