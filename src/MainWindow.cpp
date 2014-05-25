#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>
#include <QKeyEvent>

#include "MainWindow.h"
#include "Figure.h"

#include "Scene3D.h"
#include "DialogNewFigure.h"
#include "DialogAbout.h"
#include "DialogSettings.h"
#include "DialogTemplates.h"
#include "DialogSaveFigure.h"
#include "otherGuiClasses.h"
#include "FigureClasses.h"


#include "LanguageManager.h"


int versionSaving = 0;

QString file_save_name, path_to_save;

MainWindow::MainWindow()
{
    //QEvent::wi
//    int * x = new int[67000];
//    unsigned int p0 = (unsigned int)(x);
//    unsigned int p1 = (unsigned int)(x+65535);
//    unsigned int p2 = (unsigned int)(x+65537);

    m_widgetsCount = 0;
    m_panelWidth = 200;
    m_s3d = new Scene3D();
    m_s3d->setParent(this);
   // m_s3d->
    m_dialogNewFigure = new DialogNewFigure();
    m_dialogNewFigure->m_s3d = m_s3d;

    m_dialogTemplates = new DialogTemplates();
    connect(m_dialogTemplates, SIGNAL(newActive()),this,SLOT(setComboModels()));

    m_dialogAbout = new DialogAbout();
    m_dialogSettings = new DialogSettings(m_s3d);
    
    m_panelSettings = new MyGroupBox();

    m_sliderVelocity = new QSlider(m_panelSettings);
    m_sliderVelocity->setOrientation(Qt::Horizontal);
    m_sliderVelocity->setGeometry(10,40,m_panelWidth-20,20);
    m_sliderVelocity->setMaximum(100);
    m_sliderVelocity->setValue(30);
    
    m_widgets[m_widgetsCount++] = m_labelVelocity = new QLabel(m_panelSettings);
   // m_labelVelocity->set
    m_labelVelocity->move(m_sliderVelocity->pos().x(),m_sliderVelocity->pos().y()-20);
    connect(m_sliderVelocity,SIGNAL(valueChanged(int)),this,SLOT(sliderVelValueChanged(int)));

    m_widgets[m_widgetsCount++] = m_checkBoxAnimation = new QCheckBox(m_panelSettings);
    m_checkBoxAnimation->setChecked(true);
    m_checkBoxAnimation->move(10,70);
    connect(m_checkBoxAnimation,SIGNAL(stateChanged(int)),m_s3d,SLOT(setAnimationEnable(int)));

    m_widgets[m_widgetsCount++] = m_checkBoxGrid = new QCheckBox(m_panelSettings);
    m_checkBoxGrid->move(10,90);
    connect(m_checkBoxGrid,SIGNAL(stateChanged(int)),m_s3d,SLOT(setGridEnable(int)));

    m_widgets[m_widgetsCount++] = m_checkBoxAxes = new QCheckBox(m_panelSettings);
    m_checkBoxAxes->setChecked(true);
    m_checkBoxAxes->move(10,110);
    connect(m_checkBoxAxes,SIGNAL(stateChanged(int)),m_s3d,SLOT(setAxesVisible(int)));

    m_widgets[m_widgetsCount++] = m_checkBoxStatistic = new QCheckBox(m_panelSettings);
    m_checkBoxStatistic->setChecked(true);
    m_checkBoxStatistic->move(10,130);
    connect(m_checkBoxStatistic,SIGNAL(stateChanged(int)),m_s3d,SLOT(setStatisticVisible(int)));

    m_widgets[m_widgetsCount++] = m_checkBoxDraw = new QCheckBox(m_panelSettings);
    m_checkBoxDraw->move(10,150);
    connect(m_checkBoxDraw,SIGNAL(stateChanged(int)),m_s3d,SLOT(setDrawingEnable(int)));
    connect(m_checkBoxDraw,SIGNAL(stateChanged(int)),this,SLOT(setDrawingEnable(int)));
    

    m_widgets[m_widgetsCount++] = m_lavelDrawType = new QLabel(m_panelSettings);
    m_lavelDrawType->move(15,175);

    m_comboBoxModels = new QComboBox(m_panelSettings);
    m_comboBoxModels->move(10,195);
    connect(m_comboBoxModels, SIGNAL(currentIndexChanged(const QString&) ), this, SLOT(changeDrawModel(const QString&) ));

    m_widgets[m_widgetsCount++] = m_buttonStart = new QPushButton(m_panelSettings);
    m_buttonStart->move(20,m_checkBoxDraw->pos().y()+100);
    connect(m_buttonStart,SIGNAL(clicked()),this,SLOT(buttonStartClicked()));

    m_widgets[m_widgetsCount++] = m_buttonStep = new QPushButton(m_panelSettings);
    m_buttonStep->move(20,m_buttonStart->pos().y()+m_buttonStart->height()+5);
    connect(m_buttonStep,SIGNAL(clicked()),m_s3d,SLOT(stepFigure()));

    m_widgets[m_widgetsCount++] = m_buttonClear = new QPushButton(m_panelSettings);
    m_buttonClear->move(20,m_buttonStep->pos().y()+m_buttonStep->height()+20);
    connect(m_buttonClear,SIGNAL(clicked()),m_s3d,SLOT(clearMap()));

    m_widgets[m_widgetsCount++] = m_buttonAgar = new QPushButton(m_panelSettings);
    m_buttonAgar->move(20,m_buttonClear->pos().y()+m_buttonClear->height()+5);
    connect(m_buttonAgar,SIGNAL(clicked()),m_s3d,SLOT(createAgar()));

    m_widgets[m_widgetsCount++] = m_buttonRnd = new QPushButton(m_panelSettings);
    m_buttonRnd->move(20,m_buttonAgar->pos().y()+m_buttonAgar->height()+5);
    connect(m_buttonRnd,SIGNAL(clicked()),m_s3d,SLOT(createRandomMap()));
    //list.

    menuFile = this->menuBar()->addMenu(LNG["file"]);
    //menuFile->addActions(*(QList<QAction*>*)&list);
        actNewFigure = menuFile->addAction(LNG["new_fig"]);
        connect(actNewFigure,SIGNAL(triggered()), this,SLOT(createNewFigure()));
        menuFile->addSeparator();
        actOpen = menuFile->addAction(LNG["open"]);
        connect(actOpen,SIGNAL(triggered()),this,SLOT(openFile()));

        actSave = menuFile->addAction(LNG["save"]);
        connect(actSave,SIGNAL(triggered()),this,SLOT(saveFile()));

        actSaveAs = menuFile->addAction(LNG["save_as"]);
        connect(actSaveAs,SIGNAL(triggered()),this,SLOT(saveFileAs()));
        menuFile->addSeparator();
        actExit = menuFile->addAction(LNG["exit"]);
        connect(actExit,SIGNAL(triggered()),this,SLOT(close()));
    
    menuEdit = this->menuBar()->addMenu(LNG["edit"]);

        actClearMap = menuEdit->addAction(LNG["clear_map"]);
        connect(actClearMap,SIGNAL(triggered()),m_s3d,SLOT(clearMap()));

        actAgar = menuEdit->addAction(LNG["create_agar"]);
        connect(actAgar,SIGNAL(triggered()),m_s3d,SLOT(createAgar()));
        actRandomMap = menuEdit->addAction(LNG["create_random"]);
        connect(actRandomMap,SIGNAL(triggered()),m_s3d,SLOT(createRandomMap()));
        menuEdit->addSeparator();
        actTemplates = menuEdit->addAction(LNG["templates"]);
        connect(actTemplates,SIGNAL(triggered()),m_dialogTemplates,SLOT(exec()));
        
    menuView = this->menuBar()->addMenu(LNG["view"]);
        actFullScreen = menuView->addAction(LNG["full_scr"]);
        connect(actFullScreen,SIGNAL(triggered()),m_s3d,SLOT(setFullScreen()));
        actPanelSettings = menuView->addAction(LNG["settings_panel"]);
        actPanelSettings->setCheckable(true);
        actPanelSettings->setChecked(true);
        connect(actPanelSettings,SIGNAL(triggered(bool)),this,SLOT(setSettingsVisible(bool)));
      //  if (LNG.Count()>1)
        {
            menuLang = menuView->addMenu(LNG["languages"]);
            const QList<QString> & languages = LNG.getLanguagesList();
            for ( QList<QString>::const_iterator it = languages.begin(); it != languages.end(); ++it )
            {
                QAction * action = menuLang->addAction( *it );
                connect( action, SIGNAL(triggered()), this, SLOT( actionLanguageClicked ) );
            }
        }
    menuModeling = this->menuBar()->addMenu(LNG["modeling"]);
        actSettings = menuModeling->addAction(LNG["modeling_sett"]);
        connect(actSettings,SIGNAL(triggered()),m_dialogSettings,SLOT(exec()));
        menuModeling->addSeparator();
        actStep = menuModeling->addAction(LNG["step"]);
        connect(actStep,SIGNAL(triggered()),m_s3d,SLOT(stepFigure()));
        actStartStop = menuModeling->addAction(LNG["start"]);
        
        connect(actStartStop,SIGNAL(triggered()),this,SLOT(buttonStartClicked()));

    menuHelp = this->menuBar()->addMenu(LNG["help"]);
        actAbout = menuHelp->addAction(LNG["about"]);
        connect(actAbout,SIGNAL(triggered()),m_dialogAbout,SLOT(exec()));
 
       
    m_panelSettings->setParent(this);

    menuOpenFinded = NULL;
    createOpenTree();
    setLang();
   // delete menuOpenFinded;
 //   menuOpenFinded->setVisible(false);
    path_to_save = QDir::homePath()+tr("/Save Conway");
    QDir dir(QDir::homePath());
    dir.mkdir(tr("Save Conway"));
    setComboModels();
    //    QMessageBox M1;
     //   M1.exec();
    setDrawingEnable(0);
}

