#ifndef FIGURESFACTORY_H
#define FIGURESFACTORY_H

#include <map>
#include <string>

class Figure;

class FiguresFactory
{
public:
    class IFigureCreator
    {
    public:
        virtual Figure * create() = 0;
    };

    template <typename FigureType>
    class FigureCreator<FigureType> : public IFigureCreator
    {
    public:
        FigureCreator<FigureType>()
        {
            FiguresFactory::getInstance().addCreator( FigureType::s_stringType, this );
        }

        Figure * create()
        {
            return new FigureType();
        }
    };
public:
    static FiguresFactory & getInstance();

    void addCreator( const std::string & _type, IFigureCreator * _creator );

    Figure * createFigure( const std::string & _type ) const;

private:
    FiguresFactory();
    ~FiguresFactory();

private:

    std::map<std::string, IFigureCreator*> m_creators;

    static FiguresFactory * s_instance;
};

#endif // FIGURESFACTORY_H
