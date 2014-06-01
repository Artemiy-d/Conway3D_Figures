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
    m_gridBox = NULL;
    m_widgetsCount = 0;
    m_widgets[m_widgetsCount++] = m_labelAllTemplates = new QLabel(this);
    m_labelAllTemplates->move(15,12);
    m_listWidgetModels = new QListWidget(this);
    m_listWidgetModels->setGeometry(15,m_labelAllTemplates->y()+18,200,180);
    m_listWidgetModels->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_btnToActive = new QPushButton(this);
    m_btnToActive->setGeometry(m_listWidgetModels->x() + m_listWidgetModels->width() + 10,
                               m_listWidgetModels->y() + m_listWidgetModels->height() / 2 - 13,
                               26,
                               26);
    m_btnToActive->setText(">");
    connect(m_btnToActive,SIGNAL(clicked()),this,SLOT(modelsToActive()));

    m_listWidgetActiveModels = new QListWidget(this);
    m_listWidgetActiveModels->setGeometry(m_btnToActive->x() + m_btnToActive->width()+10,
                                          m_listWidgetModels->y(),
                                          m_listWidgetModels->width(),
                                          m_listWidgetModels->height() );
    m_listWidgetActiveModels->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_widgets[m_widgetsCount++] = m_labelActiveTemplates = new QLabel(this);
    m_labelActiveTemplates->move(m_listWidgetActiveModels->x(),m_labelAllTemplates->y());

    m_gridBox = new GroupBoxCustom(this);

    m_widgets[m_widgetsCount++] = m_labelCurrent = new QLabel(m_gridBox);
    m_labelCurrent->move(15,20);

    m_editCurrentName = new QLineEdit(m_gridBox);
    m_editCurrentName->setGeometry(14,m_labelCurrent->y()+18,120,24);
    m_editCurrentName->setMaxLength(150);
    m_editCurrentName->setText(tr("My template"));

    m_widgets[m_widgetsCount++] = m_labelSize = new QLabel(m_gridBox);
    m_labelSize->move(15,m_editCurrentName->y()+m_editCurrentName->height()+10);

    m_spinBoxTemplateSize = new QSpinBox(m_gridBox);
    m_spinBoxTemplateSize->setGeometry(14,m_labelSize->y()+18,120,25);
    m_spinBoxTemplateSize->setRange(2,100);
    m_spinBoxTemplateSize->setValue(5);

    m_widgets[m_widgetsCount++] = m_btnAddToActive = new QPushButton(m_gridBox);
    m_btnAddToActive->move(14,m_spinBoxTemplateSize->y()+m_spinBoxTemplateSize->height()+15);
    connect(m_btnAddToActive,SIGNAL(clicked()),this,SLOT(addToActive()) );

    m_widgets[m_widgetsCount++] = m_btnSave = new QPushButton(m_gridBox);
    m_btnSave->move(14,m_btnAddToActive->y()+m_btnAddToActive->height()+1);
    connect(m_btnSave,SIGNAL(clicked()),this,SLOT(saveTemplate()) );

    m_modelRedactor = new ModelRedactor(m_gridBox);
    connect(m_spinBoxTemplateSize,SIGNAL(valueChanged(int)),m_modelRedactor,SLOT(setQuadSize(int) ));
    setLang();
    connect(&LNG,SIGNAL(set_lang()),this,SLOT(setLang()) );
    this->resize(700,500);
    this->setMinimumSize(600,400);

    connect(m_listWidgetModels,SIGNAL(itemDoubleClicked(QListWidgetItem*) ),this,SLOT(selectModel(QListWidgetItem*) ) );
    connect(m_listWidgetActiveModels,SIGNAL(itemDoubleClicked(QListWidgetItem*) ),this,SLOT(selectModel(QListWidgetItem*) ) );

   // m_listWidgetActiveModels->setDragEnabled(true);
  //  m_listWidgetModels->setDragEnabled(true);
  //  m_listWidgetModels->setDragDropMode(QAbstractItemView::DragDrop);
  //  m_listWidgetActiveModels->setDragDropMode(QAbstractItemView::DragDrop);
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
    refreshList(modelCollection,m_listWidgetModels);
    refreshList(currentModelCollection,m_listWidgetActiveModels);

    m_listWidgetModels->installEventFilter(this);
    m_listWidgetActiveModels->installEventFilter(this);
}

