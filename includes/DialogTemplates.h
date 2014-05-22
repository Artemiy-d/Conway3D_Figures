#ifndef MODELTEMPLATES_H
#define	MODELTEMPLATES_H

#include <QDialog>



class GrawGrid;
class MyGroupBox;
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
struct StringMap;

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

    GrawGrid * d_g;
    MyGroupBox * grid_box;
    QLineEdit * current_name;
    QLabel * current_label, * label_size, * label_all_templates, * label_active_templates;
    QSpinBox * spin_size;
    QListWidget * listModels, * listActiveModels;
    QPushButton * buttAddToActive, * buttSave, * buttToActive, * buttRotR,
        * buttRotL, * butMirrX, * buttMirrY;
    QWidget * m_widgets[20];
    int m_widgetsCount;
};

#endif	/* MODELTEMPLATES_H */

