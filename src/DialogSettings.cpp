/* 
 * File:   DialogSettings.cpp
 * Author: artyom
 * 
 * Created on 11 Ноябрь 2011 г., 18:11
 */

#include "DialogSettings.h"
#include "Languages.h"
#include "complect_headers.h"
#include "Scene3D.h"


ProbabilitySpinBox::ProbabilitySpinBox(QWidget * parent) : QDoubleSpinBox(parent)
{
    setRange(0,1);
    setSingleStep(0.001);
    newVal(0);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(newVal(double)));
}


void ProbabilitySpinBox::setParent(QWidget * parent)
{
    label.setParent(parent);
    QDoubleSpinBox::setParent(parent);
}

void ProbabilitySpinBox::setGeometry(int x, int y, int w, int h)
{
    label.move(x + 3,y - 18);
    QDoubleSpinBox::setGeometry(x, y, w, h);
}

void ProbabilitySpinBox::setText(const QString & text)
{
    label.setText(text);
}

void ProbabilitySpinBox::newVal(double val)
{
    QPalette p = this->palette();
    if (val == 0)
        p.setColor(QPalette::Text,Qt::blue);
    else if (val == this->maximum())
        p.setColor(QPalette::Text,QColor(0, 100, 0));
    else
        p.setColor(QPalette::Text,Qt::black);
    this->setPalette(p);
}

void possGroup::refresh()
{
    int w_box = (width() - xMrgn - 5)/cnt;
    for (int i = 0; i < cnt; i++)
        boxes[i].setGeometry(xMrgn + i*w_box, yMrgn+20, w_box-2, 25);
}


possGroup::possGroup()
{
    xMrgn = 15;
    yMrgn = 20;
    for (int i = 0; i < cnt; i++)
    {
        boxes[i].setParent(this);
        boxes[i].setRange(0,1);
        boxes[i].setSingleStep(0.01);
        boxes[i].setDecimals(3);
        boxes[i].setText("N: " + QString::number(i));
    }
    this->refresh();
}

double * possGroup::getValues()
{
    for (int i = 0; i < cnt; i++)
        values[i] = boxes[i].value();
    return values;
}
void possGroup::setValues(double * values)
{
    for (int i = 0; i < cnt; i++)
        boxes[i].setValue(values[i]);
}
void possGroup::setGeometry(int x, int y, int w, int h)
{
    MyGroupBox::setGeometry(x, y, w, h);
    this->refresh();
}
    //~possGroup() {}

DialogSettings::DialogSettings()
{
    int w = 700;
    group_live = new possGroup();
    group_dead = new possGroup();
    group_live->setGeometry(0, 5, w, 100);
   
    group_dead->setGeometry(0, 105, w, 100);
    group_live->setParent(this);
    group_dead->setParent(this);

    buttApply = new QPushButton(this);
    buttApply->move(10,210);
    connect(buttApply, SIGNAL(clicked()), this, SLOT(apply()));

    buttOK = new QPushButton(this);
    connect(buttOK, SIGNAL(clicked()), this, SLOT(apply()));
    connect(buttOK, SIGNAL(clicked()), this, SLOT(close()));

    buttCancel = new QPushButton(this);
    connect(buttCancel, SIGNAL(clicked()), this, SLOT(close()));

    buttDefault = new QPushButton(this);
    connect(buttDefault, SIGNAL(clicked()), this, SLOT(setDefault()));

    
    connect(&LNG, SIGNAL(set_lang()), this, SLOT(setLang()) );
    setLang();
    this->setFixedSize(w, buttCancel->pos().y()+buttCancel->height()+10);
}

void DialogSettings::setLang()
{
    this->setWindowTitle(LNG["modeling_settings"]);
    group_dead->setText(LNG["probabilities_d"]);
    group_live->setText(LNG["probabilities_r"]);
    buttDefault->setText(LNG["set_def"]);
    buttCancel->setText(LNG["cancel"]);
    buttOK->setText(LNG["ok"]);
    buttApply->setText(LNG["apply"]);
    buttCancel->adjustSize();
    buttOK->adjustSize();
    buttDefault->adjustSize();
    buttOK->move(buttApply->pos().x() + buttApply->width() + 10, 210);
    buttCancel->move(buttOK->pos().x() + buttOK->width() + 10, 210);
    buttDefault->move(40+buttCancel->width()+buttCancel->pos().x(), buttCancel->pos().y());
}

void DialogSettings::setDefault()
{
    s3d->getFigure()->defaultProbabilities();
    fromFigure();
}


DialogSettings::~DialogSettings()
{
    delete buttApply;
    delete buttOK;
    delete buttCancel;
    delete group_dead;
    delete group_live;
    delete buttDefault;
}

void DialogSettings::apply()
{
    s3d->getFigure()->setProbabilities(group_live->getValues(),group_dead->getValues());
}
void DialogSettings::fromFigure()
{
    double p_live[9],p_dead[9];
    s3d->getFigure()->getProbabilities(p_live,p_dead);
    group_live->setValues(p_live);
    group_dead->setValues(p_dead);
}
void DialogSettings::showEvent(QShowEvent * /* e */)
{
    fromFigure();
}
