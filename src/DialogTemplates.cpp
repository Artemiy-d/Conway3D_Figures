#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDir>


#include "LanguageManager.h"
#include "Modeles.h"
#include "ModelRedactor.h"
#include "GroupBoxCustom.h"
#include "DialogTemplates.h"


DialogTemplates::DialogTemplates()
{
    grid_box = NULL;
    m_widgetsCount = 0;
    m_widgets[m_widgetsCount++] = label_all_templates = new QLabel(this);
    label_all_templates->move(15,12);
    listModels = new QListWidget(this);
    listModels->setGeometry(15,label_all_templates->y()+18,200,180);
    listModels->setSelectionMode(QAbstractItemView::ExtendedSelection);

    buttToActive = new QPushButton(this);
    buttToActive->setGeometry(listModels->x()+listModels->width()+10,
                             listModels->y()+listModels->height()/2-13,26,26);
    buttToActive->setText(">");
    connect(buttToActive,SIGNAL(clicked()),this,SLOT(modelsToActive()));

    listActiveModels = new QListWidget(this);
    listActiveModels->setGeometry(buttToActive->x()+buttToActive->width()+10,listModels->y(),listModels->width(),listModels->height() );
    listActiveModels->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_widgets[m_widgetsCount++] = label_active_templates = new QLabel(this);
    label_active_templates->move(listActiveModels->x(),label_all_templates->y());

    grid_box = new GroupBoxCustom(this);

    m_widgets[m_widgetsCount++] = current_label = new QLabel(grid_box);
    current_label->move(15,20);

    current_name = new QLineEdit(grid_box);
    current_name->setGeometry(14,current_label->y()+18,120,24);
    current_name->setMaxLength(150);
    current_name->setText(tr("My template"));

    m_widgets[m_widgetsCount++] = label_size = new QLabel(grid_box);
    label_size->move(15,current_name->y()+current_name->height()+10);

    spin_size = new QSpinBox(grid_box);
    spin_size->setGeometry(14,label_size->y()+18,120,25);
    spin_size->setRange(2,100);
    spin_size->setValue(5);

    m_widgets[m_widgetsCount++] = buttAddToActive = new QPushButton(grid_box);
    buttAddToActive->move(14,spin_size->y()+spin_size->height()+15);
    connect(buttAddToActive,SIGNAL(clicked()),this,SLOT(addToActive()) );

    m_widgets[m_widgetsCount++] = buttSave = new QPushButton(grid_box);
    buttSave->move(14,buttAddToActive->y()+buttAddToActive->height()+1);
    connect(buttSave,SIGNAL(clicked()),this,SLOT(saveTemplate()) );

    d_g = new ModelRedactor(grid_box);
    connect(spin_size,SIGNAL(valueChanged(int)),d_g,SLOT(setQuadSize(int) ));
    setLang();
    connect(&LNG,SIGNAL(set_lang()),this,SLOT(setLang()) );
    this->resize(700,500);
    this->setMinimumSize(600,400);

    connect(listModels,SIGNAL(itemDoubleClicked(QListWidgetItem*) ),this,SLOT(selectModel(QListWidgetItem*) ) );
    connect(listActiveModels,SIGNAL(itemDoubleClicked(QListWidgetItem*) ),this,SLOT(selectModel(QListWidgetItem*) ) );

   // listActiveModels->setDragEnabled(true);
  //  listModels->setDragEnabled(true);
  //  listModels->setDragDropMode(QAbstractItemView::DragDrop);
  //  listActiveModels->setDragDropMode(QAbstractItemView::DragDrop);
    QDir dir(QDir::currentPath());
    dir.mkdir(tr("modeles"));
    QDir dir_mod(QDir::currentPath()+tr("/modeles"));
    QStringList filt(tr("*.mdl"));
    QStringList L = dir_mod.entryList(filt,QDir::Files);
    if (L.count()!=0)
    {
        char dt[250];
        foreach (QString s, L)
        {
            QString abs_path = dir_mod.absoluteFilePath(s);
            char * abs_data = abs_path.toLocal8Bit().data();
            abs_data[abs_path.length()]=0;
            char * data = dt;
            while ( !!(*(data++) = *(abs_data++)) );

            if (Model::isFileValid(dt) )
            {
                int index = s.indexOf( '.' );
                if ( index != -1 )
                    s[index] = 0;
                modelCollection[s] = new Model(dt);
            }
        }
    }
    refreshList(modelCollection,listModels);
    refreshList(currentModelCollection,listActiveModels);

    listModels->installEventFilter(this);
    listActiveModels->installEventFilter(this);
}

