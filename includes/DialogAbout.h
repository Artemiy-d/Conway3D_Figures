/* 
 * File:   DialogAbout.h
 * Author: artyom
 *
 * Created on 11 Ноябрь 2011 г., 17:01
 */

#ifndef DIALOGABOUT_H
#define	DIALOGABOUT_H

#include <QDialog>

class QLabel;
class QTextBrowser;
class QPushButton;

class DialogAbout : public QDialog
{
    Q_OBJECT
private:
    QLabel * label_info, * label_author;
    QTextBrowser * info_browser;
    QPushButton * buttonOK;
public:
    DialogAbout();
    virtual ~DialogAbout();
};

#endif	/* DIALOGABOUT_H */

