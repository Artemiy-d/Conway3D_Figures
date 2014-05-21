/* 
 * File:   DialogAbout.cpp
 * Author: artyom
 * 
 * Created on 11 Ноябрь 2011 г., 17:01
 */

#include "DialogAbout.h"

#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>

DialogAbout::DialogAbout()
{
    this->setWindowTitle(tr("About") );
    m_labelInfo = new QLabel(this);
    m_labelInfo->move(20,20);
    m_labelInfo->setText(tr("Conway's game\nModeling of life") );
    m_labelAuthor = new QLabel(this);
    m_labelAuthor->move(20,m_labelInfo->pos().y()+m_labelInfo->height()+20);
    m_labelAuthor->setText(tr("Author:") );
    QFont F;
    F.setBold(true);
    F.setPointSize(10);
    F.setItalic(true);
    m_labelAuthor->setFont(F);
    m_browserInfo = new QTextBrowser(this);
    m_browserInfo->setGeometry(20,
            m_labelAuthor->pos().y()+20,
            200, 50);
    m_browserInfo->setText(tr( "Artyom Dyomin\ntema_2008@meta.ua") );
    m_browserInfo->setReadOnly(true);
    m_buttonOK = new QPushButton(this);
    m_buttonOK->setText(tr("OK"));
    int w = m_browserInfo->pos().x()*2+m_browserInfo->width();
    m_buttonOK->move((w-m_buttonOK->width())/2,m_browserInfo->pos().y()+m_browserInfo->height()+20);
    this->setFixedSize(w,m_buttonOK->pos().y()+m_buttonOK->height()+20);
    connect(m_buttonOK,SIGNAL(clicked()),this,SLOT(close()));
}

DialogAbout::~DialogAbout()
{

}

