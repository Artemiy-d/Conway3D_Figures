#ifndef WIDG_H
#define	WIDG_H

#include <QAction>
#include <QMainWindow>
#include <QList>

class OpenAction : public QAction
{
    Q_OBJECT
public:
    QString m_fileName;
    OpenAction(const QString &_text, const QString & _fn, QObject * _parent)
        : QAction(_text, _parent),
          m_fileName(_fn)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onClick()) );
    }
private slots:
    void onClick()
    {
        emit fileSelected(m_fileName);
    }
signals:
    void fileSelected(const QString & _fn);
};


class ModelsManager;
class DialogSettings;
class DialogNewFigure;
class DialogAbout;
class DialogTemplates;
class Scene3D;
class QPushButton;
class GroupBoxCustom;
class QSlider;
class QLabel;
class QComboBox;
class QCheckBox;
class QAction;
class QMenu;
class QWidget;
class QResizeEvent;
class QKeyEvent;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    //void paintEvent(QPaintEvent *event);
    MainWindow();
    static bool isFileValid(const QString & _fn);
    void resizeEvent(QResizeEvent * _e);
    void resize();
    virtual ~MainWindow();

private:
    void startStopNames();
    bool createOpenMenuTreeRec(QMenu * _menu, const QString &_path, int _it = 0);
    void createOpenTree();
    void saveFileTo(const QString & _fn);
    void keyPressEvent(QKeyEvent* _e);

public slots:
    void setComboModels();
    void buttonStartClicked();
    void sliderVelValueChanged(int _value);
    void setSettingsVisible(bool _value);
    void openFile(const QString & _fn);
    void openFile();
    void saveFileAs();
    void saveFile();
    void setLang();
    void createNewFigure();
    void setDrawingEnable(int _on);
    void actionLanguageClicked();
    void changeDrawModel(const QString& _name);
signals:
    void sign();
    //void setFullScreen
  //  void menuBar(QAction* arg1);
private:
    DialogSettings * m_dialogSettings;
    DialogNewFigure * m_dialogNewFigure;
    DialogAbout * m_dialogAbout;
    DialogTemplates * m_dialogTemplates;
    Scene3D * m_s3d;
    QPushButton * m_buttonStart, * m_buttonStep, * m_buttonClear, * m_buttonAgar, * m_buttonRnd;
    GroupBoxCustom * m_panelSettings;
    QSlider * m_sliderVelocity;
    QLabel * m_labelVelocity, * m_lavelDrawType;
    QComboBox * m_comboBoxModels;
    QCheckBox * m_checkBoxAnimation, * m_checkBoxGrid, * m_checkBoxStatistic, *m_checkBoxDraw, *m_checkBoxAxes;

    QAction * actAbout, * actStep, * actSettings, * actPanelSettings, * actFullScreen,
         * actRandomMap, * actClearMap, * actNewFigure, * actOpen, * actSaveAs, * actExit,
         * actStartStop, * actAgar, * actSave, * actTemplates;
    QAction * m_checkedLanguageAction;

    QMenu * menuFile, * menuEdit, * menuView, * menuModeling, * menuHelp, * menuLang, * menuOpenFinded;
    QWidget * m_widgets[50];
    int m_widgetsCount;
    int m_panelWidth;
    QList<QMenu*> menuList;
    QList<OpenAction*> actList;

    ModelsManager * m_modelsManager;
};



#endif	/* WIDG_H */

