/*
@author: Frederique Couderette
12/2016-01-2017
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>
#include <string>
#include <iostream>

#include <tinyxml2.h>
#define TIXML_USE_STL

#include<rply.h>
#include<rplyfile.h>

#include<toolply.h>
#include<fstream>

#include <QMouseEvent>
#include <QWidget>
#include <QObject>
#include <QString>

#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qfiledialog.h>






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mScene(),
    mValeur_R(0),
    mValeur_G(0),
    mValeur_B(0),
    mValeur_AH(0),
    mValeur_AS(0),
    mValeur_AL(0),
    mvectorHSL({0,0,0}),
    mvectorAmpliHSL({0,0,0}),
    mselectionInterval({0,0,0,0,0,0,0,0}),
    mhuemin1(0),
    mhuemin2(0),
    msatmin(0),
    mlightmin(0),
    mhuemax1(0),
    mhuemax2(0),
    msatmax(0),
    mlightmax(0)
{
    ui->setupUi(this);
    ui->graphicsView_Image->installEventFilter(this);

    // Import image
    QObject::connect(ui->pushButton_SelectImage, SIGNAL(clicked()), this , SLOT(chooseImage()));

    // Displays scene in graphicsView
    ui->graphicsView_Image->setScene(&mScene);

    // Sets the image's default size
    ui->graphicsView_Image->scale(0.4,0.4);

    // Connection of sliders
    QObject::connect(ui->horizontalSlider_AT, SIGNAL(sliderReleased()), this , SLOT(setColorAmplitude()));
    QObject::connect(ui->horizontalSlider_AS, SIGNAL(sliderReleased()), this , SLOT(setColorAmplitude()));
    QObject::connect(ui->horizontalSlider_AL, SIGNAL(sliderReleased()), this , SLOT(setColorAmplitude()));

    // Connexion of signal from class sceneclickable and method setReferenceColor from mainwindow
    QObject::connect(&mScene, SIGNAL(colorSelected(QColor)), this , SLOT(setReferenceColor(QColor)));

    // Connection with xheckbox to show unfiltered image
    QObject::connect(ui->showOriginalImage_checkBox, SIGNAL(stateChanged(int)), &mScene , SLOT(fromOneImageToAnother(int)));

    // Connection with button for xml writing
    QObject::connect(ui->pushButton_Export, SIGNAL(clicked()), this , SLOT(structurateXml()));

    // Connections for reading, writing and getting path of xml and ply files
    QObject::connect(ui->pushButton_xml, SIGNAL(clicked()), this , SLOT(chooseXml()));
    QObject::connect(ui->pushButton_ply, SIGNAL(clicked()), this , SLOT(choosePly()));
    QObject::connect(ui->segment_pushButton, SIGNAL(clicked()), this , SLOT(filterPly()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::chooseImage()
{
    std::cout<<"\n*** chooseImage() ***"<<std::endl;

    // Gets back the image's path
    QString chemin_image;
    chemin_image=QFileDialog::getOpenFileName(this,"Select Image", "/home","Images(*.jpg *.png)");

    mScene.clear();
    // Associates image to scene
    mScene.addImage(chemin_image);
} //end chooseImage()


void MainWindow::wheelEvent(QWheelEvent *event)
{
    //holds how the view should position the scene during transformations
    ui->graphicsView_Image->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    // Chooses zoom
    double myScale = 1.15;
    // Zooms in
    if(event->delta() > 0)
    {ui->graphicsView_Image-> scale(myScale, myScale);}
    // Zooms out
    else {ui->graphicsView_Image->scale(1.0 / myScale, 1.0 / myScale);}
} // end wheelEvent()


void MainWindow::setReferenceColor(QColor coul)
{
    std::cout<<"\n*** setReferenceColor() ***"<<std::endl;

    std::cout<<"setReferenceColor()"<<std::endl;
    // Temporaire : initialise RVB
    mValeur_R=coul.red();
    mValeur_G=coul.green();
    mValeur_B=coul.blue();

    // Set new values for RGB (default : (0,0,0))
    ui->text_value_R->setText(QString("%1").arg(mValeur_R));//.toStdString());
    ui->text_value_G->setText(QString("%1").arg(mValeur_G));//.toStdString());
    ui->text_value_B->setText(QString("%1").arg(mValeur_B));//.toStdString());

    // Convert RGB to HSL
    mvectorHSL=convertRGBtoTSL(mValeur_R,mValeur_G,mValeur_B);
    // Setting of value in interface
    ui->text_value_H->setText(QString("%1").arg(mvectorHSL[0]));
    ui->text_value_S->setText(QString("%1").arg(mvectorHSL[1]));
    ui->text_value_L->setText(QString("%1").arg(mvectorHSL[2]));

    //mValeur_R=mValeurRef+ui.RedSlider->value();
    setColorAmplitude();
} // end setReferenceColor()


std::vector<float> MainWindow::convertRGBtoTSL(int R,int G,int B)
{
    std::cout<<"\n*** MainWindow::convertRGBtoTSL() ***"<<std::endl;

    //determination of maximum among R,G,B

    int Max=std::max(std::max(R,G),B);
    int Min=std::min(std::min(R,G),B);

    std::cout<<"Max : "<<Max<<std::endl;
    std::cout<<"Min : "<<Min<<std::endl;



    // Difference Max-Min
    int C=Max-Min;
    //std::cout<<"C = "<<C<<std::endl;

    if (R==255 && G==255 && B==255)
    {
        mValeur_H=0;
        mValeur_S=0;
        mValeur_L=100;
    }
    else if (R==0 && G==0 && B==0){
        mValeur_H=0;
        mValeur_S=0;
        mValeur_L=0;
    }
    else
    {
        // Hue
        // max is red
        if(Max==R && Max!=G && Max!=B){mValeur_H=60*(fmod(abs(float(G-B))/float(C),6));}
        // max is green
        else if(Max==G && Max!=R && Max!=B){mValeur_H=60*(2+(float((B-R)/C)));}
        // max is blue
        else if(Max==B && Max!=G && Max!=R){mValeur_H=60*(4+(float((R-G)/C)));}
        // max is red=blue
        else if(Max==B && Max==R && Max!=G){mValeur_H=60*(4+(float((R-G)/C)));}
        // max is red=green
        else if(Max==G && Max==R && Max!=B){mValeur_H=60*(2+(float((B-R)/C)));}
        // max is blue=green
        else if(Max==G && Max==B && Max!=R){mValeur_H=60*(2+(float((B-R)/C)));}
        // red=green=blue
        else if(Max==G && Max==R && Max==B){mValeur_H=0;}

        //std::cout<<"HueRef : "<<mValeur_H<<std::endl;

        // Saturation
        mValeur_S=100*(float(C)/float(Max));
        //std::cout<<"SaturationRef : "<<mValeur_S<<std::endl;


        // Luminosity
        mValeur_L=100*(float(Max)/255);
        //std::cout<<"LuminosityRef : "<<mValeur_L<<std::endl;
    }



    // return a vector with HSL float values
    std::vector<float> vectorHSL {mValeur_H,mValeur_S,mValeur_L};


    return vectorHSL;
/*
    // Setting of value in interface
    ui->text_value_H->setText(QString("%1").arg(mValeur_H));
    ui->text_value_S->setText(QString("%1").arg(mValeur_S));
    ui->text_value_L->setText(QString("%1").arg(mValeur_L));
*/
} // end convertRGBtoTSL()


