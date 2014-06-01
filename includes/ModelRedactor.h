#ifndef OTHERGUICLASSES_H
#define	OTHERGUICLASSES_H

#include <QWidget>

class Model;

class ModelRedactor : public QWidget
{
    Q_OBJECT
private:
    class View
    {
    public:
        View(const QSize & _visibleSize, const Model * _model);
        virtual ~View();
        void getModelPointByVisiblePoint(const QPoint & _visiblePoint, int & _x, int & _y) const;
        void update(const QSize & _visibleSize, const Model * _model);
        void draw(QPaintDevice * _device);
    protected:
        void updateBorders();
    private:
        QSize m_visibleSize;
        const Model * m_model;
        int * m_XPoints, * m_YPoints;
    };
public:
    ModelRedactor(QWidget * _parent = 0);
    virtual ~ModelRedactor();

    void setModel (const Model * _model);
protected:
    void paintEvent(QPaintEvent * _e);
    void mousePressEvent(QMouseEvent * _e);
    void resizeEvent(QResizeEvent * _e);

    void createField(int _sz);

public slots:
    void setQuadSize(int _sz);

private:
    Model * m_model;
    View * m_view;
};

#endif	/* OTHERGUICLASSES_H */

