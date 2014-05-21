/*
 * File:   main.cpp
 * Author: artyom
 *
 * Created on 1 Ноябрь 2011 г., 1:20
 */
//#include "complect_headers.h"

#include "MainWindow.h"
#include "complect_headers.h"
#include "Scene3D.h"
#include "DialogOfCreation.h"
#include "DialogAbout.h"
#include "DialogSettings.h"
#include "modelTemplates.h"
#include "otherGuiClasses.h"

#include "MyCollection.h"
#include "Languages.h"
#include "complect_headers.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    strPen = QObject::tr("Pen");
    modelCollection.AddItem("Glider",new modelPlaner());
    modelCollection.AddItem("Z-symbol",new modelZSymbol());
    modelCollection.AddItem("X-symbol",new modelXSymbol());
    modelCollection.AddItem("LWSS",new modelShip());
    modelCollection.AddItem("MWSS",new modelShip(6));
    modelCollection.AddItem("HWSS",new modelShip(7));
    modelCollection.AddItem("Pentadecatron",new modelPentadecatron());
    modelCollection.AddItem("Acorn",new modelAcorn());

    ITEM <Model*> * it;
    if ((it = modelCollection.getFirst())!=NULL)
        do
        {
            currentModelCollection.AddItem(it);
        }
    while ((it = modelCollection.getNext())!=NULL);

    MainWindow mw;
    mw.setGeometry(10,10,1000,650);
    mw.show();

    return app.exec();
}
