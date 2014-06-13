#ifndef DIALOGNEWFIGURE_H
#define	DIALOGNEWFIGURE_H

#include <QDialog>

#include "NewFigureOptions.h"

class QLabel;
class QComboBox;
class GroupBoxCustom;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QWidget;

class Scene3D;



class DialogNewFigure : public QDialog
{
    Q_OBJECT
public:

    DialogNewFigure(Scene3D * _s3d);
    virtual ~DialogNewFigure();


public slots:
    void pressOK();
    void selectingFigure(int _index);
    void setLang();

    virtual int exec();

private:
    Scene3D * m_s3d;
    bool m_result;

    QLabel * m_labelCombo;
    QComboBox * m_combo;
    NewFigureOptions::OptionsBase * m_options;
    QPushButton * m_buttonExit, * m_buttonOK;
    QCheckBox * m_checkBoxDefaultSettings;
    QWidget * m_widgets[15];
    int m_widgetsCount;

};

#endif	/* DIALOGNEWFIGURE_H */

