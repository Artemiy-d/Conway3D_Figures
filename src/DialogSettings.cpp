#include <QPushButton>
#include <QDebug>

#include <assert.h>

#include "DialogSettings.h"
#include "LanguageManager.h"
#include "Scene3D.h"
#include "Figure.h"



const int ProbabilitySpinBox::s_decimalsCount = 6;
const double ProbabilitySpinBox::s_singleStep = 0.01;

ProbabilitySpinBox::ProbabilitySpinBox(QWidget * _parent)
    : QDoubleSpinBox(_parent),
      m_label(_parent)
{
    setRange(0, 1);
    setSingleStep( s_singleStep );
    setDecimals( s_decimalsCount );
    newVal(0);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(newVal(double)));
}


void ProbabilitySpinBox::setParent(QWidget * _parent)
{
    m_label.setParent(_parent);
    QDoubleSpinBox::setParent(_parent);
}

void ProbabilitySpinBox::setGeometry(int _x, int _y, int _w, int _h)
{
    m_label.move(_x + 3, _y - 18);
    QDoubleSpinBox::setGeometry(_x, _y, _w, _h);
}

void ProbabilitySpinBox::setText(const QString & _text)
{
    m_label.setText(_text);
}

void ProbabilitySpinBox::newVal(double _val)
{
    QPalette p = palette();
    if (_val == 0)
        p.setColor(QPalette::Text, Qt::blue);
    else if (_val == maximum())
        p.setColor(QPalette::Text, QColor(0, 100, 0));
    else
        p.setColor(QPalette::Text, Qt::black);
    setPalette(p);
}

const int ProbabilitiesGroup::s_xMrgn = 15;
const int ProbabilitiesGroup::s_yMrgn = 20;

ProbabilitiesGroup::ProbabilitiesGroup(QWidget * _parent, size_t _count)
    : GroupBoxCustom(_parent)
{
    m_boxes.resize(_count);
    for (size_t i = 0; i < _count; ++i)
    {
        m_boxes[i] = new ProbabilitySpinBox(this);
        m_boxes[i]->setText("N: " + QString::number(i));
    }
    refresh();
}

void ProbabilitiesGroup::refresh()
{
    int boxWidth = (width() - s_xMrgn - 5) / m_boxes.size();
    for (int i = 0; i < m_boxes.size(); ++i)
        m_boxes[i]->setGeometry(s_xMrgn + i * boxWidth, s_yMrgn + 20, boxWidth - 2, 25);
}


ProbabilitiesGroup::Values ProbabilitiesGroup::getValues()
{
    Values result;
    result.reserve( m_boxes.size() );
    foreach ( ProbabilitySpinBox * box, m_boxes )
    {
        result.push_back(box->value());
    }
    return result;

}
void ProbabilitiesGroup::setValues(const Values & _values)
{
    assert( _values.size() == m_boxes.size() );

    for ( int i = 0; i < _values.size(); ++i )
        m_boxes[i]->setValue( _values[i] );
}
void ProbabilitiesGroup::setGeometry(int _x, int _y, int _w, int _h)
{
    GroupBoxCustom::setGeometry(_x, _y, _w, _h);
    refresh();
}
    //~ProbabilitiesGroup() {}

DialogSettings::DialogSettings(Scene3D * _s3d)
    : QDialog(),
      m_s3d(_s3d)
{
    static const int defaultWidth = 700;

    m_groupLive = new ProbabilitiesGroup( this, Figure::Probabilities::s_defaultCount );
    m_groupDead = new ProbabilitiesGroup( this, Figure::Probabilities::s_defaultCount );
    m_groupLive->setGeometry(0, 5, defaultWidth, 100);
   
    m_groupDead->setGeometry(0, 105, defaultWidth, 100);

    m_buttonApply = new QPushButton(this);
    m_buttonApply->move(10, 210);
    connect(m_buttonApply, SIGNAL(clicked()), this, SLOT(apply()));

    m_buttonOK = new QPushButton(this);
    connect(m_buttonOK, SIGNAL(clicked()), this, SLOT(apply()));
    connect(m_buttonOK, SIGNAL(clicked()), this, SLOT(close()));

    m_buttonCancel = new QPushButton(this);
    connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(close()));

    m_buttonDefault = new QPushButton(this);
    connect(m_buttonDefault, SIGNAL(clicked()), this, SLOT(setDefault()));

    
    connect(&LNG, SIGNAL(set_lang()), this, SLOT(setLang()) );
    setLang();
    setFixedSize(defaultWidth, m_buttonCancel->pos().y() + m_buttonCancel->height() + 10);
}

void DialogSettings::setLang()
{
    setWindowTitle(LNG["modelling_settings"]);
    m_groupDead->setText(LNG["probabilities_death"]);
    m_groupLive->setText(LNG["probabilities_revival"]);
    m_buttonDefault->setText(LNG["set_default"]);
    m_buttonCancel->setText(LNG["cancel"]);
    m_buttonOK->setText(LNG["ok"]);
    m_buttonApply->setText(LNG["apply"]);
    m_buttonCancel->adjustSize();
    m_buttonOK->adjustSize();
    m_buttonDefault->adjustSize();
    m_buttonOK->move(m_buttonApply->pos().x() + m_buttonApply->width() + 10, 210);
    m_buttonCancel->move(m_buttonOK->pos().x() + m_buttonOK->width() + 10, 210);
    m_buttonDefault->move(40 + m_buttonCancel->width() + m_buttonCancel->pos().x(), m_buttonCancel->pos().y());
}

void DialogSettings::setDefault()
{
    m_s3d->getFigure()->setProbabilities();
    fromFigure();
}


DialogSettings::~DialogSettings()
{
    delete m_buttonApply;
    delete m_buttonOK;
    delete m_buttonCancel;
    delete m_groupDead;
    delete m_groupLive;
    delete m_buttonDefault;
}

void DialogSettings::apply()
{ 
    const ProbabilitiesGroup::Values & d = m_groupDead->getValues();
    const ProbabilitiesGroup::Values & l = m_groupLive->getValues();
    assert( d.size() == l.size() );

    m_s3d->getFigure()->setProbabilities( Figure::Probabilities( l.data(), d.data(),
                                                                 l.size() ) );
}
void DialogSettings::fromFigure()
{
    ProbabilitiesGroup::Values valuesDeath;
    ProbabilitiesGroup::Values valuesLive;
    valuesDeath.resize( Figure::Probabilities::s_defaultCount );
    valuesLive.resize( Figure::Probabilities::s_defaultCount );

    m_s3d->getFigure()->getProbabilities().toDouble( valuesLive.data(), valuesDeath.data(), Figure::Probabilities::s_defaultCount);
    m_groupLive->setValues(valuesLive);
    m_groupDead->setValues(valuesDeath);
}
void DialogSettings::showEvent(QShowEvent * /* _e */)
{
    fromFigure();
}
