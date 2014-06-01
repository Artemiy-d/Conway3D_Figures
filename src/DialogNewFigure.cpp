#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

#include "GroupBoxCustom.h"
#include "Scene3D.h"
#include "DialogNewFigure.h"
#include "LanguageManager.h"

DialogNewFigure::DialogNewFigure(Scene3D * _s3d)
{
    m_s3d = _s3d;
    m_result = false;
    m_widgetsCount = 0;
    m_widgets[m_widgetsCount++] = m_labelCombo = new QLabel(this);
    m_labelCombo->move(10,10);
    
    m_widgets[m_widgetsCount++] = m_combo = new QComboBox(this);
    m_combo->move(10,30);
    connect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(selectingFigure(int)) );

    m_widgets[m_widgetsCount++] = m_checkBoxDefaultSettings = new QCheckBox(this);

    m_grpBoxSizes = new GroupBoxCustom();
   
    for (int i = 0; i < 3; i++)
    {
        m_widgets[m_widgetsCount++] = m_labelSize[i] = new QLabel(m_grpBoxSizes);
        m_labelSize[i]->move(15,25+i*50);

        m_spinBoxSize[i] = new QSpinBox(m_grpBoxSizes);
        m_spinBoxSize[i]->setRange(1,10000);
        m_spinBoxSize[i]->setValue(10);
        m_spinBoxSize[i]->move(15,45+i*50);
    }
    m_widgets[m_widgetsCount++] = m_labelOffset = new QLabel(m_grpBoxSizes);
    m_labelOffset->move(110,25);

    m_spinBoxOffset = new QSpinBox(m_grpBoxSizes);
    m_spinBoxOffset->move(110,45);
    m_spinBoxOffset->setRange(-1000,1000);

    m_grpBoxSizes->setParent(this);

    m_widgets[m_widgetsCount++] = m_buttonExit = new QPushButton(this);
    
    
    connect(m_buttonExit,SIGNAL(pressed()),this,SLOT(close()));

    m_widgets[m_widgetsCount++] = m_buttonOK = new QPushButton(this);
    
    connect(m_buttonOK,SIGNAL(pressed()),this,SLOT(pressOK()));
    
    connect(&LNG,SIGNAL(set_lang()),this,SLOT(setLang()) );
    setLang();
    this->setFixedSize(m_grpBoxSizes->pos().x()+m_grpBoxSizes->width()+10,m_buttonExit->pos().y()+m_buttonExit->height()+15);
    m_combo->setCurrentIndex(0);
    selectingFigure(0);
}   

void DialogNewFigure::setLang()
{
    this->setWindowTitle(LNG["creating_figure"]);
    m_labelCombo->setText(LNG["type_figure"]);
    m_combo->clear();

    m_combo->addItem(LNG["plane"]);
    m_combo->addItem(LNG["torus"]);
    m_combo->addItem(LNG["parallelepiped"]);
    m_combo->addItem(LNG["ellipsoid"]);

    m_checkBoxDefaultSettings->setText(LNG["default_settings"]);
    m_grpBoxSizes->setText(LNG["geometry"]);
    m_labelOffset->setText(LNG["offset"]);
    m_buttonExit->setText(LNG["exit"]);
    m_buttonOK->setText(LNG["create"]);
    for (int i = 0; i<3; i++)
        m_labelSize[i]->setText(LNG["size"] + " " + QString::number(i + 1));
    for (int i = 0; i<m_widgetsCount; i++)
        m_widgets[i]->adjustSize();
    m_checkBoxDefaultSettings->move(m_combo->pos().x(),m_combo->pos().y() + m_combo->height()+10);
    m_grpBoxSizes->setGeometry(m_combo->pos().x(),m_checkBoxDefaultSettings->pos().y()+m_checkBoxDefaultSettings->height()+10,220,200);
    m_buttonExit->move(15,m_grpBoxSizes->pos().y()+m_grpBoxSizes->height()+10);
    m_buttonOK->move(20+m_buttonExit->width(),m_buttonExit->pos().y());

}

void DialogNewFigure::selectingFigure(int _index)
{
    switch (_index)
    {
        case 0:
            m_labelSize[2]->setEnabled(false);
            m_spinBoxSize[2]->setEnabled(false);
            m_labelOffset->setEnabled(false);
            m_spinBoxOffset->setEnabled(false);
            break;
        case 1:
            m_labelSize[2]->setEnabled(false);
            m_spinBoxSize[2]->setEnabled(false);
            m_labelOffset->setEnabled(true);
            m_spinBoxOffset->setEnabled(true);
            break;
        case 2: case 3:
            m_labelSize[2]->setEnabled(true);
            m_spinBoxSize[2]->setEnabled(true);
            m_labelOffset->setEnabled(false);
            m_spinBoxOffset->setEnabled(false);
            break;
    }
}

void DialogNewFigure::pressOK()
{
    if (m_combo->currentIndex()<0)
        return;
    int p[] = {m_spinBoxSize[0]->value(), m_spinBoxSize[1]->value(), m_spinBoxSize[2]->value()};
    bool c_s = !m_checkBoxDefaultSettings->isChecked();
    switch (m_combo->currentIndex())
    {
        case 0:
            m_s3d->createFigure(figSurface,p,NULL,c_s);
            break;
        case 1:
            p[2] = m_spinBoxOffset->value();
            m_s3d->createFigure(figTorus,p,NULL,c_s);
            break;
        case 2:
            m_s3d->createFigure(figParallelepiped,p,NULL,c_s);
            break;
        case 3:
            m_s3d->createFigure(figEllipsoid,p,NULL,c_s);
            break;
    }

    m_result = true;
    this->close();
}
DialogNewFigure::~DialogNewFigure()
{

}

int DialogNewFigure::exec()
{
    QDialog::exec();
    return (int)m_result;
}

