#include <QDir>
#include <QDebug>

#include <assert.h>

#include "ModelsManager.h"

#include "Models.h"

const QString ModelsManager::s_modelPenName = "Pen";
const QString ModelsManager::s_modelsDirectory = "models";
const QString ModelsManager::s_extension = ".mdl";

ModelsManager::ModelData::ModelData( Model * _m, bool _mainEnabled, bool _activeEnabled ) :
    model(_m), mainEnabled(_mainEnabled), activeEnabled(_activeEnabled)
{ }

void ModelsManager::ModelData::setEnabled( Enabled _main, Enabled _active )
{
    if ( _main == SetTrue )
        mainEnabled = true;
    else if ( _main == SetFalse )
        mainEnabled = false;

    if ( _active == SetTrue )
        activeEnabled = true;
    else if ( _active == SetFalse )
        activeEnabled = false;
}

QString ModelsManager::normalizeName(const QString & _str)
{
    QString result;
    const QChar * s = _str.data();

    while ( s->isSpace() )
        ++s;

    while ( !s->isNull() )
    {
        while ( !s->isNull() && !s->isSpace() )
            result += *(s++);

        while ( s->isSpace() )
            ++s;

        if ( !s->isNull() )
            result += QChar(' ');
    }

    return result;
}

ModelsManager::ModelsManager()
{
    m_modelsMap.insert( "Glider", ModelData( new ModelPlaner() ) );
    m_modelsMap.insert( "Z-symbol", ModelData( new ModelZSymbol() ) );
    m_modelsMap.insert( "X-symbol", ModelData( new ModelXSymbol() ) );
    m_modelsMap.insert( "LWSS", ModelData( new ModelShip() ) );
    m_modelsMap.insert( "MWSS", ModelData( new ModelShip(6) ) );
    m_modelsMap.insert( "HWSS", ModelData( new ModelShip(7) ) );
    m_modelsMap.insert( "Pentadecatron", ModelData( new ModelPentadecatron() ) );
    m_modelsMap.insert( "Acorn", ModelData( new ModelAcorn() ) );


    QDir dir(QDir::currentPath() + "/" + s_modelsDirectory );
    QStringList fileList = dir.entryList( QStringList(  QString("*") + s_extension ), QDir::Files);

    foreach (const QString & s, fileList)
    {
        Model m(0);
        if ( m.openFromFile( dir.absoluteFilePath(s).toLocal8Bit().data() ) )
        {
            addModel(s.mid( 0, s.indexOf( '.' ) ), &m);
        }
    }
}

void ModelsManager::addModel(const QString & _name, const Model * _model, Enabled _main, Enabled _active)
{
    assert( _main || _active );
    ModelsMap::iterator it = m_modelsMap.find( _name );

    if ( it != m_modelsMap.end() )
    {
        delete it.value().model;
        it.value().model = _model->clone();
        it.value().setEnabled( _main, _active );
    }
    else
    {
        m_modelsMap.insert( _name, ModelData( _model->clone(), _main == SetTrue, _active == SetTrue ) );
    }
}

void ModelsManager::setEnabled(const QString & _name, Enabled _main, Enabled _active)
{
    ModelsMap::iterator it = m_modelsMap.find( _name );

    if ( it != m_modelsMap.end() )
    {
        it.value().setEnabled(_main, _active);

        if ( !it.value().activeEnabled && !it.value().mainEnabled )
        {
            delete it.value().model;
            m_modelsMap.erase( it );
        }
    }
}

ModelsManager::~ModelsManager()
{
    for ( ModelsMap::iterator it = m_modelsMap.begin();
          it != m_modelsMap.end(); ++it)
    {
        delete it.value().model;
    }
}

void ModelsManager::saveModel(const QString & _name) const
{
    ModelsMap::const_iterator it = m_modelsMap.find( _name );

    if ( it != m_modelsMap.end() )
    {
        QDir::current().mkdir(s_modelsDirectory);
        it.value().model->saveToFile( (QDir::currentPath() + "/" + s_modelsDirectory + "/" + _name + s_extension).toLocal8Bit().data() );
    }
}

Model * ModelsManager::getModel(const QString & _name) const
{
    ModelsMap::const_iterator it = m_modelsMap.find( _name );

    return it == m_modelsMap.end() ? NULL : it.value().model;
}

QStringList ModelsManager::getActiveModels() const
{
    QStringList result;

    for ( ModelsMap::const_iterator it = m_modelsMap.begin();
          it != m_modelsMap.end(); ++it)
    {
        if ( it.value().activeEnabled )
            result += it.key();
    }
    return result;
}

QStringList ModelsManager::getMainModels() const
{
    QStringList result;

    for ( ModelsMap::const_iterator it = m_modelsMap.begin();
          it != m_modelsMap.end(); ++it)
    {
        if ( it.value().mainEnabled )
            result += it.key();
    }
    return result;
}