void MainWindow::createOpenTree()
{
  //  foreach (OpenAction * a, actList) { delete a; }
   // foreach (QMenu * m, menuList) { delete m; }
    actList.clear();
    menuList.clear();

    QString P = QDir::homePath()+tr("/Save Conway");
    delete menuOpenFinded;
    menuOpenFinded = new QMenu(LNG["open_finded"],this);
    menuFile->insertMenu(actSave,menuOpenFinded);
    if (!createOpenMenuTreeRec(menuOpenFinded,P))
        menuOpenFinded = NULL;
}

bool MainWindow::createOpenMenuTreeRec(QMenu * _menu, const QString & _path, int _it)
{
    if (_it == 3)
        return false;
    bool ret = false;

    QDir d(_path);
    QStringList filt(tr("*.cf"));
    QStringList fileList = d.entryList(filt,QDir::Files);

    if (fileList.count()!=0)
    {
        foreach (QString s, fileList)
        {
            QString absPath = d.absoluteFilePath(s);
            if (isFileValid(absPath))
            {
                OpenAction * a = new OpenAction(s, absPath, NULL);
                connect(a, SIGNAL(fileSelected(const QString &)), this, SLOT(openFile(const QString &)));
                _menu->addAction(a);
                actList += a;
                ret = true;
            }
        }
    }

    QStringList dirList = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if (dirList.count() != 0)
    {
        foreach (QString s, dirList)
        {
            QMenu * m = _menu->addMenu(s);
            ret |= createOpenMenuTreeRec(m, d.absoluteFilePath(s), _it + 1);
        }
    }

    if (!ret)
        delete _menu;
    else
        menuList += _menu;

    return ret;
}

