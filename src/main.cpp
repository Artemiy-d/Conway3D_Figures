#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.setGeometry(10, 100, 1000, 650);
    mw.show();

    return app.exec();
}