void DialogTemplates::selectModel(QListWidgetItem *item)
{
    Model * model = QObject::sender() == listModels ?
        modelCollection[item->text()] : currentModelCollection[item->text()];
    spin_size->setValue(model->getSize());
    *d_g = *model;
    current_name->setText(item->text());
}

bool DialogTemplates::eventFilter(QObject * obj, QEvent * E)
{
    if (E->type() == QEvent::KeyPress && (reinterpret_cast<QKeyEvent*>(E) )->key() == Qt::Key_Delete)
    {
        if (obj == listModels)
            deleteModels(modelCollection,listModels);
        else if (obj == listActiveModels)
            deleteModels(currentModelCollection,listActiveModels);
    }
    return QDialog::eventFilter(obj,E);
}

void DialogTemplates::deleteModels(StringMap<Model*> &coll, QListWidget * list)
{
    QList<QListWidgetItem*> L = list->selectedItems();
    foreach (QListWidgetItem* w, L)
    {
        coll.erase( coll.find( w->text() ) );
    }
    refreshList(coll,list);
}

void DialogTemplates::refreshList(StringMap<Model*> &coll, QListWidget * list)
{
    list->clear();
    for ( StringMap<Model*>::iterator it = coll.begin(); it != coll.end(); ++it )
        list->addItem( it.key() );
}

static void normalizeStr(char * str)
{
    char * s1 = str;
    if (*s1 == ' ') 
    {
        while ( *(++s1) == ' ' );
        while ( !!(*(str++) = *(s1++)) );
    }
    else while (*(str++));
    str--;
    while ( *(--str) == ' ' );
    *(++str)=0;
}

bool DialogTemplates::beginAddTemplate(StringMap<Model*> &coll)
{
    if (current_name->text().length()==0)
    {
        QMessageBox M;
        M.setText(LNG["input_name"]);
        M.setWindowTitle(LNG["empty_name"]);
        M.exec();
        return false;
    }
    char str[200];
    strcpy(str,current_name->text().toLocal8Bit().data());
    normalizeStr(str);
    if (coll.find(str) != coll.end())
    {
        QMessageBox M;
        M.addButton(QMessageBox::Yes);
        M.addButton(QMessageBox::No);
        M.setDefaultButton(QMessageBox::No);
        M.setText(LNG["replace_"]);
        M.setWindowTitle(LNG["name_prezent"]);
        if (M.exec() == QMessageBox::No)
            return false;
    }
    coll[str] = new Model(*(Model*)d_g);
    this->setWindowTitle(QString::fromLocal8Bit(str) );
    return true;
}

void DialogTemplates::saveTemplate()
{
    if (!beginAddTemplate(modelCollection))
        return;
    QString fn = QDir::currentPath()+tr("/modeles/")+current_name->text()+tr(".mdl");
    modelCollection[current_name->text()]->saveToFile(fn.toLocal8Bit().data());
    refreshList(modelCollection,listModels);
}

void DialogTemplates::addToActive()
{
    if (!beginAddTemplate(currentModelCollection))
        return;
    refreshList(currentModelCollection,listActiveModels);
    emit newActive();
}


DialogTemplates::~DialogTemplates()
{
    for (int i = 0; i < m_widgetsCount; i++)
        delete m_widgets[i];
}

void DialogTemplates::setLang()
{
    this->setWindowTitle(LNG["templates"]);
    grid_box->setText(LNG["current_template"]);
    current_label->setText(LNG["template_name"]);
    label_size->setText(LNG["size_template"]);
    label_all_templates->setText(LNG["all_templates"]);
    label_active_templates->setText(LNG["active_templates"] );
    buttAddToActive->setText(LNG["add_to_active"]);
    buttSave->setText(LNG["save_template"]);
    for (int i = 0; i < m_widgetsCount; i++)
        m_widgets[i]->adjustSize();
}

void DialogTemplates::resizeEvent(QResizeEvent * /*e*/)
{
    if (grid_box == NULL)
        return;
    int offset = 150, h = listModels->y()+listModels->height()+15;
    grid_box->setGeometry(0,h,this->width(),this->height()-h);
    int w = std::min(grid_box->width()- offset - 20, grid_box->height()-30);
    d_g->setGeometry(offset,20,w,w);
}

void DialogTemplates::modelsToActive()
{
    QList<QListWidgetItem*> L = listModels->selectedItems();
    if (L.count() == 0)
        return;
    foreach (QListWidgetItem * it, L)
    {
        currentModelCollection[it->text()] = modelCollection[it->text()];
    }
    refreshList(currentModelCollection,listActiveModels);
    emit newActive();
}
