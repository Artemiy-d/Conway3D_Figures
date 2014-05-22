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
#include "DialogTemplates.h"
#include "otherGuiClasses.h"

#include "StringMap.h"
#include "Languages.h"
#include "complect_headers.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    strPen = QObject::tr("Pen");
    modelCollection["Glider"] = new modelPlaner();
    modelCollection["Z-symbol"] = new modelZSymbol();
    modelCollection["X-symbol"] = new modelXSymbol();
    modelCollection["LWSS"] = new modelShip();
    modelCollection["MWSS"] = new modelShip(6);
    modelCollection["HWSS"] = new modelShip(7);
    modelCollection["Pentadecatron"] = new modelPentadecatron();
    modelCollection["Acorn"]  = new modelAcorn();

    currentModelCollection = modelCollection;

    MainWindow mw;
    mw.setGeometry(10,10,1000,650);
    mw.show();

    return app.exec();
}
