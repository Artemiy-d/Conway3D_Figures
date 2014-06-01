#include <QPushButton>

#include "DialogSettings.h"
#include "LanguageManager.h"
#include "Scene3D.h"


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
    QPalette p = this->palette();
    if (_val == 0)
        p.setColor(QPalette::Text, Qt::blue);
    else if (_val == this->maximum())
        p.setColor(QPalette::Text, QColor(0, 100, 0));
    else
        p.setColor(QPalette::Text, Qt::black);
    this->setPalette(p);
}

void ProbabilitiesGroup::refresh()
{
    int boxWidth = (width() - m_xMrgn - 5) / cnt;
    for (int i = 0; i < cnt; i++)
        m_boxes[i].setGeometry(m_xMrgn + i * boxWidth, m_yMrgn + 20, boxWidth - 2, 25);
}




ProbabilitiesGroup::ProbabilitiesGroup()
    : GroupBoxCustom(),
      m_xMrgn(15),
      m_yMrgn(20)
{
    for (int i = 0; i < cnt; i++)
    {
        m_boxes[i].setParent(this);
        m_boxes[i].setText("N: " + QString::number(i));
    }
    this->refresh();
}

double * ProbabilitiesGroup::getValues()
{
    for (int i = 0; i < cnt; i++)
        m_values[i] = m_boxes[i].value();
    return m_values;
}
void ProbabilitiesGroup::setValues(double * _values)
{
    for (int i = 0; i < cnt; i++)
        m_boxes[i].setValue(_values[i]);
}
void ProbabilitiesGroup::setGeometry(int _x, int _y, int _w, int _h)
{
    GroupBoxCustom::setGeometry(_x, _y, _w, _h);
    this->refresh();
}
    //~ProbabilitiesGroup() {}

DialogSettings::DialogSettings(Scene3D * _s3d)
    : QDialog(),
      m_s3d(_s3d)
{
    int w = 700;
    m_groupLive = new ProbabilitiesGroup();
    m_groupDead = new ProbabilitiesGroup();
    m_groupLive->setGeometry(0, 5, w, 100);
   
    m_groupDead->setGeometry(0, 105, w, 100);
    m_groupLive->setParent(this);
    m_groupDead->setParent(this);

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
    this->setFixedSize(w, m_buttonCancel->pos().y() + m_buttonCancel->height() + 10);
}

void DialogSettings::setLang()
{
    this->setWindowTitle(LNG["modelling_settings"]);
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
    m_s3d->getFigure()->defaultProbabilities();
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
    m_s3d->getFigure()->setProbabilities(m_groupLive->getValues(), m_groupDead->getValues());
}
void DialogSettings::fromFigure()
{
    double pLive[9], pDead[9];
    m_s3d->getFigure()->getProbabilities(pLive, pDead);
    m_groupLive->setValues(pLive);
    m_groupDead->setValues(pDead);
}
void DialogSettings::showEvent(QShowEvent * /* e */)
{
    fromFigure();
}