QString createMenuText(const QString & _begin, const QString & _end, int _len = 20)
{
    QString s;
    if (_begin.length() + _end.length() < _len)
        s.fill(' ', _len - _begin.length() - _end.length());
    return _begin + s + _end;
}

void MainWindow::setLang()
{
    startStopNames();
    this->setWindowTitle(LNG["conway_game"]);
    m_panelSettings->setText(LNG["sett"]);
    m_labelVelocity->setText(LNG["speed"]);
    m_lavelDrawType->setText(LNG["type_of_draw"]);
    m_checkBoxAnimation->setText(LNG["animation"]);
    m_checkBoxGrid->setText(LNG["grid"]);
    m_checkBoxAxes->setText(LNG["axes"]);
    m_checkBoxStatistic->setText(LNG["statistic"]);
    m_checkBoxDraw->setText(LNG["drawing"]);
    m_buttonStep->setText(LNG["step"]);
    m_buttonClear->setText(LNG["clear_map"]);
    m_buttonAgar->setText(LNG["create_agar"]);
    m_buttonRnd->setText(LNG["random_map"]);

    menuFile->setTitle(LNG["file"]);

        actNewFigure->setText(createMenuText(LNG["new_fig"],tr("  ctrl+N")));
        actOpen->setText(createMenuText(LNG["open"],tr("  ctrl+O")));
        if (menuOpenFinded!=NULL) menuOpenFinded->setTitle(LNG["open_finded"]);
        actSave->setText(createMenuText(LNG["save"],tr("  ctrl+S")));
        actSaveAs->setText(LNG["save_as"]);
        actExit->setText(LNG["exit"]);
    menuEdit->setTitle(LNG["edit"]);
        actClearMap->setText(LNG["clear_map"]);
        actAgar->setText(LNG["create_agar"]);
        actRandomMap->setText(LNG["create_random"]);
        actTemplates->setText(LNG["templates"]);
    menuView->setTitle(LNG["view"]);
        actFullScreen->setText(LNG["full_scr"]);
        actPanelSettings->setText(LNG["settings_panel"]);
        if (LNG.count()>1)
            menuLang->setTitle(LNG["languages"]);

    menuModeling->setTitle(LNG["modeling"]);
        actSettings->setText(LNG["modeling_sett"]);
        actStep->setText(LNG["step"]);
        
    menuHelp->setTitle(LNG["help"]);
        actAbout->setText(LNG["about"]);

    for (int i = 0; i < m_widgetsCount; i++)
        m_widgets[i]->adjustSize();
}

