#include "Modeles.h"
#include "Figure.h"


Model::Model(int _sz)
{
    m_cells = NULL;
    createField(_sz);
}

Model::Model(const char * _fn)
{
    m_cells = NULL;
    openFromFile(_fn);
}

Model::Model(const Model& _orig)
{
    m_cells = NULL;
    operator = (_orig);
}

Model::~Model()
{
    delete m_cells;
}

void Model::setCellFilled(int _i, int _j, bool _value)
{
    m_cells[_i * m_size + _j] = _value;
}

void Model::createField(int _sz)
{
    if (_sz < 1 || _sz == m_size)
        return;
    bool * newCells = new bool[m_square = _sz * _sz];
    for (int i = 0; i < m_square; i++)
        newCells[i] = false;
    if (m_cells != NULL)
    {
        int minSize = std::min(_sz, m_size);
        for (int i = 0; i < minSize; i++)
            for (int j = 0; j < minSize;j++)
                newCells[i * _sz + j] = m_cells[i * m_size + j];
        delete m_cells;
    }
    m_cells = newCells;
    m_size = _sz;
}

bool Model::isCellFilled(int _i, int _j) const
{
    return m_cells[_i * m_size + _j];
}

void Model::mirrorX()
{
    int s = m_size / 2;
    for (int i = 0; i < s; i++)
        for (int j = 0; j < m_size; j++)
        {
            bool b = m_cells[i * m_size + j];
            m_cells[i * m_size + j] = m_cells[(m_size - i - 1) * m_size + j];
            m_cells[(m_size - i - 1) * m_size + j] = b;
        }
}

void Model::mirrorY()
{
    int s = m_size / 2;
    for (int i = 0; i < s; i++)
        for (int j = 0; j < m_size; j++)
        {
            bool b = m_cells[j * m_size + i];
            m_cells[j * m_size + i] = m_cells[(m_size - j - 1) * m_size + i];
            m_cells[(m_size - j - 1) * m_size + i] = b;
        }
}

void Model::rotate(int _nmb)
{
    int nmb = (_nmb % 4 + 4) % 4;
    if (m_cells == NULL)
        return;
    bool * v = new bool[m_square];
    int k;
    for (int i = 0; i<m_size; i++)
        for (int j = 0; j<m_size; j++)
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
    for (int i = 0; i < m_square; i++)
        m_cells[i] = _m.m_cells[i];
    return *this;
}

int Model::getSize() const
{
    return m_size;
}

bool Model::isFileValid(const char * _fn)
{
    FILE * file;
    if ( (file = fopen(_fn, "rb") ) != NULL )
    {
        char s;
        fseek(file, 0, SEEK_END);
        int sz = ftell(file) - 4;
        int sum = -1234;
        if (sz <= 0)
        {
            fclose(file);
            return false;
        }
        rewind(file);

        while (sz--)
        {
            fread(&s, 1, 1, file);
            sum += s;
        }
        fread(&sz, 4, 1, file);
        fclose(file);

        if (sz == sum)
            return true;

    }
    return false;
}
bool Model::saveToFile(const char * _fn) const
{
    FILE * file;
    if ( (file = fopen(_fn, "w+b") ) != NULL )
    {
        fwrite(&m_size, 4, 1, file);
        fwrite(m_cells, m_square, 1, file);
        int sz = ftell(file);
        rewind(file);
        char s;
        int sum = -1234;
        while (sz--)
        {
            fread(&s, 1, 1, file);
            sum += s;
        }
        fwrite(&sum, 4, 1, file);
        fclose(file);
        return true;
    }
    return false;
}
bool Model::openFromFile(const char * _fn)
{
   // if (!isFileValid(fn)) return false;
    FILE * file;
    if ( (file = fopen(_fn, "rb") ) != NULL )
    {
        int sz;
        fread(&sz, 4, 1, file);
        createField(sz);
        fread(m_cells, m_square, 1, file);
        fclose(file);
        return true;
    }
    return false;
}

///--------------------------------------------------


ModelPlaner::ModelPlaner() : Model(3)
{
    this->setCellFilled(0, 0);
    this->setCellFilled(0, 1);
    this->setCellFilled(0, 2);
    this->setCellFilled(1, 0);
    this->setCellFilled(2, 1);
}

ModelZSymbol::ModelZSymbol(int _s) : Model(_s)
{
    for (int i = 0; i < _s; i++)
    {
        this->setCellFilled(_s - 1, i);
        this->setCellFilled(i, i);
        this->setCellFilled(0, i);
    }
}

ModelXSymbol::ModelXSymbol(int _s) : Model(_s)
{
    for (int i = 0; i < _s; i++)
    {
        this->setCellFilled(i, i);
        this->setCellFilled(_s - i - 1, i);
    }
}

ModelRect::ModelRect(int _a) : Model(_a)
{
    for (int i = 0; i < _a; i++)
        for (int j = 0; j < _a; j++)
            setCellFilled(i, j);
}

ModelRect::ModelRect(int _a, int _b) : Model( std::max(_a, _b) )
{
    for (int i = 0; i < _a; i++)
        for (int j = 0; j < _b; j++)
            setCellFilled(i, j);
}

ModelShip::ModelShip(int _a) : Model(_a)
{
    for (int i = 1; i < _a; i++)
        setCellFilled(0, i);
    this->setCellFilled(1, 0);
    this->setCellFilled(3, 0);
    this->setCellFilled(1, _a - 1);
    this->setCellFilled(2, _a - 1);
    this->setCellFilled(3, _a - 2);
    for (int i = 2; i < _a - 3; i++)
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


StringMap <Model*> modelCollection;
StringMap <Model*> currentModelCollection;
QString strPen;
