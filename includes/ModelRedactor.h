#ifndef OTHERGUICLASSES_H
#define	OTHERGUICLASSES_H
#include <QWidget>
#include <QPainter>

#include "Modeles.h"


class ModelRedactor : public QWidget, public Model
{
    Q_OBJECT
public:
    ModelRedactor(QWidget * parent = 0);
    virtual ~ModelRedactor();

    ModelRedactor& operator = (const Model &model);
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

#endif	/* OTHERGUICLASSES_H */
