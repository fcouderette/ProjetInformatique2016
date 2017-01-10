#include "mainwindow.h"
#include <QApplication>
#include <tinyxml2.h>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