void MainWindow::setComboModels()
{
    m_comboBoxModels->clear();
    m_comboBoxModels->addItem(strPen);
    for ( StringMap<Model*>::iterator it = currentModelCollection.begin(); it != currentModelCollection.end(); ++it )
        m_comboBoxModels->addItem( it.key() );
    m_comboBoxModels->adjustSize();
}

void MainWindow::setDrawingEnable(int on)
{
    m_comboBoxModels->setEnabled(on!=0);
    m_lavelDrawType->setEnabled(on!=0);
}

void MainWindow::createNewFigure()
{
    m_dialogNewFigure->exec();
    if (m_dialogNewFigure->m_result)
    {
        file_save_name = "";
    }
}

void MainWindow::actionLanguageClicked()
{
    QAction * action = ( QAction* )QObject::sender();
    LNG.setCurrentLanguage( action->text() );
    setLang();
}

void MainWindow::changeDrawModel(const QString& name)
{
    m_s3d->setCurrentModel( name == strPen
        ? NULL
        : currentModelCollection[name] );
}

void MainWindow::resize()
{
    int menuHeight = this->menuBar()->height();
    m_s3d->setGeometry(0,menuHeight,this->width()-m_panelWidth,this->height()-menuHeight);
    m_panelSettings->setGeometry(this->width()-m_panelWidth,menuHeight,m_panelWidth,this->height()-menuHeight);
}

void MainWindow::setSettingsVisible(bool value)
{
    m_panelSettings->setVisible(value);
    m_panelWidth = value ? 200 : 0;
    resize();
}

double getVelocity(QSlider * slider)
{
    return pow(1. * (slider->maximum() - slider->value()) / slider->maximum(), 1) * 1000;
}

void MainWindow::startStopNames()
{
    if (m_s3d != NULL && m_s3d->isExecute())
    {
        actStartStop->setText(LNG["stop"]);
        m_buttonStart->setText(LNG["stop"]);
    }
    else
    {
        actStartStop->setText(LNG["start"]);
        m_buttonStart->setText(LNG["start"]);
    }
}

void MainWindow::buttonStartClicked()
{
    if (m_s3d->isExecute())
        m_s3d->stop();
    else
        m_s3d->start( getVelocity( m_sliderVelocity ) );
    startStopNames();

}
void MainWindow::sliderVelValueChanged(int)
{
    m_s3d->setInterval( getVelocity( m_sliderVelocity ) );
}

