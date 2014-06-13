#include <QLabel>
#include <QSpinBox>

#include "NewFigureOptions.h"
#include "GroupBoxCustom.h"
#include "FigureClasses.h"
#include "LanguageManager.h"

using namespace NewFigureOptions;


FiguresFactory::FigureCreatorExtended<Torus, OptionsTorus> s_addTorus;
FiguresFactory::FigureCreatorExtended<Surface, OptionsSurface> s_addSurface;
FiguresFactory::FigureCreatorExtended<Parallelepiped, OptionsParallelepiped> s_addParallelepiped;
FiguresFactory::FigureCreatorExtended<Ellipsoid, OptionsEllipsoid> s_addEllipsoid;

LabeledSpinboxes::LabeledSpinboxes(QWidget * _parent, size_t _paramsCount)
    : OptionsBase(_parent),
      m_paramsCount(_paramsCount)
{
    m_box = new GroupBoxCustom(this);
    m_labelsSize = new QLabel*[_paramsCount];
    m_spinBoxesSize = new QSpinBox*[_paramsCount];

    for ( size_t i = 0; i < m_paramsCount; ++i )
    {
        m_labelsSize[i] = new QLabel(this);
        m_spinBoxesSize[i] = new QSpinBox(this);

        m_spinBoxesSize[i]->setRange(1, 10000);
        m_spinBoxesSize[i]->setValue(10);
    }
}

LabeledSpinboxes::~LabeledSpinboxes()
{
    for ( size_t i = 0; i < m_paramsCount; ++i )
    {
        delete m_labelsSize[i];
        delete m_spinBoxesSize[i];
    }

    delete [] m_labelsSize;
    delete [] m_spinBoxesSize;
}


void LabeledSpinboxes::resizeEvent(QResizeEvent * /* _e */)
{
    static const int leftMargin = 15;
    static const int bottomMargin = 25;
    static const int deltaX = 100;
    static const int deltaY = 50;

    m_box->setGeometry( 0, 0, width(), height() );

    int y = bottomMargin;
    int x = leftMargin;
    for ( size_t i = 0; i < m_paramsCount; ++i, y += deltaY )
    {
        if ( y + deltaY > height() )
        {
            x += deltaX;
            y = bottomMargin;
        }

        m_labelsSize[i]->move(x, y);
        m_spinBoxesSize[i]->move(x, y + 20);
    }
}

void LabeledSpinboxes::setLang()
{
    m_box->setText( LNG["geometry"] );
    for ( size_t i = 0; i < m_paramsCount; ++i )
    {
        m_labelsSize[i]->setText( LNG["size"] + " " + QString::number(i + 1) );
        m_labelsSize[i]->adjustSize();
    }
}

OptionsTorus::OptionsTorus(QWidget * _parent) : LabeledSpinboxes(_parent, 3)
{
    m_spinBoxesSize[2]->setRange(-1000, 1000);
    m_spinBoxesSize[2]->setValue( 1 );
    setLang();
}

Figure * OptionsTorus::createFigure()
{
    return new Torus(m_spinBoxesSize[0]->value(), m_spinBoxesSize[1]->value(), m_spinBoxesSize[2]->value());
}

void OptionsTorus::setLang()
{
    for ( size_t i = 0; i < m_paramsCount - 1; ++i )
    {
        m_labelsSize[i]->setText( LNG["size"] + " " + QString::number(i + 1) );
        m_labelsSize[i]->adjustSize();
    }
    m_labelsSize[2]->setText( LNG["offset"] );
    m_labelsSize[2]->adjustSize();
    m_box->setText( LNG["geometry"] );
}

OptionsSurface::OptionsSurface(QWidget * _parent) : LabeledSpinboxes(_parent, 2)
{
    setLang();
}

Figure * OptionsSurface::createFigure()
{
    return new Surface(m_spinBoxesSize[0]->value(), m_spinBoxesSize[1]->value());
}

OptionsParallelepiped::OptionsParallelepiped(QWidget * _parent) : LabeledSpinboxes(_parent, 3)
{
    setLang();
}

Figure * OptionsParallelepiped::createFigure()
{
    return new Parallelepiped(m_spinBoxesSize[0]->value(), m_spinBoxesSize[1]->value(), m_spinBoxesSize[2]->value());
}


OptionsEllipsoid::OptionsEllipsoid(QWidget * _parent) : LabeledSpinboxes(_parent, 3)
{
    setLang();
}

Figure * OptionsEllipsoid::createFigure()
{
    return new Ellipsoid(m_spinBoxesSize[0]->value(), m_spinBoxesSize[1]->value(), m_spinBoxesSize[2]->value());
}