std::vector<float> MainWindow::setColorAmplitude()
{
    //std::cout<<"\n*** setColorAmplitude() ***"<<std::endl;

    // Put value from slider inside mValeur_AH
    //ui->horizontalSlider_AT->setRange(0, 360-mValeur_H);
    mValeur_AH=ui->horizontalSlider_AT->value();
    //std::cout<<"mValeur_AH = "<<mValeur_AH<<std::endl;


    //ui->horizontalSlider_AS->setRange(0, 100-mValeur_S);
    mValeur_AS=ui->horizontalSlider_AS->value();
    //std::cout<<"mValeur_AS = "<<mValeur_AS<<std::endl;

    //ui->horizontalSlider_AL->setRange(0, 100-mValeur_L);
    mValeur_AL=ui->horizontalSlider_AL->value();
    //std::cout<<"mValeur_AL = "<<mValeur_AL<<std::endl;

    // Return a vector containing length of selection interval
    mvectorAmpliHSL={mValeur_AH,mValeur_AS,mValeur_AL};

    defineSelection(mvectorHSL,mvectorAmpliHSL);

    // Enables use of export and display of original image
    ui->pushButton_Export->setEnabled(true);
    ui->showOriginalImage_checkBox->setEnabled(true);

    return mvectorAmpliHSL;
} // end setColorAmplitude()


