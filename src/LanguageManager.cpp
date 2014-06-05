#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "LanguageManager.h"

LanguageManager * LanguageManager::s_instance = NULL;
const QString LanguageManager::s_baseLanguageId( "English" );
const QString LanguageManager::s_dirName( "languages" );

LanguageManager::LanguageManager()
    : QObject(),
      m_current(NULL),
      m_base(NULL)
{
    //qDebug() << QDir::currentPath();
    QDir dir(QDir::currentPath() + "/" + s_dirName );
    QStringList fileList = dir.entryList(QDir::Files);

    if (fileList.count())
    {
        foreach (const QString & s, fileList)
        {
            QString path = dir.absoluteFilePath(s);
            addLanguageFile(path);
        }
    }

    LanguagesMap::iterator it = m_languages.find( s_baseLanguageId );
    if ( it == m_languages.end() )
        it = m_languages.begin();
    if ( it != m_languages.end() )
        m_base = m_current = &it.value();
}

LanguageManager & LanguageManager::getInstance()
{
    if ( s_instance == NULL )
        s_instance = new LanguageManager();
    return *s_instance;
}

void LanguageManager::setCurrentLanguage(const QString & _current)
{
    LanguagesMap::iterator it = m_languages.find(_current);
    if ( it != m_languages.end() )
    {
        m_current = &it.value();
        emit set_lang();
    }
}


const QString& LanguageManager::operator [] (const QString & _key) const
{
    LanguageMap * languages[] = { m_current, m_base };
    for ( size_t i = 0; i < sizeof( languages ) / sizeof( LanguageMap* ); ++i )
        if ( languages[i] )
        {
            LanguageMap::iterator it = languages[i]->find(_key);
            if (it != languages[i]->end())
                return it.value();
        }
    return _key;
}

QList<QString> LanguageManager::getLanguagesList() const
{
    return m_languages.keys();
}

int LanguageManager::count() const
{
    return m_languages.size();
}


void LanguageManager::addLanguageFile(const QString & _filename)
{
    QChar * str = NULL, * strValue, * key, * strValuePtr;
    QFile file(_filename);
    if (file.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        QString S = ts.readAll();
        file.close();
        if (S.length() < 2)
            return;
        LanguageMap lm;
        str = S.data();

        while (*str != 0)
        {
            while (*str == ' ' || *str == '\n' || *(str) == '\r' || *str == '\t')
                ++str;

            if (*str == '#')
            {
                do
                    ++str;
                while ( *str != 0 && *str != '\n');

                continue;
            }

            if (*(key = str) == 0)
                break;

            while ( *str != ' ' && *str != '\t' && *str != '\n' && *str != '\r' && *str != 0)
                ++str;

            if (*str == 0)
                break;

            *(str++) = 0;

            while (*str != '"' && *str != 0 )
                ++str;

            if (*str == 0)
                break;

            strValuePtr = strValue = ++str;

            while ( *str != '"' && *str != 0 )
            {
                if (*str == '\\' && *(str + 1) != 0)
                {
                    if ( *(str + 1) == 'n' )
                        *(strValuePtr++) = '\n';
                    else
                        *(strValuePtr++) = *(str + 1);

                    str += 2;
                }
                else
                {
                    *(strValuePtr++) = *(str++);
                }

            }
            if (*str != 0)
                ++str;
            *strValuePtr = 0;

         //   qDebug() << "  " << QString(key) << "   " << QString(strValue);
            lm[QString(key)] = QString(strValue);
        }


        m_languages.insert( lm["language_id"], LanguageMap() ).value().swap(lm);      //  delete str;
    }
}
