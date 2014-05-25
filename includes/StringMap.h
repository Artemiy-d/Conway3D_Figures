#ifndef STRINGMAP_H
#define	STRINGMAP_H


#include <QMap>
#include <QString>

template <typename T>
struct StringMap : public QMap<QString, T> { };

#endif	/* STRINGMAP_H */

