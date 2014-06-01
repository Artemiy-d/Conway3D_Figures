#ifndef DIALOGSETTINGS_H
#define	DIALOGSETTINGS_H

#include <QLabel>
#include <QDoubleSpinBox>
#include <QDialog>

#include "GroupBoxCustom.h"


class QPushButton;

class Scene3D;

class ProbabilitySpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:

    ProbabilitySpinBox(QWidget * _parent = 0);

    void setParent(QWidget * _parent);

    void setGeometry(int _x, int _y, int _w, int _h);

    void setText(const QString & _text);
public slots:
    void newVal(double _val);

private:
    QLabel m_label;

    static const int s_decimalsCount;
    static const double s_singleStep;
};

class ProbabilitiesGroup : public GroupBoxCustom
{
    Q_OBJECT
private:
    int m_xMrgn, m_yMrgn;
    static const int cnt = 9;
    double m_values[cnt];
    void refresh();
public:

    ProbabilitySpinBox m_boxes[cnt];
    ProbabilitiesGroup();

    double * getValues();
    void setValues(double * _values);
    void setGeometry(int _x, int _y, int _w, int _h);
    ~ProbabilitiesGroup() {}
};

class DialogSettings : public QDialog
{
    Q_OBJECT
private:
    void showEvent(QShowEvent * _e);
    void fromFigure();
public:
    DialogSettings(Scene3D * _s3d);
    virtual ~DialogSettings();
public slots:
    void apply();
    void setDefault();
    void setLang();

private:
    ProbabilitiesGroup * m_groupLive, * m_groupDead;
    QPushButton * m_buttonApply, * m_buttonOK, * m_buttonCancel, * m_buttonDefault;
    Scene3D * m_s3d;
};

#endif	/* DIALOGSETTINGS_H */

