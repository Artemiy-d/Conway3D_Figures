#include <iostream>

#include "FiguresFactory.h"


FiguresFactory * FiguresFactory::s_instance = NULL;

FiguresFactory & FiguresFactory::getInstance()
{
    if (s_instance == NULL)
        s_instance = new FiguresFactory();
    return *s_instance;
}

void FiguresFactory::addCreator( const std::string & _type, IFigureCreatorExtended * _creator )
{
    m_extendedCreators.insert( ExtendedCreators::value_type( _type, _creator ) );
}

Figure * FiguresFactory::createFigure( const std::string & _type ) const
{
    ExtendedCreators::const_iterator found = m_extendedCreators.find( _type );
    if ( found == m_extendedCreators.end() )
    {
        std::cout << "Warn: found == m_extendedCreators.end()" << std::endl;
        return NULL;
    }
    return found->second->createFigure();
}
FiguresFactory::IFigureCreator * FiguresFactory::createFigureCreator(const std::string & _type) const
{
    ExtendedCreators::const_iterator found = m_extendedCreators.find( _type );
    if ( found == m_extendedCreators.end() )
    {
        std::cout << "Warn: found == m_extendedCreators.end()" << std::endl;
        return NULL;
    }
    return found->second->createFigureCreator();
}

std::list<std::string> FiguresFactory::getTypes() const
{
    std::list<std::string> result;
    for ( ExtendedCreators::const_iterator it = m_extendedCreators.begin(); it != m_extendedCreators.end(); ++it )
        result.push_back(it->first);
    return result;
}

void FiguresFactory::reset()
{
    for ( ExtendedCreators::iterator it = m_extendedCreators.begin(); it != m_extendedCreators.end(); ++it )
        it->second->reset();
}

