#ifndef FIGURESFACTORY_H
#define FIGURESFACTORY_H

#include <map>
#include <string>
#include <list>

class Figure;
class QWidget;

class FiguresFactory
{
public:
    class IFigureCreator
    {
    public:
        virtual Figure * createFigure() = 0;
    };

    class IFigureCreatorExtended : public IFigureCreator
    {
    public:
        virtual IFigureCreator * createFigureCreator() = 0;
        virtual void reset() = 0;
    };

    template <typename FigureType, typename FigureCreatorType>
    class FigureCreatorExtended : public IFigureCreatorExtended
    {
    public:
        FigureCreatorExtended<FigureType, FigureCreatorType>() :
            m_figureCreator(NULL)
        {
            FiguresFactory::getInstance().addCreator( FigureType::s_stringType, this );
        }

        virtual ~FigureCreatorExtended<FigureType, FigureCreatorType>()
        {
            delete m_figureCreator;
        }

        Figure * createFigure()
        {
            return new FigureType();
        }

        IFigureCreator * createFigureCreator()
        {
            if (m_figureCreator == NULL)
                m_figureCreator = new FigureCreatorType();
            return m_figureCreator;
        }

        void reset()
        {
            delete m_figureCreator;
            m_figureCreator = NULL;
        }

    private:
        FigureCreatorType * m_figureCreator;
    };

    typedef std::map<std::string, IFigureCreatorExtended*> ExtendedCreators;

public:
    static FiguresFactory & getInstance();

    void addCreator( const std::string & _type, IFigureCreatorExtended * _creator );

    Figure * createFigure( const std::string & _type ) const;
    IFigureCreator * createFigureCreator(const std::string & _type) const;

    template <typename CreatorType>
    CreatorType * createFigureCreator(const std::string & _type) const
    {
        return dynamic_cast<CreatorType*>(createFigureCreator(_type));
    }

    std::list<std::string> getTypes() const;

    void reset();


private:
    FiguresFactory() { }
    FiguresFactory(const FiguresFactory & ) { }
    FiguresFactory & operator = (const FiguresFactory & ) { return *this; }
    ~FiguresFactory() { }


private:

    ExtendedCreators m_extendedCreators;

    static FiguresFactory * s_instance;
};

#endif // FIGURESFACTORY_H