void MainWindow::defineSelection(std::vector<float> vectorHSL,std::vector<float> vectorAmpliHSL)
{
    // Create a vector containing interval of selection bounds

    // Bounds
    float minHue=vectorHSL[0]-vectorAmpliHSL[0];
    float maxHue=vectorHSL[0]+vectorAmpliHSL[0];

    float minSat=vectorHSL[1]-vectorAmpliHSL[1];
    if(minSat<0){minSat=0;}
    float maxSat=vectorHSL[1]+vectorAmpliHSL[1];
    if(maxSat>100){maxSat=100;}

    float minLum=vectorHSL[2]-vectorAmpliHSL[2];
    if(minLum<0){minLum=0;}
    float maxLum=vectorHSL[2]+vectorAmpliHSL[2];
    if(maxLum>100){maxLum=100;}

    // Intervals
    if(minHue>=0 && maxHue<=360)
    {
        //std::vector<float> selectionIntervalHue1{minHue,maxHue};
        std::vector<float> selectionInterval{
                    minHue, maxHue,
                    minHue, maxHue,
                    minSat, maxSat,
                    minLum, maxLum};
        mScene.maskThings(selectionInterval);
    }
    else
    {
        if(minHue<0)
        {
            //std::vector<float> selectionIntervalHue1{0,maxHue};
            //std::vector<float> selectionIntervalHue2{360+minHue,360};
            std::vector<float> selectionInterval{
                        0, maxHue,
                        360+minHue, 360,
                        minSat, maxSat,
                        minLum, maxLum
                        };
            mScene.maskThings(selectionInterval);
        }
        else if(maxHue>360)
        {
            //std::vector<float> selectionIntervalHue1{minHue,360};
            //std::vector<float> selectionIntervalHue2{0,maxHue-360};
            std::vector<float> selectionInterval{
                        minHue, 360,
                        0, maxHue-360,
                        minSat, maxSat,
                        minLum, maxLum};
            mScene.maskThings(selectionInterval);
        }
    }

    mselectionInterval=defineSelection2(vectorHSL,vectorAmpliHSL);
} // end defineSelection()


std::vector<float> MainWindow::defineSelection2(std::vector<float> vectorHSL,std::vector<float> vectorAmpliHSL)
    {
        // Create a vector containing interval of selection bounds

        // Bounds
        float minHue=vectorHSL[0]-vectorAmpliHSL[0];
        float maxHue=vectorHSL[0]+vectorAmpliHSL[0];

        float minSat=vectorHSL[1]-vectorAmpliHSL[1];
        if(minSat<0){minSat=0;}
        float maxSat=vectorHSL[1]+vectorAmpliHSL[1];
        if(maxSat>100){maxSat=100;}

        float minLum=vectorHSL[2]-vectorAmpliHSL[2];
        if(minLum<0){minLum=0;}
        float maxLum=vectorHSL[2]+vectorAmpliHSL[2];
        if(maxLum>100){maxLum=100;}

        // Intervals
        if(minHue>=0 && maxHue<=360)
        {
            std::vector<float> selectionInterval{
                        minHue, maxHue,
                        minHue, maxHue,
                        minSat, maxSat,
                        minLum, maxLum};
            return selectionInterval;
        }
        else
        {
            if(minHue<0)
            {
                std::vector<float> selectionInterval{
                            0, maxHue,
                            360+minHue, 360,
                            minSat, maxSat,
                            minLum, maxLum
                            };
                return selectionInterval;
            }
            else if(maxHue>360)
            {
                std::vector<float> selectionInterval{
                            minHue, 360,
                            0, maxHue-360,
                            minSat, maxSat,
                            minLum, maxLum};
                return selectionInterval;
            }
        }
} // end defineSelection2()


