/* 
 * File:   Languages.h
 * Author: artyom
 *
 * Created on 14 Ноябрь 2011 г., 18:05
 */

#ifndef LANGUAGES_H
#define	LANGUAGES_H
#include "MyCollection.h"

#include "complect_headers.h"

class Action_lang : public QAction, public MyCollection<QString>
{
    Q_OBJECT
public:
    Action_lang() : QAction((QObject*)NULL) , MyCollection<QString>() {}
    Action_lang(MyCollection<QString> &C) : QAction((QObject*)NULL), MyCollection<QString>(C) {}
    void setText()
    {
        QAction::setText(MyCollection<QString>::operator []("language_name") );
    }

    virtual ~Action_lang() {}

};

class Languages : public QObject
{
    Q_OBJECT
private:
    QList<Action_lang*> lang_list;
    void AddLang(QString &filename, bool base_lang = false);
    Action_lang BaseLang;
    Action_lang * CurrentLang;
public:
    void CreateActions(QMenu * menu, QObject * parent);
    Languages();
    QString & operator [] (char * key);
    virtual ~Languages();
    inline int Count()
    {
        return lang_list.count();
    }
signals:
    void set_lang();
public slots:
    void act_click();
};

extern Languages appLangs;
#define LNG (appLangs)
#endif	/* LANGUAGES_H */

