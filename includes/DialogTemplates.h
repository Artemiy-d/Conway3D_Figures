#ifndef DIALOGTEMPLATES_H
#define	DIALOGTEMPLATES_H

#include <QDialog>



class ModelRedactor;
class GroupBoxCustom;
class QLineEdit;
class QLabel;
class QSpinBox;
class QPushButton;
class QWidget;
class QListWidgetItem;
class QListWidget;
class Model;
class QResizeEvent;

template <class T>
class StringMap;

class DialogTemplates : public QDialog
{
    Q_OBJECT
public:
    DialogTemplates();
    virtual ~DialogTemplates();
private slots:
    void setLang();
    void selectModel(QListWidgetItem *item);
    void addToActive();
    void saveTemplate();
    void modelsToActive();
protected:
    void resizeEvent(QResizeEvent * e);
    void refreshList(StringMap<Model*> &coll, QListWidget * list);
    void deleteModels(StringMap<Model*> &coll, QListWidget * list);
    bool eventFilter(QObject * obj, QEvent * e);
    bool beginAddTemplate(StringMap<Model*> &coll);
signals:
    void newActive();

private:

    ModelRedactor * m_modelRedactor;
    GroupBoxCustom * m_gridBox;
    QLineEdit * m_editCurrentName;
    QLabel * m_labelCurrent, * m_labelSize, * m_labelAllTemplates, * m_labelActiveTemplates;
    QSpinBox * m_spinBoxTemplateSize;
    QListWidget * m_listWidgetModels, * m_listWidgetActiveModels;
    QPushButton * m_btnAddToActive, * m_btnSave, * m_btnToActive;
    QWidget * m_widgets[20];
    int m_widgetsCount;
};

#endif	/* DIALOGTEMPLATES_H */

