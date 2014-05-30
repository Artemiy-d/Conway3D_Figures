#ifndef SCENE3D_H
#define	SCENE3D_H

#include <QGLWidget>
#include <QPainter>

#include "FigureClasses.h"
#include "Scene3DView.h"


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
        StatisticWidget(QWidget * _parent);
        void resizeEvent(QResizeEvent * _e);
        void paintEvent(QPaintEvent * _e);
        void setText(const QString & _text, int _stringNumber, bool _refresh = true);
    private:
        QString m_text[STATISTIC_ITEMS_COUNT];
        QPainter m_painter;
        QFont m_font;
    };

public:
    Scene3D(QWidget* _parent = 0);
    virtual ~Scene3D();

    bool isExecuted();
    void start(int _interval = 500);
    void stop();
    void setInterval(int _interval = 500);
    void createFigure(FigureType _figureType, int * _sizeParams, float * _phizParams, bool _copySettings);

    void setFigure(Figure * _figure);

    Figure * getFigure();
    void setCurrentModel(Model * _model);

private:
    void drawStatistic();
    void drawModel(const QPoint & _point);

protected:
    void initializeGL();
    void resizeGL(int _nWidth, int _nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* _e);
    void mouseMoveEvent(QMouseEvent* _e);
    void mouseDoubleClickEvent(QMouseEvent * _e);
    void wheelEvent(QWheelEvent* _e);
    void keyPressEvent(QKeyEvent* _e);

public slots:
    void stepFigure();
    void setAnimationEnable(int _on);
    void setGridEnable(int _on);
    void setDrawingEnable(int _on);
    void setStatisticVisible(int _on);
    void setAxesVisible(int _on);
    void createAgar();
    void clearMap();
    void createRandomMap();
    void setFullScreen();
    void restoreNormal();

private:
    QTimer * m_timer;

    StatisticWidget * m_statisticWidget;
    Model * m_currentModel;

    bool m_animationOn,
            m_drawingOn,
            m_axesVisible,
            m_statisticVisible;

    bool m_leftButtonPressed;
    QPoint m_mousePosition;
    unsigned long long m_stepsNumber;
    QWidget * m_savedParent;
    QRect m_savedGeometry;
    Figure * m_figure;

    Scene3DView m_view;
};

#endif	/* SCENE3D_H */

