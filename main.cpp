/*
@author: Frederique Couderette
12/2016-01-2017
*/

#include <QApplication>
#include <tinyxml2.h>
#include <QFileDialog>
#include <QString>


#include"toolply.h"
#include <iostream>



int main(int argc, char *argv[])
{
    setlocale(LC_NUMERIC, "C");

    // Forces locale to always use "." as decimal separator
    setenv("LC_NUMERIC","POSIX",1);

    QLocale::setDefault(QLocale::C);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    /*QString xmlname = QFileDialog::getSaveFileName(0, QObject::tr("Save color parameters"), "/home", QObject::tr("*.xml"));
    std::string utf8_text = xmlname.toUtf8().constData();
    std::cout<<utf8_text<<std::endl;*/

/*
    const char *thepath="/home/frederique/Bureau/input.ply";
    readply(thepath);
    */

    return a.exec();
    //return 0;
}
