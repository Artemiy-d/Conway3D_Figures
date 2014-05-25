#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "LanguageManager.h"

LanguageManager * LanguageManager::s_instance = NULL;

LanguageManager::LanguageManager()
    : QObject(),
      m_current(NULL),
      m_base(NULL)
{
    qDebug() << QDir::currentPath();
    QDir dir(QDir::currentPath() + tr("/languages") );
    QStringList sl = dir.entryList(QDir::Files);
 //   QString basefile = tr("BaseLang.txt");
    if (sl.count())
    {
        foreach (QString s, sl)
        {
          //  if (s == basefile)
            {
                QString sum = dir.absoluteFilePath(s);
                addLanguageFile(sum);
            }
        }
    }

    setCurrentLanguage( tr( "Russian" ) );
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

        while (*str!=0)
        {
            while (*str == ' ' || *str == '\n' || *(str) == '\r' || *str == '\t')
                ++str;

            if (*(key = str) == 0)
                break;

            while ( *str != ' ' && *str != 0)
                ++str;

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

            qDebug() << "Add: " << QString(key) << "  " << QString(strValue);
            lm[QString(key)] = QString(strValue);
        }

        m_languages.insert( lm["language_id"], LanguageMap() ).value().swap(lm);      //  delete str;
    }
}
