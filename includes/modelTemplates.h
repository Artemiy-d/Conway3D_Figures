/* 
 * File:   modelTemplates.h
 * Author: artyom
 *
 * Created on 22 Ноябрь 2011 г., 12:16
 */

#ifndef MODELTEMPLATES_H
#define	MODELTEMPLATES_H

#include <QDialog>

#include "MyCollection.h"


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

class DialogTemplates : public QDialog
{
    Q_OBJECT
public:
    DialogTemplates();
    DialogTemplates(const DialogTemplates& orig);
    virtual ~DialogTemplates();
private slots:
    void setLang();
    void selectModel(QListWidgetItem *item);
    void addToActive();
    void saveTemplate();
    void modelsToActive();
protected:
    void resizeEvent(QResizeEvent * e);
    void refreshList(MyCollection<Model*> &coll, QListWidget * list);
    void deleteModels(MyCollection<Model*> &coll, QListWidget * list);
    bool eventFilter(QObject * obj, QEvent * e);
    bool beginAddTemplate(MyCollection<Model*> &coll);
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
    QWidget * widgets[20];
    int w_count;
};

#endif	/* MODELTEMPLATES_H */

