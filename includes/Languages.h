#ifndef LANGUAGES_H
#define	LANGUAGES_H

#include <QObject>
#include <QList>

#include "StringMap.h"


class Languages : public QObject
{
    Q_OBJECT
private:
    typedef StringMap<  QString > LanguageMap;
    typedef StringMap< LanguageMap > LanguagesMap;

public:

    static Languages & getInstance();

    void setCurrentLanguage(const QString & _current);

    const QString & operator [] (const QString & _key);
    QList<QString> getLanguagesList() const;
    int count() const;
private:
    Languages();
    ~Languages() {}

    void addLanguageFile(const QString & _filename);

signals:
    void set_lang();

private:

    LanguagesMap m_languages;
    LanguageMap * m_current;
    LanguageMap * m_base;

    static Languages * s_instance;
};

#define LNG (Languages::getInstance())
#endif	/* LANGUAGES_H */