void DialogTemplates::selectModel(QListWidgetItem *item)
{
    Model * model = QObject::sender() == m_listWidgetModels ?
        modelCollection[item->text()] : currentModelCollection[item->text()];
    m_spinBoxTemplateSize->setValue(model->getSize());
    m_modelRedactor->setModel( model );
    m_editCurrentName->setText(item->text());
}

bool DialogTemplates::eventFilter(QObject * obj, QEvent *E)
{
    if (E->type() == QEvent::KeyPress && (reinterpret_cast<QKeyEvent*>(E) )->key() == Qt::Key_Delete)
    {
        if (obj == m_listWidgetModels)
            deleteModels(modelCollection,m_listWidgetModels);
        else if (obj == m_listWidgetActiveModels)
            deleteModels(currentModelCollection,m_listWidgetActiveModels);
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
    else
    {
        while (*(str++));
    }
    --str;
    while ( *(--str) == ' ' );
    *(++str)=0;
}

bool DialogTemplates::beginAddTemplate(StringMap<Model*> &coll)
{
    if (m_editCurrentName->text().length()==0)
    {
        QMessageBox messageBox;
        messageBox.setText(LNG["input_name"]);
        messageBox.setWindowTitle(LNG["empty_name"]);
        messageBox.exec();
        return false;
    }
    char str[200];
    strcpy(str,m_editCurrentName->text().toLocal8Bit().data());
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
    coll[str] = new Model(*(Model*)m_modelRedactor);
    this->setWindowTitle(QString::fromLocal8Bit(str) );
    return true;
}

void DialogTemplates::saveTemplate()
{
    if (!beginAddTemplate(modelCollection))
        return;
    QString fn = QDir::currentPath()+tr("/modeles/")+m_editCurrentName->text()+tr(".mdl");
    modelCollection[m_editCurrentName->text()]->saveToFile(fn.toLocal8Bit().data());
    refreshList(modelCollection,m_listWidgetModels);
}

void DialogTemplates::addToActive()
{
    if (!beginAddTemplate(currentModelCollection))
        return;
    refreshList(currentModelCollection,m_listWidgetActiveModels);
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
    m_gridBox->setText(LNG["current_template"]);
    m_labelCurrent->setText(LNG["template_name"]);
    m_labelSize->setText(LNG["size_template"]);
    m_labelAllTemplates->setText(LNG["all_templates"]);
    m_labelActiveTemplates->setText(LNG["active_templates"] );
    m_btnAddToActive->setText(LNG["add_to_active"]);
    m_btnSave->setText(LNG["save_template"]);
    for (int i = 0; i < m_widgetsCount; i++)
        m_widgets[i]->adjustSize();
}

void DialogTemplates::resizeEvent(QResizeEvent * /*e*/)
{
    if (m_gridBox == NULL)
        return;
    int offset = 150, h = m_listWidgetModels->y()+m_listWidgetModels->height()+15;
    m_gridBox->setGeometry(0,h,this->width(),this->height()-h);
    int w = std::min(m_gridBox->width()- offset - 20, m_gridBox->height()-30);
    m_modelRedactor->setGeometry(offset,20,w,w);
}

void DialogTemplates::modelsToActive()
{
    QList<QListWidgetItem*> items( m_listWidgetModels->selectedItems() );
    if (items.empty())
        return;
    foreach (QListWidgetItem * it, items)
    {
        currentModelCollection[it->text()] = modelCollection[it->text()];
    }
    refreshList(currentModelCollection,m_listWidgetActiveModels);
    emit newActive();
}
