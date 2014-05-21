/* 
 * File:   DialogOfCreation.cpp
 * Author: artyom
 * 
 * Created on 9 Ноябрь 2011 г., 21:40
 */

#include "otherGuiClasses.h"
#include "Scene3D.h"
#include "DialogOfCreation.h"
#include "Languages.h"

DialogOfCreation::DialogOfCreation()
{
    result = false;
    w_count = 0;
    widgets[w_count++] = comboLabel = new QLabel(this);
    comboLabel->move(10,10);
    
    widgets[w_count++] = Combo = new QComboBox(this);
    Combo->move(10,30);
    connect(Combo,SIGNAL(currentIndexChanged(int)),this,SLOT(selectingFigure(int)) );

    widgets[w_count++] = checkDef = new QCheckBox(this);

    sizesBox = new MyGroupBox();
   
    for (int i = 0; i<3; i++)
    {
        widgets[w_count++] = size_label[i] = new QLabel(sizesBox);
        size_label[i]->move(15,25+i*50);

        size_spin[i] = new QSpinBox(sizesBox);
        size_spin[i]->setRange(1,10000);
        size_spin[i]->setValue(10);
        size_spin[i]->move(15,45+i*50);
    }
    widgets[w_count++] = offset_label = new QLabel(sizesBox);
    offset_label->move(110,25);

    offset_spin = new QSpinBox(sizesBox);
    offset_spin->move(110,45);
    offset_spin->setRange(-1000,1000);

    sizesBox->setParent(this);

    widgets[w_count++] = buttExit = new QPushButton(this);
    
    
    connect(buttExit,SIGNAL(pressed()),this,SLOT(close()));

    widgets[w_count++] = buttOK = new QPushButton(this);
    
    connect(buttOK,SIGNAL(pressed()),this,SLOT(pressOK()));
    
    connect(&LNG,SIGNAL(set_lang()),this,SLOT(setLang()) );
    setLang();
    this->setFixedSize(sizesBox->pos().x()+sizesBox->width()+10,buttExit->pos().y()+buttExit->height()+15);
    Combo->setCurrentIndex(0);
    selectingFigure(0);
}   

void DialogOfCreation::setLang()
{
    this->setWindowTitle(LNG["creating_figure"]);
    comboLabel->setText(LNG["type_figure"]);
    Combo->clear();

    Combo->addItem(LNG["plane"]);
    Combo->addItem(LNG["torus"]);
    Combo->addItem(LNG["parallel"]);
    Combo->addItem(LNG["ellip"]);

    checkDef->setText(LNG["default_sett"]);
    sizesBox->setText(LNG["geometry"]);
    offset_label->setText(LNG["offset"]);
    buttExit->setText(LNG["exit"]);
    buttOK->setText(LNG["create"]);
    for (int i = 0; i<3; i++) size_label[i]->setText(LNG["size"]+" "+QString::number(i+1));
    for (int i = 0; i<w_count; i++) widgets[i]->adjustSize();
    checkDef->move(Combo->pos().x(),Combo->pos().y() + Combo->height()+10);
    sizesBox->setGeometry(Combo->pos().x(),checkDef->pos().y()+checkDef->height()+10,220,200);
    buttExit->move(15,sizesBox->pos().y()+sizesBox->height()+10);
    buttOK->move(20+buttExit->width(),buttExit->pos().y());

}

void DialogOfCreation::selectingFigure(int index)
{
    switch (Combo->currentIndex())
    {
        case 0:
            size_label[2]->setEnabled(false);
            size_spin[2]->setEnabled(false);
            offset_label->setEnabled(false);
            offset_spin->setEnabled(false);
            break;
        case 1:
            size_label[2]->setEnabled(false);
            size_spin[2]->setEnabled(false);
            offset_label->setEnabled(true);
            offset_spin->setEnabled(true);
            break;
        case 2: case 3:
            size_label[2]->setEnabled(true);
            size_spin[2]->setEnabled(true);
            offset_label->setEnabled(false);
            offset_spin->setEnabled(false);
            break;
    }
}

void DialogOfCreation::pressOK()
{
    if (Combo->currentIndex()<0) return;
    int p[] = {size_spin[0]->value(), size_spin[1]->value(), size_spin[2]->value()};
    bool c_s = !checkDef->isChecked();
    switch (Combo->currentIndex())
    {
        case 0:
            s3d->createFigure(figSurface,p,NULL,c_s);
            break;
        case 1:
            p[2] = offset_spin->value();
            s3d->createFigure(figTorus,p,NULL,c_s);
            break;
        case 2:
            s3d->createFigure(figParallelepiped,p,NULL,c_s);
            break;
        case 3:
            s3d->createFigure(figEllipsoid,p,NULL,c_s);
            break;
    }

    result = true;
    this->close();
}
DialogOfCreation::~DialogOfCreation()
{

}

