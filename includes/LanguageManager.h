#ifndef LANGUAGES_H
#define	LANGUAGES_H

#include <QObject>
#include <QList>

#include "StringMap.h"


class LanguageManager : public QObject
{
    Q_OBJECT
private:
    typedef StringMap<  QString > LanguageMap;
    typedef StringMap< LanguageMap > LanguagesMap;

public:

    static LanguageManager & getInstance();

    void setCurrentLanguage(const QString & _current);

    const QString & operator [] (const QString & _key);
    QList<QString> getLanguagesList() const;
    int count() const;
private:
    LanguageManager();
    ~LanguageManager() {}

    void addLanguageFile(const QString & _filename);

signals:
    void set_lang();

private:

    LanguagesMap m_languages;
    LanguageMap * m_current;
    LanguageMap * m_base;

    static LanguageManager * s_instance;
};

#define LNG (LanguageManager::getInstance())
#endif	/* LANGUAGES_H */

