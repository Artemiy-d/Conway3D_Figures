#ifndef SCENE3D_H
#define	SCENE3D_H

#include <QGLWidget>
#include <QPainter>

#include "FigureClasses.h"

class QTimer;

class Model;

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
        QString m_text[STATISTIC_ITEMS_COUNT];
        QPainter m_painter;
        QFont m_font;
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
    void setCurrentModel(Model * _model);

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
    void setFullScreen();

private:
    QTimer * m_timer;

    StatisticWidget * m_statisticWidget;
    Model * m_currentModel;

    bool m_executed,
            m_animationOn,
            m_drawingOn,
            m_axesVisible,
            m_gridEnable,
            m_statisticVisible;

    bool m_leftButtonPressed;
    GLfloat m_xRot, m_yRot, m_zRot, m_zTra, m_nSca;
    QPoint m_mousePosition;
    int m_stepsNumber;
    QWidget * m_savedParent;
    Figure * m_figure;
};

#endif	/* SCENE3D_H */

