#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDir>


#include "LanguageManager.h"
#include "Models.h"
#include "ModelsManager.h"
#include "ModelRedactor.h"
#include "GroupBoxCustom.h"
#include "DialogTemplates.h"


DialogTemplates::DialogTemplates(ModelsManager * _modelsManager)
{
    m_modelsManager = _modelsManager;
    m_gridBox = NULL;
    m_widgetsCount = 0;
    m_widgets[m_widgetsCount++] = m_labelAllTemplates = new QLabel(this);
    m_labelAllTemplates->move(15, 12);
    m_listWidgetModels = new QListWidget(this);
    m_listWidgetModels->setGeometry(15, m_labelAllTemplates->y() + 18, 200, 180);
    m_listWidgetModels->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_btnToActive = new QPushButton(this);
    m_btnToActive->setGeometry(m_listWidgetModels->x() + m_listWidgetModels->width() + 10,
                               m_listWidgetModels->y() + m_listWidgetModels->height() / 2 - 13,
                               26,
                               26);
    m_btnToActive->setText(">");
    connect(m_btnToActive, SIGNAL(clicked()), this, SLOT(modelsToActive()));

    m_listWidgetActiveModels = new QListWidget(this);
    m_listWidgetActiveModels->setGeometry(m_btnToActive->x() + m_btnToActive->width() + 10,
                                          m_listWidgetModels->y(),
                                          m_listWidgetModels->width(),
                                          m_listWidgetModels->height() );
    m_listWidgetActiveModels->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_widgets[m_widgetsCount++] = m_labelActiveTemplates = new QLabel(this);
    m_labelActiveTemplates->move(m_listWidgetActiveModels->x(), m_labelAllTemplates->y());

    m_gridBox = new GroupBoxCustom(this);

    m_widgets[m_widgetsCount++] = m_labelCurrent = new QLabel(m_gridBox);
    m_labelCurrent->move(15, 20);

    m_editCurrentName = new QLineEdit(m_gridBox);
    m_editCurrentName->setGeometry(14, m_labelCurrent->y() + 18, 120, 24);
    m_editCurrentName->setMaxLength(150);
    m_editCurrentName->setText( "My template" );

    m_widgets[m_widgetsCount++] = m_labelSize = new QLabel(m_gridBox);
    m_labelSize->move(15, m_editCurrentName->y() + m_editCurrentName->height() + 10);

    m_spinBoxTemplateSize = new QSpinBox(m_gridBox);
    m_spinBoxTemplateSize->setGeometry(14, m_labelSize->y() + 18, 120, 25);
    m_spinBoxTemplateSize->setRange(2, 100);
    m_spinBoxTemplateSize->setValue(5);

    m_widgets[m_widgetsCount++] = m_btnAddToActive = new QPushButton(m_gridBox);
    m_btnAddToActive->move(14, m_spinBoxTemplateSize->y() + m_spinBoxTemplateSize->height() + 15);
    connect(m_btnAddToActive, SIGNAL(clicked()), this, SLOT(addToActive()) );

    m_widgets[m_widgetsCount++] = m_btnSave = new QPushButton(m_gridBox);
    m_btnSave->move(14, m_btnAddToActive->y() + m_btnAddToActive->height() + 1);
    connect(m_btnSave, SIGNAL(clicked()), this, SLOT(saveTemplate()) );

    m_modelRedactor = new ModelRedactor(m_gridBox);
    connect(m_spinBoxTemplateSize, SIGNAL(valueChanged(int)), m_modelRedactor, SLOT(setQuadSize(int) ));
    setLang();
    connect(&LNG, SIGNAL(set_lang()), this, SLOT(setLang()) );
    this->resize(700, 500);
    this->setMinimumSize(600, 400);

    connect(m_listWidgetModels,SIGNAL(itemDoubleClicked(QListWidgetItem*) ),this,SLOT(selectModel(QListWidgetItem*) ) );
    connect(m_listWidgetActiveModels,SIGNAL(itemDoubleClicked(QListWidgetItem*) ),this,SLOT(selectModel(QListWidgetItem*) ) );

    updateMainModels();
    updateActiveModels();

    m_listWidgetModels->installEventFilter(this);
    m_listWidgetActiveModels->installEventFilter(this);
}

