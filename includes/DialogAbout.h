#ifndef DIALOGABOUT_H
#define	DIALOGABOUT_H

#include <QDialog>

class QLabel;
class QTextEdit;
class QPushButton;

class DialogAbout : public QDialog
{
    Q_OBJECT
private:
    QLabel * m_labelInfo, * m_labelAuthor;
    QTextEdit * m_browserInfo;
    QPushButton * m_buttonOK;
public:
    DialogAbout();
    virtual ~DialogAbout();

private:

    static const QString s_authorLabelText;
    static const QString s_programmNameText;
    static const QString s_titleText;
    static const QString s_authorName;
    static const QString s_authorEmail;
    static const QString s_mainText;

};

#endif	/* DIALOGABOUT_H */