void MainWindow::maskDefinedInterval(QImage img)
{
    mScene.addPixmap(QPixmap::fromImage(img));
} // end maskDefinedInterval()


void MainWindow::structurateXml()
{
    QString xmlname = QFileDialog::getSaveFileName(0, QObject::tr("Save color parameters"), "/home",
                                                   QObject::tr("*.xml"));
    // Converts QString to string
    std::string xmlname_text = xmlname.toUtf8().constData(); //(char*)deux.c_str();
    // Converts string to char
    char* xmlname_good = (char*)xmlname_text.c_str();
    //std::cout<<"xmlname "<<xmlname_good<<std::endl;

    // Initialises document
    tinyxml2::XMLDocument xmlDoc;

    // Creates declaration
    tinyxml2::XMLDeclaration * decl = xmlDoc.NewDeclaration();
    xmlDoc.InsertFirstChild(decl);
    xmlDoc.InsertEndChild(decl);

    // Creates root
    tinyxml2::XMLNode * pColorCriterias = xmlDoc.NewElement("colorCriterias");
    xmlDoc.InsertFirstChild(pColorCriterias);

    // Creates minimum bounds of interval
    tinyxml2::XMLElement * pMinInterval = xmlDoc.NewElement("minInterval");

    // Creates elements
    tinyxml2::XMLElement * pRedMin1 = xmlDoc.NewElement("hue1");
    pRedMin1->SetText(mselectionInterval[0]);
    pMinInterval->InsertEndChild(pRedMin1);

    tinyxml2::XMLElement * pRedMin2 = xmlDoc.NewElement("hue2");
    pRedMin2->SetText(mselectionInterval[2]);
    pMinInterval->InsertEndChild(pRedMin2);

    tinyxml2::XMLElement * pGreenMin = xmlDoc.NewElement("saturation");
    pGreenMin->SetText(mselectionInterval[4]);
    pMinInterval->InsertEndChild(pGreenMin);

    tinyxml2::XMLElement * pBlueMin = xmlDoc.NewElement("lightness");
    pBlueMin->SetText(mselectionInterval[6]);
    pMinInterval->InsertEndChild(pBlueMin);

    // End of minimum set
    pColorCriterias->InsertEndChild(pMinInterval);

    // Creates maximum bounds of interval
    tinyxml2::XMLElement * pMaxInterval = xmlDoc.NewElement("maxInterval");

    // Creates elements
    tinyxml2::XMLElement * pRedMax1 = xmlDoc.NewElement("hue1");
    pRedMax1->SetText(mselectionInterval[1]);
    pMaxInterval->InsertEndChild(pRedMax1);

    tinyxml2::XMLElement * pRedMax2 = xmlDoc.NewElement("hue2");
    pRedMax2->SetText(mselectionInterval[3]);
    pMaxInterval->InsertEndChild(pRedMax2);

    tinyxml2::XMLElement * pGreenMax = xmlDoc.NewElement("saturation");
    pGreenMax->SetText(mselectionInterval[5]);
    pMaxInterval->InsertEndChild(pGreenMax);

    tinyxml2::XMLElement * pBlueMax = xmlDoc.NewElement("lightness");
    pBlueMax->SetText(mselectionInterval[7]);
    pMaxInterval->InsertEndChild(pBlueMax);

    // End of maximum set
    pColorCriterias->InsertEndChild(pMaxInterval);

    // Save xml file
    xmlDoc.SaveFile(xmlname_good);

} // end structurateXml()


