#ifndef MYCOLLECTION_H
#define	MYCOLLECTION_H


#include <QMap>
#include <QString>

template <typename T>
struct StringMap : public QMap<QString, T> { };


#endif	/* MYCOLLECTION_H */

