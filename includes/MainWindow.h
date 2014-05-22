#ifndef WIDG_H
#define	WIDG_H

#include <QAction>
#include <QMainWindow>
#include <QList>

class OpenAction : public QAction
{
    Q_OBJECT
public:
    QString file_name;
    OpenAction(const QString &text, const QString & fn, QObject * parent)
        : QAction(text, parent),
          file_name(fn)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onClick()) );
    }
private slots:
    void onClick()
    {
        emit fileSelected(file_name);
    }
signals:
    void fileSelected(QString fn);
};



class DialogSettings;
class DialogNewFigure;
class DialogAbout;
class DialogTemplates;
class Scene3D;
class QPushButton;
class MyGroupBox;
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
    static bool isFileValid(const QString & fn);
    void resizeEvent(QResizeEvent * E);
    void resize();
    virtual ~MainWindow();

private:
    void startStopNames();
    bool createOpenMenuTreeRec(QMenu * menu, const QString &path, int it = 0);
    void createOpenTree();
    void saveFileTo(const QString & fn);
    void keyPressEvent(QKeyEvent* E);

public slots:
    void setComboModels();
    void buttonStartClicked();
    void sliderVelValueChanged(int value);
    void setSettingsVisible(bool value);
    void openFile(const QString & fn);
    void openFile();
    void saveFileAs();
    void saveFile();
    void setLang();
    void createNewFigure();
    void setDrawingEnable(int on);
    void actionLanguageClicked();
    void changeDrawModel(const QString& name);
signals:
    void sign();
    //void setFullScreen
  //  void menuBar(QAction* arg1);
private:
    DialogSettings * dSettings;
    DialogNewFigure * dialogNewFigure;
    DialogAbout * dAbout;
    DialogTemplates * dTemplates;
    Scene3D *s3d;
    QPushButton * ButtonStart, * ButtonStep, * ButtonClear, * ButtonAgar, * ButtonRnd;
    MyGroupBox * panelSettings;
    QSlider * sliderVelocity;
    QLabel * labelV, * lavelTypeDraw;
    QComboBox * comboModels;
    QCheckBox * checkAnimation, * checkGrid, * checkStatistic, *checkDraw, *checkAxes;

    QAction * actAbout, * actStep, * actSettings, * actPanelSettings, * actFullScreen,
         * actRandomMap, * actClearMap, * actNewFigure, * actOpen, * actSaveAs, * actExit,
         * actStartStop, * actAgar, * actSave, * actTemplates;

    QMenu * menuFile, * menuEdit, * menuView, * menuModeling, * menuHelp, * menuLang, * menuOpenFinded;
    QWidget * m_widgets[50];
    int m_widgetsCount;
    int m_panelWidth;
    QList<QMenu*> menuList;
    QList<OpenAction*> actList;
};



#endif	/* WIDG_H */

