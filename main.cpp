/*
#include <QApplication>
#include <tinyxml2.h>
#include <QFileDialog>
#include <QString>
*/

#include"readply.h"
#include <iostream>



int main(int argc, char *argv[])
{
    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    */

    /*QString xmlname = QFileDialog::getSaveFileName(0, QObject::tr("Save color parameters"), "/home", QObject::tr("*.xml"));
    std::string utf8_text = xmlname.toUtf8().constData();
    std::cout<<utf8_text<<std::endl;*/

    const char *thepath="/home/frederique/Bureau/Reims.ply";
    readply(thepath);

    //return a.exec();
    return 0;
}
