/* 
 * File:   DialogOfCreation.h
 * Author: artyom
 *
 * Created on 9 Ноябрь 2011 г., 21:40
 */

#ifndef DIALOGOFCREATING_H
#define	DIALOGOFCREATING_H

#include <QDialog>

class QLabel;
class QComboBox;
class MyGroupBox;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QWidget;

class Scene3D;


class DialogOfCreation : public QDialog
{
    Q_OBJECT
private:
    QLabel * comboLabel;
    QComboBox * Combo;
    MyGroupBox * sizesBox, *paramBox;
    QLabel * size_label[3], * offset_label;
    QSpinBox * size_spin[3], * offset_spin;
    QPushButton * buttExit, * buttOK;
    QCheckBox * checkDef;
    QWidget * widgets[15];
    int w_count;

public:
    Scene3D * s3d;
    bool result;
    DialogOfCreation();
   // DialogOfCreation(const DialogOfCreation& orig);
    virtual ~DialogOfCreation();

public slots:
    void pressOK();
    void selectingFigure(int index);
    void setLang();


};

#endif	/* DIALOGOFCREATING_H */

