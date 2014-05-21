/* 
 * File:   DialogSettings.h
 * Author: artyom
 *
 * Created on 11 Ноябрь 2011 г., 18:11
 */

#ifndef DIALOGSETTINGS_H
#define	DIALOGSETTINGS_H

#include <QLabel>
#include <QDoubleSpinBox>

#include "otherGuiClasses.h"


class QPushButton;

class Scene3D;

class ProbabilitySpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:

    ProbabilitySpinBox(QWidget * parent = 0);

    void setParent(QWidget * parent);

    void setGeometry(int x, int y, int w, int h);

    void setText(const QString & text);
public slots:
    void newVal(double val);

private:
    QLabel label;
};

class possGroup : public MyGroupBox
{
    Q_OBJECT
private:
    int xMrgn, yMrgn;
    static const int cnt = 9;
    double values[cnt];
    void refresh();
public:

    ProbabilitySpinBox boxes[cnt];
    possGroup();

    double * getValues();
    void setValues(double * values);
    void setGeometry(int x, int y, int w, int h);
    ~possGroup() {}
};

class DialogSettings : public QDialog
{

    Q_OBJECT
private:
    possGroup * group_live, * group_dead;
    QPushButton * buttApply, * buttOK, * buttCancel, * buttDefault;
    void showEvent(QShowEvent * E);
    void fromFigure();
public:
    Scene3D * s3d;
    DialogSettings();
    virtual ~DialogSettings();
public slots:
    void apply();
    void setDefault();
    void setLang();

};

#endif	/* DIALOGSETTINGS_H */

