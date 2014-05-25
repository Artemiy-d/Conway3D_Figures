#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

#include "DialogAbout.h"

const QString DialogAbout::s_authorLabelText = tr("Author:");
const QString DialogAbout::s_programmNameText = tr("Conway's game\nModeling of life");
const QString DialogAbout::s_titleText = tr("About");
const QString DialogAbout::s_authorName = tr("Artyom Dyomin");
const QString DialogAbout::s_authorEmail = tr("mailto:art.dyomin@gmail.com");
const QString DialogAbout::s_mainText = s_authorName + tr("\n<a href=\"") + s_authorEmail + tr("\">") + s_authorEmail + "</a>";

DialogAbout::DialogAbout()
{
    this->setWindowTitle( s_titleText );
    m_labelInfo = new QLabel(this);
    m_labelInfo->move(20, 20);
    m_labelInfo->setText( s_programmNameText );
    m_labelAuthor = new QLabel(this);
    m_labelAuthor->move(20, m_labelInfo->pos().y() + m_labelInfo->height() + 20);
    m_labelAuthor->setText( s_authorLabelText );
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    font.setItalic(true);
    m_labelAuthor->setFont(font);
    m_browserInfo = new QTextEdit(this);
    m_browserInfo->setGeometry(20,
                        m_labelAuthor->pos().y() + 20,
                        200,
                        50);


    m_browserInfo->setHtml( s_mainText );
    m_browserInfo->setReadOnly(true);
    m_buttonOK = new QPushButton(this);
    m_buttonOK->setText(tr("OK"));
    int w = m_browserInfo->pos().x() * 2 + m_browserInfo->width();
    m_buttonOK->move((w - m_buttonOK->width()) / 2, m_browserInfo->pos().y() + m_browserInfo->height() + 20);
    this->setFixedSize(w, m_buttonOK->pos().y() + m_buttonOK->height() + 20);
    connect(m_buttonOK, SIGNAL(clicked()), this, SLOT(close()));
}

DialogAbout::~DialogAbout()
{

}

