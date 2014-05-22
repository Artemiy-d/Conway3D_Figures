#include "MainWindow.h"
#include "Figure.h"

#include "complect_headers.h"
#include "Scene3D.h"
#include "DialogOfCreation.h"
#include "DialogAbout.h"
#include "DialogSettings.h"
#include "DialogTemplates.h"
#include "otherGuiClasses.h"
#include "FigureClasses.h"


#include "Languages.h"


bool init = false;
int wPanel = 200;

int version_saving = 0;
QString file_save_name, path_to_save;

MainWindow::MainWindow()
{
    //QEvent::wi
//    int * x = new int[67000];
//    unsigned int p0 = (unsigned int)(x);
//    unsigned int p1 = (unsigned int)(x+65535);
//    unsigned int p2 = (unsigned int)(x+65537);

    m_widgetsCount = 0;
    s3d = new Scene3D();
    s3d->setParent(this);
   // s3d->
    dialogCreating = new DialogOfCreation();
    dialogCreating->s3d = s3d;

    dTemplates = new DialogTemplates();
    connect(dTemplates, SIGNAL(newActive()),this,SLOT(setComboModels()));

    dAbout = new DialogAbout();
    dSettings = new DialogSettings();
    dSettings->s3d = s3d;
    
    panelSettings = new MyGroupBox();

    sliderVelocity = new QSlider(panelSettings);
    sliderVelocity->setOrientation(Qt::Horizontal);
    sliderVelocity->setGeometry(10,40,wPanel-20,20);
    sliderVelocity->setMaximum(100);
    sliderVelocity->setValue(30);
    
    m_widgets[m_widgetsCount++] = labelV = new QLabel(panelSettings);
   // labelV->set
    labelV->move(sliderVelocity->pos().x(),sliderVelocity->pos().y()-20);
    connect(sliderVelocity,SIGNAL(valueChanged(int)),this,SLOT(sliderVelValueChanged(int)));

    m_widgets[m_widgetsCount++] = checkAnimation = new QCheckBox(panelSettings);
    checkAnimation->setChecked(true);
    checkAnimation->move(10,70);
    connect(checkAnimation,SIGNAL(stateChanged(int)),s3d,SLOT(setAnimationEnable(int)));

    m_widgets[m_widgetsCount++] = checkGrid = new QCheckBox(panelSettings);
    checkGrid->move(10,90);
    connect(checkGrid,SIGNAL(stateChanged(int)),s3d,SLOT(setGridEnable(int)));

    m_widgets[m_widgetsCount++] = checkAxes = new QCheckBox(panelSettings);
    checkAxes->setChecked(true);
    checkAxes->move(10,110);
    connect(checkAxes,SIGNAL(stateChanged(int)),s3d,SLOT(setAxesVisible(int)));

    m_widgets[m_widgetsCount++] = checkStatistic = new QCheckBox(panelSettings);
    checkStatistic->setChecked(true);
    checkStatistic->move(10,130);
    connect(checkStatistic,SIGNAL(stateChanged(int)),s3d,SLOT(setStatisticVisible(int)));

    m_widgets[m_widgetsCount++] = checkDraw = new QCheckBox(panelSettings);
    checkDraw->move(10,150);
    connect(checkDraw,SIGNAL(stateChanged(int)),s3d,SLOT(setDrawingEnable(int)));
    connect(checkDraw,SIGNAL(stateChanged(int)),this,SLOT(setDrawingEnable(int)));
    

    m_widgets[m_widgetsCount++] = lavelTypeDraw = new QLabel(panelSettings);
    lavelTypeDraw->move(15,175);

    comboModels = new QComboBox(panelSettings);
    comboModels->move(10,195);
    connect(comboModels,SIGNAL(currentIndexChanged(const QString&) ),s3d,SLOT(changeDrawModel(const QString&) ));

    m_widgets[m_widgetsCount++] = ButtonStart = new QPushButton(panelSettings);
    ButtonStart->move(20,checkDraw->pos().y()+100);
    connect(ButtonStart,SIGNAL(clicked()),this,SLOT(buttonStartClicked()));

    m_widgets[m_widgetsCount++] = ButtonStep = new QPushButton(panelSettings);
    ButtonStep->move(20,ButtonStart->pos().y()+ButtonStart->height()+5);
    connect(ButtonStep,SIGNAL(clicked()),s3d,SLOT(stepFigure()));

    m_widgets[m_widgetsCount++] = ButtonClear = new QPushButton(panelSettings);
    ButtonClear->move(20,ButtonStep->pos().y()+ButtonStep->height()+20);
    connect(ButtonClear,SIGNAL(clicked()),s3d,SLOT(clearMap()));

    m_widgets[m_widgetsCount++] = ButtonAgar = new QPushButton(panelSettings);
    ButtonAgar->move(20,ButtonClear->pos().y()+ButtonClear->height()+5);
    connect(ButtonAgar,SIGNAL(clicked()),s3d,SLOT(createAgar()));

    m_widgets[m_widgetsCount++] = ButtonRnd = new QPushButton(panelSettings);
    ButtonRnd->move(20,ButtonAgar->pos().y()+ButtonAgar->height()+5);
    connect(ButtonRnd,SIGNAL(clicked()),s3d,SLOT(createRandomMap()));
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
        connect(actClearMap,SIGNAL(triggered()),s3d,SLOT(clearMap()));

        actAgar = menuEdit->addAction(LNG["create_agar"]);
        connect(actAgar,SIGNAL(triggered()),s3d,SLOT(createAgar()));
        actRandomMap = menuEdit->addAction(LNG["create_random"]);
        connect(actRandomMap,SIGNAL(triggered()),s3d,SLOT(createRandomMap()));
        menuEdit->addSeparator();
        actTemplates = menuEdit->addAction(LNG["templates"]);
        connect(actTemplates,SIGNAL(triggered()),dTemplates,SLOT(exec()));
        
    menuView = this->menuBar()->addMenu(LNG["view"]);
        actFullScreen = menuView->addAction(LNG["full_scr"]);
        connect(actFullScreen,SIGNAL(triggered()),s3d,SLOT(setFullScreen()));
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
        connect(actSettings,SIGNAL(triggered()),dSettings,SLOT(exec()));
        menuModeling->addSeparator();
        actStep = menuModeling->addAction(LNG["step"]);
        connect(actStep,SIGNAL(triggered()),s3d,SLOT(stepFigure()));
        actStartStop = menuModeling->addAction(LNG["start"]);
        
        connect(actStartStop,SIGNAL(triggered()),this,SLOT(buttonStartClicked()));

    menuHelp = this->menuBar()->addMenu(LNG["help"]);
        actAbout = menuHelp->addAction(LNG["about"]);
        connect(actAbout,SIGNAL(triggered()),dAbout,SLOT(exec()));
 
       
    panelSettings->setParent(this);

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

bool MainWindow::createOpenMenuTreeRec(QMenu * menu, const QString &path, int it)
{
    if (it == 3)
        return false;
    bool ret = false;

    QDir d(path);
    QStringList filt(tr("*.cf"));
    QStringList l_file = d.entryList(filt,QDir::Files);

    if (l_file.count()!=0)
    {
        foreach (QString s, l_file)
        {
            QString absPath = d.absoluteFilePath(s);
            if (isFileValid(absPath))
            {
                OpenAction * A = new OpenAction(s,absPath,NULL);
                connect(A,SIGNAL(fileSelected(QString)),this,SLOT(openFile(QString)));
                menu->addAction(A);
                actList += A;
                ret = true;
            }
        }
    }

    QStringList l_dir = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if (l_dir.count()!=0)
    {
        foreach (QString s, l_dir)
        {
            QMenu * M = menu->addMenu(s);
            ret |= createOpenMenuTreeRec(M,d.absoluteFilePath(s),it+1);
        }
    }

    if (!ret) delete menu;
    else menuList += menu;
    return ret;
}

static QString AddEnd(QString str, QString end, int len = 20)
{
    QString S;
    if (str.length()+end.length()<20)
        S.fill(' ',20-str.length()-end.length());
    return str+S+end;
}

void MainWindow::setLang()
{
    startStopNames();
    this->setWindowTitle(LNG["conway_game"]);
    panelSettings->setText(LNG["sett"]);
    labelV->setText(LNG["speed"]);
    lavelTypeDraw->setText(LNG["type_of_draw"]);
    checkAnimation->setText(LNG["animation"]);
    checkGrid->setText(LNG["grid"]);
    checkAxes->setText(LNG["axes"]);
    checkStatistic->setText(LNG["statistic"]);
    checkDraw->setText(LNG["drawing"]);
    ButtonStep->setText(LNG["step"]);
    ButtonClear->setText(LNG["clear_map"]);
    ButtonAgar->setText(LNG["create_agar"]);
    ButtonRnd->setText(LNG["random_map"]);

    menuFile->setTitle(LNG["file"]);

        actNewFigure->setText(AddEnd(LNG["new_fig"],tr("  ctrl+N")));
        actOpen->setText(AddEnd(LNG["open"],tr("  ctrl+O")));
        if (menuOpenFinded!=NULL) menuOpenFinded->setTitle(LNG["open_finded"]);
        actSave->setText(AddEnd(LNG["save"],tr("  ctrl+S")));
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
        if (LNG.count()>1) menuLang->setTitle(LNG["languages"]);

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
    comboModels->clear();
    comboModels->addItem(strPen);
    for ( StringMap<Model*>::iterator it = currentModelCollection.begin(); it != currentModelCollection.end(); ++it )
        comboModels->addItem( it.key() );
    comboModels->adjustSize();
}

void MainWindow::setDrawingEnable(int on)
{
    comboModels->setEnabled(on!=0);
    lavelTypeDraw->setEnabled(on!=0);
}

void MainWindow::createNewFigure()
{
    dialogCreating->exec();
    if (dialogCreating->m_result)
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
    s3d->setCurrentModel( name == strPen
        ? NULL
        : currentModelCollection[name] );
}

void MainWindow::resize()
{
    int menuHeight = this->menuBar()->height();
    s3d->setGeometry(0,menuHeight,this->width()-wPanel,this->height()-menuHeight);
    panelSettings->setGeometry(this->width()-wPanel,menuHeight,wPanel,this->height()-menuHeight);
}

void MainWindow::setSettingsVisible(bool value)
{
    panelSettings->setVisible(value);
    wPanel = value?200:0;
    resize();
}

void MainWindow::animationChanged(int value)
{
  //  s3d.setAnimationEnable(value!=0);
}

double getVelocity(QSlider * slider)
{
    return pow(1. * (slider->maximum() - slider->value()) / slider->maximum(), 1) * 1000;
}

void MainWindow::startStopNames()
{
    if (s3d != NULL && s3d->isExecute())
    {
        actStartStop->setText(LNG["stop"]);
        ButtonStart->setText(LNG["stop"]);
    }
    else
    {
        actStartStop->setText(LNG["start"]);
        ButtonStart->setText(LNG["start"]);
    }
}

void MainWindow::buttonStartClicked()
{
    if (s3d->isExecute())
        s3d->stop();
    else
        s3d->start( getVelocity( sliderVelocity ) );
    startStopNames();

}
void MainWindow::sliderVelValueChanged(int)
{
    s3d->setInterval( getVelocity( sliderVelocity ) );
}

void MainWindow::resizeEvent(QResizeEvent * /*e*/)
{
    resize();
}

void MainWindow::openFile(QString fn)
{
    if (!isFileValid(fn))
    {
        QMessageBox M;
        M.setWindowTitle(LNG["file_not_correct"]);
        M.setText(LNG["cannot_open_this_file"]);
        M.exec();
        return;
    }
    FILE * F;
    if (!fn.isNull() && (F = fopen(fn.toLocal8Bit().data(),"rb") ) != NULL )
    {
        int vers, type;
        fread(&vers,4,1,F);
        fread(&type,4,1,F);

        switch ((FigureType)type)
        {
            case figTorus:
                s3d->setFigure( new Torus(F) );
                break;
            case figSurface:
                s3d->setFigure( new Surface(F) );
                break;
            case figEllipsoid:
            case figParallelepiped:
                s3d->setFigure( new Ellipsoid(F) );
                break;
        }
        fclose(F);
        file_save_name = fn;
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

bool MainWindow::isFileValid(QString& fn)
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

void MainWindow::saveFileTo(QString fn)
{
    FILE * F;
    if (!fn.isNull() && (F = fopen(fn.toLocal8Bit().data(),"w+b") )!=NULL )
    {
        fwrite(&version_saving,4,1,F);
        s3d->getFigure()->toFile(F);
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
    if (s3d == NULL || s3d->getFigure() == NULL )
    {
        QMessageBox M;
        M.setText(tr("Figure hasn't been created") );
        M.setWindowTitle(tr("Cannot save"));
        M.exec();
        return;
    }
    MyFileDialog D;
    QString fn = D.execSave();
    if (!fn.isNull())
        saveFileTo(fn);
}

void MainWindow::keyPressEvent(QKeyEvent* E)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        switch (E->key())
        {
            case Qt::Key_N:
                dialogCreating->exec();
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
    delete dSettings;
    delete dialogCreating;
    delete dAbout;
    delete ButtonStart;
    delete ButtonStep;

    delete sliderVelocity;
    delete labelV;
    delete checkAnimation;
    delete checkGrid;
    delete checkStatistic;
    delete checkDraw;
    delete checkAxes;
    delete panelSettings;
    delete s3d;
    delete dTemplates;
 //       QMessageBox X;
  //  X.exec();
  //  DELETE(menuHelp);

    //QAction * actStartStop;
}