void DialogTemplates::selectModel(QListWidgetItem * _item)
{
    const Model * model =  m_modelsManager->getModel( _item->text() );

    if ( model == NULL )
        return;

    m_spinBoxTemplateSize->setValue(model->getSize());
    m_modelRedactor->setModel( model );
    m_editCurrentName->setText( _item->text() );
}

bool DialogTemplates::eventFilter(QObject * _obj, QEvent * _e)
{
    if (_e->type() == QEvent::KeyPress && ( (QKeyEvent*)_e )->key() == Qt::Key_Delete)
    {
        QListWidget * list = (QListWidget*)_obj;
        QList<QListWidgetItem*> selected = list->selectedItems();

        foreach (QListWidgetItem* it, selected)
        {
            m_modelsManager->setEnabled( it->text(),
                                         _obj == m_listWidgetModels ? ModelsManager::SetFalse : ModelsManager::DoNotChange,
                                         ModelsManager::SetFalse );
        }

        if ( _obj == m_listWidgetModels )
            updateMainModels();

        updateActiveModels();
    }
    return QDialog::eventFilter(_obj, _e);
}


bool DialogTemplates::beginAddTemplate()
{
    if (m_editCurrentName->text().length()==0)
    {
        QMessageBox messageBox;
        messageBox.setText(LNG["input_name"]);
        messageBox.setWindowTitle(LNG["empty_name"]);
        messageBox.exec();
        return false;
    }

    QString normalized = ModelsManager::normalizeName(m_editCurrentName->text());
    if (m_modelsManager->getModel( normalized ))
    {
        QMessageBox messageBox;
        messageBox.addButton(QMessageBox::Yes);
        messageBox.addButton(QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        messageBox.setText(LNG["replace_"]);
        messageBox.setWindowTitle(LNG["name_prezent"]);
        if (messageBox.exec() == QMessageBox::No)
            return false;
    }

    m_editCurrentName->setText( normalized );

    return true;
}

void DialogTemplates::updateMainModels()
{
    m_listWidgetModels->clear();
    m_listWidgetModels->addItems( m_modelsManager->getMainModels() );
}

void DialogTemplates::updateActiveModels()
{
    m_listWidgetActiveModels->clear();
    m_listWidgetActiveModels->addItems( m_modelsManager->getActiveModels() );
}

void DialogTemplates::saveTemplate()
{
    if (!beginAddTemplate())
        return;

    m_modelsManager->addModel( m_editCurrentName->text(),
                               m_modelRedactor->getModel(),
                               ModelsManager::SetTrue, ModelsManager::DoNotChange );

    m_modelsManager->saveModel( m_editCurrentName->text() );

    updateMainModels();
    updateActiveModels();
}

void DialogTemplates::addToActive()
{
    if (!beginAddTemplate())
        return;

    m_modelsManager->addModel( m_editCurrentName->text(),
                               m_modelRedactor->getModel(),
                               ModelsManager::DoNotChange, ModelsManager::SetTrue );

    updateActiveModels();

    emit newActive();
}


DialogTemplates::~DialogTemplates()
{
    for (int i = 0; i < m_widgetsCount; i++)
        delete m_widgets[i];
}

void DialogTemplates::setLang()
{
    setWindowTitle(LNG["templates"]);
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

void DialogTemplates::resizeEvent(QResizeEvent * /*_e*/)
{
    if (m_gridBox == NULL)
        return;
    int offset = 150,
        h = m_listWidgetModels->y() + m_listWidgetModels->height() + 15;
    m_gridBox->setGeometry(0, h, width(), height() - h);
    int w = std::min(m_gridBox->width() - offset - 20, m_gridBox->height() - 30);
    m_modelRedactor->setGeometry(offset, 20, w, w);
}

void DialogTemplates::modelsToActive()
{
    QList<QListWidgetItem*> items( m_listWidgetModels->selectedItems() );
    if (items.empty())
        return;
    foreach (QListWidgetItem * it, items)
    {
        m_modelsManager->setEnabled( it->text(), ModelsManager::DoNotChange, ModelsManager::SetTrue );
    }
    updateActiveModels();

    emit newActive();
}
