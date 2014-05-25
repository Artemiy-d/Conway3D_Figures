#ifndef GROUPBOXCUSTOM_H
#define GROUPBOXCUSTOM_H

#include <QWidget>

class QPaintEvent;

class GroupBoxCustom : public QWidget
{
public:
    GroupBoxCustom(QWidget * _parent = NULL);
    virtual ~GroupBoxCustom() { }
    void setText(const QString & _text);
    void paintEvent(QPaintEvent * _e);

private:
    QString m_text;
};


#endif // GROUPBOXCUSTOM_H
