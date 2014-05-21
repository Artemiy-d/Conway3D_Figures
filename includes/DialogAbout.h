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
    QLabel * m_labelInfo, * m_labelAuthor;
    QTextBrowser * m_browserInfo;
    QPushButton * m_buttonOK;
public:
    DialogAbout();
    virtual ~DialogAbout();
};

#endif	/* DIALOGABOUT_H */

