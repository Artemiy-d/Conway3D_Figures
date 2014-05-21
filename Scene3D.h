/* 
 * File:   Scene3D.h
 * Author: artyom
 *
 * Created on 1 Ноябрь 2011 г., 15:57
 */

#ifndef SCENE3D_H
#define	SCENE3D_H

#include <QGLWidget>
#include <QPainter>

#include "FigureClasses.h"

class QTimer;

enum GraphicObject
{
    torus,
    ellipsoid,
    parallelepiped
};



class Scene3D : public QGLWidget
{
    Q_OBJECT
private:

    class StatisticWidget : public QWidget
    {
    public:
        enum StatisticItemId
        {
            STEPS,
            LIVING_CELLS,
            STATISTIC_ITEMS_COUNT
        };
    public:
        StatisticWidget(QWidget * parent);
        void resizeEvent(QResizeEvent * e);
        void paintEvent(QPaintEvent * e);
        void setText(const QString & text, int stringNumber, bool refresh = true);
    private:
        QString text[STATISTIC_ITEMS_COUNT];
        QPainter painter;
        QFont font;
    };

public:
    Scene3D(QWidget* parent = 0);
    virtual ~Scene3D();

    bool isExecute();
    void start(int interval = 500);
    void stop();
    void setInterval(int interval = 500);
    void createFigure(FigureType typeFigure, int * sizeParams, float * phizParams, bool copy_settings);

    void setFigure(Figure * _figure);

    Figure * getFigure();

private:
    void scalePlus();
    void scaleMinus();
    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();
    void translateDown();
    void translateUp();
    void defaultScene();
    void drawAxis();
    void drawStatistic();
    void getCoord(int mouseX, int mouseY, fpoint * point1, fpoint *point_1);

protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseDoubleClickEvent(QMouseEvent * e);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);
    void hideEvent(QHideEvent * e);

public slots:
    void stepFigure();
    void setAnimationEnable(int on);
    void setGridEnable(int on);
    void setDrawingEnable(int on);
    void setStatisticVisible(int on);
    void setAxesVisible(int on);
    void createAgar();
    void clearMap();
    void createRandomMap();
    void changeDrawModel(const QString& name);
    void setFullScreen();

private:
    QTimer * tm;

    StatisticWidget * textWidget;
    Model * currentModel;
    bool Executing, AnimationOn, DrawingOn, AxesVisible, gridEnable, statistVisible;
    bool disposing;
    GLfloat xRot, yRot, zRot, zTra, nSca;
    QPoint ptrMousePosition;
    int stp;
    QWidget * prnt;
    Figure * figure;
};

#endif	/* SCENE3D_H */