void MainWindow::resizeEvent(QResizeEvent * /*e*/)
{
    resize();
}

void MainWindow::openFile(const QString & _fn)
{
    if (!isFileValid(_fn))
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle(LNG["file_not_correct"]);
        messageBox.setText(LNG["cannot_open_this_file"]);
        messageBox.exec();
        return;
    }
    FILE * file;
    if (!_fn.isNull() && (file = fopen(_fn.toLocal8Bit().data(),"rb") ) != NULL )
    {
        int vers, type;
        fread(&vers, 4, 1, file);
        fread(&type, 4, 1, file);

        switch ((FigureType)type)
        {
            case figTorus:
                m_s3d->setFigure( new Torus(file) );
                break;
            case figSurface:
                m_s3d->setFigure( new Surface(file) );
                break;
            case figEllipsoid:
            case figParallelepiped:
                m_s3d->setFigure( new Ellipsoid(file) );
                break;
        }
        fclose(file);
        file_save_name = _fn;
    }
}

void MainWindow::openFile()
{
   QString fn = QFileDialog::getOpenFileName(this,
              tr("Open Figure"),
              path_to_save,
              tr("Figure files (*.cf);;All files (*.*)"),
              0,
              QFileDialog::DontUseNativeDialog );
   if (!fn.isNull())
       openFile(fn);
}

void MainWindow::saveFile()
{
    if (file_save_name.isNull() || file_save_name == "")
        saveFileAs();
    else
        saveFileTo(file_save_name);
}

bool MainWindow::isFileValid(const QString & fn)
{
    FILE * F;
    if (!fn.isNull() && (F = fopen(fn.toLocal8Bit().data(),"rb") )!=NULL )
    {
        char s;
        int sum =-1000;
        fseek(F,0,SEEK_END);
        int sz = ftell(F) - 4;
        if (sz <= 0)
        {
            fclose(F);
            return false;
        }
        rewind(F);
        while (sz--)
        {
            fread(&s,1,1,F);
            sum+=s;
        }
        fread(&sz,4,1,F);
        fclose(F);
        if (sum == sz)
            return true;
    }
    return false;
}

void MainWindow::saveFileTo(const QString & fn)
{
    FILE * F;
    if (!fn.isNull() && (F = fopen(fn.toLocal8Bit().data(),"w+b") )!=NULL )
    {
        fwrite(&versionSaving,4,1,F);
        m_s3d->getFigure()->toFile(F);
        int sz = ftell(F);
        rewind(F);
        char s;
        int sum = -1000;
        while (sz--)
        {
            fread(&s,1,1,F);
            sum+=s;
        }
        fwrite(&sum,4,1,F);
        fclose(F);
        file_save_name = fn;
        createOpenTree();
    }
}

void MainWindow::saveFileAs()
{
    if (m_s3d == NULL || m_s3d->getFigure() == NULL )
    {
        QMessageBox messageBox;
        messageBox.setText(tr("Figure hasn't been created") );
        messageBox.setWindowTitle(tr("Cannot save"));
        messageBox.exec();
        return;
    }
    DialogSaveFigure dialog;
    QString fn = dialog.execSave();
    if (!fn.isNull())
        saveFileTo(fn);
}

void MainWindow::keyPressEvent(QKeyEvent * _e)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        switch (_e->key())
        {
            case Qt::Key_N:
                m_dialogNewFigure->exec();
                break;
            case Qt::Key_S:
                saveFile();
                break;
            case Qt::Key_O:
                openFile();
                break;
        }
    }

}

MainWindow::~MainWindow()
{
    delete m_dialogSettings;
    delete m_dialogNewFigure;
    delete m_dialogAbout;
    delete m_buttonStart;
    delete m_buttonStep;

    delete m_sliderVelocity;
    delete m_labelVelocity;
    delete m_checkBoxAnimation;
    delete m_checkBoxGrid;
    delete m_checkBoxStatistic;
    delete m_checkBoxDraw;
    delete m_checkBoxAxes;
    delete m_panelSettings;
    delete m_s3d;
    delete m_dialogTemplates;
 //       QMessageBox X;
  //  X.exec();
  //  DELETE(menuHelp);

    //QAction * actStartStop;
}
