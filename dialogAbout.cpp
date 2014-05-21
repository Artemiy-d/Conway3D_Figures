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
    label_info = new QLabel(this);
    label_info->move(20,20);
    label_info->setText(tr("Conway's game\nModeling of life") );
    label_author = new QLabel(this);
    label_author->move(20,label_info->pos().y()+label_info->height()+20);
    label_author->setText(tr("Author:") );
    QFont F;
    F.setBold(true);
    F.setPointSize(10);
    F.setItalic(true);
    label_author->setFont(F);
    info_browser = new QTextBrowser(this);
    info_browser->setGeometry(20,
            label_author->pos().y()+20,
            200, 50);
    info_browser->setText(tr( "Artyom Dyomin\ntema_2008@meta.ua") );
    info_browser->setReadOnly(true);
    buttonOK = new QPushButton(this);
    buttonOK->setText(tr("OK"));
    int w = info_browser->pos().x()*2+info_browser->width(); 
    buttonOK->move((w-buttonOK->width())/2,info_browser->pos().y()+info_browser->height()+20);
    this->setFixedSize(w,buttonOK->pos().y()+buttonOK->height()+20);
    connect(buttonOK,SIGNAL(clicked()),this,SLOT(close()));
}

DialogAbout::~DialogAbout()
{

}

