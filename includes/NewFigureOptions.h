#ifndef NEWFIGUREOPTIONS_H
#define NEWFIGUREOPTIONS_H

#include <QWidget>

#include "FiguresFactory.h"

class QLabel;
class QSpinBox;
class GroupBoxCustom;

namespace NewFigureOptions
{

    class OptionsBase : public QWidget, public FiguresFactory::IFigureCreator
    {
    public:
        OptionsBase(QWidget * _parent) : QWidget( _parent ) { }
    };

    class LabeledSpinboxes : public OptionsBase
    {
    public:
        LabeledSpinboxes(QWidget * _parent, size_t _paramsCount);

        void resizeEvent(QResizeEvent * _e);

        virtual ~LabeledSpinboxes();

        virtual void setLang();


    protected:
        GroupBoxCustom * m_box;
        QLabel ** m_labelsSize;
        QSpinBox ** m_spinBoxesSize;

        size_t m_paramsCount;
    };

    class OptionsTorus : public LabeledSpinboxes
    {
    public:
        OptionsTorus(QWidget * _parent = NULL);

        Figure * createFigure();

        void setLang();
    };

    class OptionsSurface : public LabeledSpinboxes
    {
    public:
        OptionsSurface(QWidget * _parent = NULL);

        Figure * createFigure();
    };

    class OptionsParallelepiped : public LabeledSpinboxes
    {
    public:
        OptionsParallelepiped(QWidget * _parent = NULL);

        Figure * createFigure();
    };

    class OptionsEllipsoid : public LabeledSpinboxes
    {
    public:
        OptionsEllipsoid(QWidget * _parent = NULL);

        Figure * createFigure();
    };

}


#endif // NEWFIGUREOPTIONS_H
