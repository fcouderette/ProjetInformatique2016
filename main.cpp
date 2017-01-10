#include "mainwindow.h"
#include <QApplication>
#include <tinyxml2.h>
#include <QFileDialog>
#include <iostream>
#include <QString>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*QString xmlname = QFileDialog::getSaveFileName(0, QObject::tr("Save color parameters"), "/home", QObject::tr("*.xml"));
    std::string utf8_text = xmlname.toUtf8().constData();
    std::cout<<utf8_text<<std::endl;*/

    return a.exec();
}
