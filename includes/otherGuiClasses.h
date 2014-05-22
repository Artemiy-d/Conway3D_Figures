#ifndef OTHERGUICLASSES_H
#define	OTHERGUICLASSES_H
#include <QWidget>
#include <QPainter>
#include <QFileDialog>

#include "Modeles.h"

class MyGroupBox : public QWidget
{
public:
    MyGroupBox(QWidget * parent = NULL);
    virtual ~MyGroupBox() {}
    void setText(const QString & _text);
    void paintEvent(QPaintEvent * e);

private:
    QPainter painter;
    QString text;
};


class GrawGrid : public QWidget, public Model
{
    Q_OBJECT
public:
    GrawGrid(QWidget * parent = 0);
    virtual ~GrawGrid();

    GrawGrid& operator = (const Model &model);
    inline void setGeometry(int x, int y, int w, int h)
    {
        createArray(w,h);
        QWidget::setGeometry(x,y,w,h);
    }
    void setQuadSize(int sz, bool * field);
protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);

    bool * getPoint(int x, int y);
    void createArray(int w, int h);
    void createField(int sz);

public slots:
    void setQuadSize(int sz);

private:
    int * x_arr, * y_arr;
    int w_cell, h_cell;
    QPainter painter;
};


class MyFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    MyFileDialog();
    virtual ~MyFileDialog() {}
    QString execSave();
private slots:
    void filterChanged(const QString& filter);

private:
    QStringList filters;
};

#endif	/* OTHERGUICLASSES_H */