void MainWindow::chooseXml()
{
    // Gets back the image's path
    QString xmlpath;
    xmlpath=QFileDialog::getOpenFileName(this,QObject::tr("Select xml file"), "/home",
                                         QObject::tr("XML files (*.xml)"));

    ui->xml_label->setText(xmlpath);


    // Parses xml
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile(xmlpath.toUtf8().constData());
    tinyxml2::XMLNode* proot = xmlDoc.FirstChildElement("colorCriterias");
    tinyxml2::XMLElement* pminInterval = proot->FirstChildElement("minInterval");


    // Inferior bound
    tinyxml2::XMLElement* pminred1 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "hue1" );
    const char* pminred1_value = pminred1->GetText();
    std::cout<<"\npred1_value : "<<pminred1_value<<std::endl;

    tinyxml2::XMLElement* pminred2 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "hue2" );
    const char* pminred2_value = pminred2->GetText();
    std::cout<<"pred2_value : "<<pminred2_value<<std::endl;

    tinyxml2::XMLElement* pmingreen = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "saturation" );
    const char* pmingreen_value = pmingreen->GetText();
    std::cout<<"pmingreen_value : "<<pmingreen_value<<std::endl;

    tinyxml2::XMLElement* pminblue = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "lightness" );
    const char* pminblue_value = pminblue->GetText();
    std::cout<<"pminblue_value : "<<pminblue_value<<std::endl;


    // Inferior bound
    tinyxml2::XMLElement* pmaxred1 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "hue1" );
    const char* pmaxred1_value = pmaxred1->GetText();
    //std::cout<<"\npmaxred1_value : "<<pmaxred1_value<<std::endl;

    tinyxml2::XMLElement* pmaxred2 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "hue2" );
    const char* pmaxred2_value = pmaxred2->GetText();
    //std::cout<<"pmaxred2_value : "<<pmaxred2_value<<std::endl;

    tinyxml2::XMLElement* pmaxgreen = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "saturation" );
    const char* pmaxgreen_value = pmaxgreen->GetText();
    //std::cout<<"pmaxgreen_value : "<<pmaxgreen_value<<std::endl;

    tinyxml2::XMLElement* pmaxblue = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "lightness" );
    const char* pmaxblue_value = pmaxblue->GetText();
    //std::cout<<"pmaxblue_value : "<<pmaxblue_value<<std::endl;

    char* pEnd;
    mhuemin1 = std::strtof(pminred1_value, &pEnd);
    mhuemin2 = std::strtof(pminred2_value, &pEnd);
    msatmin = std::strtof(pmingreen_value, &pEnd);
    mlightmin = std::strtof(pminblue_value, &pEnd);

    mhuemax1 = std::strtof(pmaxred1_value, &pEnd);
    mhuemax2 = std::strtof(pmaxred2_value, &pEnd);
    msatmax = std::strtof(pmaxgreen_value, &pEnd);
    mlightmax = std::strtof(pmaxblue_value, &pEnd);

    /*
    std::cout<<"\npminhue1_float : "<<mhuemin1<<std::endl;
    std::cout<<"pminhue2_float : "<<mhuemin2<<std::endl;
    std::cout<<"pminsat_float : "<<msatmin<<std::endl;
    std::cout<<"pminlight_float : "<<mlightmin<<std::endl;

    std::cout<<"\npmaxhue1_float : "<<mhuemax1<<std::endl;
    std::cout<<"pmaxhue2_float : "<<mhuemax2<<std::endl;
    std::cout<<"pmaxsat_float : "<<msatmax<<std::endl;
    std::cout<<"pmaxlight_float : "<<mlightmax<<std::endl;
*/

} // end chooseXml()


void MainWindow::choosePly()
{
    // Gets back the file's path

    mplypath=QFileDialog::getOpenFileName(this,QObject::tr("Select ply file"), "/home",QObject::tr("XML files (*.ply)"));

    ui->ply_label->setText(mplypath);

} //end choosePly()


void MainWindow::filterPly()
{
    std::cout<<"\nfilterPly()"<<std::endl;

    // Chooses a path for filtered ply
    QString filtered = QFileDialog::getSaveFileName(0, QObject::tr("Save filtered ply"), "/home", QObject::tr("*.ply"));
    std::string filtered_text = filtered.toUtf8().constData();
    char* filtered_good = (char*)filtered_text.c_str();

    // Chooses a path for remaining ply
    QString remaining = QFileDialog::getSaveFileName(0, QObject::tr("Save remaining ply"), "/home", QObject::tr("*.ply"));
    std::string remaining_text = remaining.toUtf8().constData();
    char* remaining_good = (char*)remaining_text.c_str();

    readply(this,mplypath.toStdString().c_str(), filtered_good, remaining_good);

    std::cout<<"filterPly() ended"<<std::endl;


} //end filterPly()






