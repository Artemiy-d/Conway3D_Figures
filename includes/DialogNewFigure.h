#ifndef DIALOGNEWFIGURE_H
#define	DIALOGNEWFIGURE_H

#include <QDialog>

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
private:
    QLabel * m_labelCombo;
    QComboBox * m_combo;
    GroupBoxCustom * m_grpBoxSizes, *m_grpBoxParams;
    QLabel * m_labelSize[3], * m_labelOffset;
    QSpinBox * m_spinBoxSize[3], * m_spinBoxOffset;
    QPushButton * m_buttonExit, * m_buttonOK;
    QCheckBox * m_checkBoxDefaultSettings;
    QWidget * m_widgets[15];
    int m_widgetsCount;

    Scene3D * m_s3d;

    bool m_result;
public:


    DialogNewFigure(Scene3D * _s3d);
   // DialogNewFigure(const DialogNewFigure& orig);
    virtual ~DialogNewFigure();



public slots:
    void pressOK();
    void selectingFigure(int _index);
    void setLang();

    virtual int exec();
};

#endif	/* DIALOGNEWFIGURE_H */

