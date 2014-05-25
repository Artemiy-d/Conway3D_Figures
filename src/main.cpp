#include <QApplication>

#include "MainWindow.h"
#include "Scene3D.h"
#include "DialogNewFigure.h"
#include "DialogAbout.h"
#include "DialogSettings.h"
#include "DialogTemplates.h"
#include "otherGuiClasses.h"

#include "StringMap.h"
#include "LanguageManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    strPen = QObject::tr("Pen");
    modelCollection["Glider"] = new ModelPlaner();
    modelCollection["Z-symbol"] = new ModelZSymbol();
    modelCollection["X-symbol"] = new ModelXSymbol();
    modelCollection["LWSS"] = new ModelShip();
    modelCollection["MWSS"] = new ModelShip(6);
    modelCollection["HWSS"] = new ModelShip(7);
    modelCollection["Pentadecatron"] = new ModelPentadecatron();
    modelCollection["Acorn"]  = new ModelAcorn();

    currentModelCollection = modelCollection;

    MainWindow mw;
    mw.setGeometry(10, 10, 1000, 650);
    mw.show();

    return app.exec();
}
