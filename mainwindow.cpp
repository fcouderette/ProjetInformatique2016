#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <QWidget>
#include <QObject>
#include <QString>
#include <math.h>
#include <tinyxml2.h>
#define TIXML_USE_STL
#include<rply.h>
#include<rplyfile.h>


#include <QMouseEvent>
#include <string>
#include <iostream>





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
    mredmin1(0),
    mredmin2(0),
    mgreenmin(0),
    mbluemin(0),
    mredmax1(0),
    mredmax2(0),
    mgreenmax(0),
    mbluemax(0)
{
    ui->setupUi(this);
    ui->graphicsView_Image->installEventFilter(this);

    // Import image
    QObject::connect(ui->pushButton_SelectImage, SIGNAL(clicked()), this , SLOT(chooseImage()));

    // Displays scene in graphicsView
    ui->graphicsView_Image->setScene(&mScene);

    // Sets the image's size
    ui->graphicsView_Image->scale(0.4,0.4);

    // Get colour
    //QObject::connect(this, SIGNAL(pressLabel(QColor)), this , SLOT(getColourOfClickedPixel(QColor)));

    // Initialize reference colour
    //QObject::connect(this, SIGNAL(pressLabel(QColor)), this , SLOT(setReferenceColor(QColor)));

    QObject::connect(ui->horizontalSlider_AT, SIGNAL(sliderReleased()), this , SLOT(setColorAmplitude()));
    QObject::connect(ui->horizontalSlider_AS, SIGNAL(sliderReleased()), this , SLOT(setColorAmplitude()));
    QObject::connect(ui->horizontalSlider_AL, SIGNAL(sliderReleased()), this , SLOT(setColorAmplitude()));



    //QObject::connect(this, SIGNAL(setColorAmplitude()), this , SLOT(setColorAmplitude()));

    // Connexion of signal from class sceneclickable and method setReferenceColor from mainwindow
    QObject::connect(&mScene, SIGNAL(colorSelected(QColor)), this , SLOT(setReferenceColor(QColor)));

    // Connexion of signal from class sceneclickable and method setReferenceColor from mainwindow
    //QObject::connect(this, SIGNAL(changeSelect(std::vector<float>)), &mScene , SLOT(maskThings(std::vector<float>)));

    // Show unfiltered image when clicked on "Show Original Image" button
    QObject::connect(ui->showOriginalImage_checkBox, SIGNAL(stateChanged(int)), &mScene , SLOT(fromOneImageToAnother(int)));


    //RAS
    QObject::connect(ui->pushButton_Export, SIGNAL(clicked()), this , SLOT(structurateXml()));

    QObject::connect(ui->pushButton_xml, SIGNAL(clicked()), this , SLOT(chooseXml()));
    QObject::connect(ui->pushButton_ply, SIGNAL(clicked()), this , SLOT(choosePly()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
//RAS
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    std::cout<<"eventFilter()"<<std::endl;
    if(obj==ui->graphicsView_Image && event->type()==QEvent::MouseButtonPress)
    {
        emit pressLabel();
        std::cout<<"emit"<<std::endl;
        return true;
    }
    return QObject::eventFilter(obj,event);
}
*/
//RAS
void MainWindow::chooseImage()
{
    std::cout<<"\n*** chooseImage() ***"<<std::endl;

    // Gets back the image's path
    QString chemin_image;
    chemin_image=QFileDialog::getOpenFileName(this,"Select Image", "/home","Images(*.jpg *.png)");

    mScene.clear();
    // Associates image to scene
    mScene.addImage(chemin_image);
}

//RAS
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





    // Changer couleur cadre textEdit_CF
    //ui->textEdit_CF->setTextBackgroundColor(QColor::rgb(mValeur_R,mValeur_G,mValeur_B));




    //mValeur_R=mValeurRef+ui.RedSlider->value();
    setColorAmplitude();
}

//RAS
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
}

//RAS
std::vector<float> MainWindow::setColorAmplitude()
{
    //std::cout<<"\n*** setColorAmplitude() ***"<<std::endl;

    // Put value from slider inside mValeur_AH
    //ui->horizontalSlider_AT->setRange(0, 360-mValeur_H);
    mValeur_AH=ui->horizontalSlider_AT->value();
    std::cout<<"mValeur_AH = "<<mValeur_AH<<std::endl;
    //mValeur_R=mValeurRef+ui.RedSlider->value();

    //ui->horizontalSlider_AS->setRange(0, 100-mValeur_S);
    mValeur_AS=ui->horizontalSlider_AS->value();
    std::cout<<"mValeur_AS = "<<mValeur_AS<<std::endl;

    //ui->horizontalSlider_AL->setRange(0, 100-mValeur_L);
    mValeur_AL=ui->horizontalSlider_AL->value();
    std::cout<<"mValeur_AL = "<<mValeur_AL<<std::endl;

    // Return a vector containing length of selection interval
    mvectorAmpliHSL={mValeur_AH,mValeur_AS,mValeur_AL};

    defineSelection(mvectorHSL,mvectorAmpliHSL);

    // Enables use of export and display of original image
    ui->pushButton_Export->setEnabled(true);
    ui->showOriginalImage_checkBox->setEnabled(true);

    return mvectorAmpliHSL;
}

//RAS
void MainWindow::defineSelection(std::vector<float> vectorHSL,std::vector<float> vectorAmpliHSL)
{
    /*
    std::cout<<"\n*** defineSelection() ***"<<std::endl;
    std::cout<<"bound T = "<<vectorAmpliHSL[0]<<std::endl;
    std::cout<<"bound S = "<<vectorAmpliHSL[1]<<std::endl;
    std::cout<<"bound L = "<<vectorAmpliHSL[2]<<std::endl;
    */
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
    //std::vector<float> selectionIntervalSat{minSat,maxSat};
    //std::vector<float> selectionIntervalLum{minLum,maxLum};

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
}

//RAS
std::vector<float> MainWindow::defineSelection2(std::vector<float> vectorHSL,std::vector<float> vectorAmpliHSL)
    {
        /*
        std::cout<<"\n*** defineSelection() ***"<<std::endl;
        std::cout<<"bound T = "<<vectorAmpliHSL[0]<<std::endl;
        std::cout<<"bound S = "<<vectorAmpliHSL[1]<<std::endl;
        std::cout<<"bound L = "<<vectorAmpliHSL[2]<<std::endl;
        */
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
        //std::vector<float> selectionIntervalSat{minSat,maxSat};
        //std::vector<float> selectionIntervalLum{minLum,maxLum};

        if(minHue>=0 && maxHue<=360)
        {
            //std::vector<float> selectionIntervalHue1{minHue,maxHue};
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
                //std::vector<float> selectionIntervalHue1{0,maxHue};
                //std::vector<float> selectionIntervalHue2{360+minHue,360};
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
                //std::vector<float> selectionIntervalHue1{minHue,360};
                //std::vector<float> selectionIntervalHue2{0,maxHue-360};
                std::vector<float> selectionInterval{
                            minHue, 360,
                            0, maxHue-360,
                            minSat, maxSat,
                            minLum, maxLum};
                return selectionInterval;
            }
        }



    /*
    std::cout<<"bound T- = "<<selectionInterval[0]<<std::endl;
    std::cout<<"bound T+ = "<<selectionInterval[1]<<std::endl;

    std::cout<<"bound S- = "<<selectionInterval[2]<<std::endl;
    std::cout<<"bound S+ = "<<selectionInterval[3]<<std::endl;

    std::cout<<"bound L- = "<<selectionInterval[4]<<std::endl;
    std::cout<<"bound L+ = "<<selectionInterval[5]<<std::endl;
    */



    //emit changeSelec(selectionInterval);

    // maskDefinedInterval

}


//RAS
void MainWindow::maskDefinedInterval(QImage img)
{
    mScene.addPixmap(QPixmap::fromImage(img));


}



//RAS
void MainWindow::structurateXml()
{

    QString xmlname = QFileDialog::getSaveFileName(0, QObject::tr("Save color parameters"), "/home", QObject::tr("*.xml"));
    // Converts QString to string
    std::string xmlname_text = xmlname.toUtf8().constData(); //(char*)deux.c_str();
    // Converts string to char
    char* xmlname_good = (char*)xmlname_text.c_str();
    std::cout<<"xmlname "<<xmlname_good<<std::endl;

    // Initialises document
    tinyxml2::XMLDocument xmlDoc;

    // Creates declaration
    tinyxml2::XMLDeclaration * decl = xmlDoc.NewDeclaration();
    xmlDoc.InsertFirstChild(decl);
    xmlDoc.InsertEndChild(decl);


    // Creates root
    tinyxml2::XMLNode * pColorCriterias = xmlDoc.NewElement("colorCriterias");
    xmlDoc.InsertFirstChild(pColorCriterias);


    tinyxml2::XMLElement * pMinInterval = xmlDoc.NewElement("minInterval");

    tinyxml2::XMLElement * pRedMin1 = xmlDoc.NewElement("red1");
    pRedMin1->SetText(mselectionInterval[0]);
    pMinInterval->InsertEndChild(pRedMin1);

    tinyxml2::XMLElement * pRedMin2 = xmlDoc.NewElement("red2");
    pRedMin2->SetText(mselectionInterval[2]);
    pMinInterval->InsertEndChild(pRedMin2);

    tinyxml2::XMLElement * pGreenMin = xmlDoc.NewElement("green");
    pGreenMin->SetText(mselectionInterval[4]);
    pMinInterval->InsertEndChild(pGreenMin);

    tinyxml2::XMLElement * pBlueMin = xmlDoc.NewElement("blue");
    pBlueMin->SetText(mselectionInterval[6]);
    pMinInterval->InsertEndChild(pBlueMin);

    pColorCriterias->InsertEndChild(pMinInterval);


    tinyxml2::XMLElement * pMaxInterval = xmlDoc.NewElement("maxInterval");

    tinyxml2::XMLElement * pRedMax1 = xmlDoc.NewElement("red1");
    pRedMax1->SetText(mselectionInterval[1]);
    pMaxInterval->InsertEndChild(pRedMax1);

    tinyxml2::XMLElement * pRedMax2 = xmlDoc.NewElement("red2");
    pRedMax2->SetText(mselectionInterval[3]);
    pMaxInterval->InsertEndChild(pRedMax2);

    tinyxml2::XMLElement * pGreenMax = xmlDoc.NewElement("green");
    pGreenMax->SetText(mselectionInterval[5]);
    pMaxInterval->InsertEndChild(pGreenMax);

    tinyxml2::XMLElement * pBlueMax = xmlDoc.NewElement("blue");
    pBlueMax->SetText(mselectionInterval[7]);
    pMaxInterval->InsertEndChild(pBlueMax);

    pColorCriterias->InsertEndChild(pMaxInterval);




    xmlDoc.SaveFile(xmlname_good);

}


//RAS
void MainWindow::chooseXml()
{

    // Gets back the image's path
    QString xmlpath;
    xmlpath=QFileDialog::getOpenFileName(this,QObject::tr("Select xml file"), "/home",QObject::tr("XML files (*.xml)"));

    ui->xml_label->setText(xmlpath);


    // Parses xml
    tinyxml2::XMLDocument xmlDoc;

    xmlDoc.LoadFile(xmlpath.toUtf8().constData());

    tinyxml2::XMLNode* proot = xmlDoc.FirstChildElement("colorCriterias");

    tinyxml2::XMLElement* pminInterval = proot->FirstChildElement("minInterval");


    // Inferior bound
    tinyxml2::XMLElement* pminred1 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "red1" );
    const char* pminred1_value = pminred1->GetText();
    std::cout<<"\npred1_value : "<<pminred1_value<<std::endl;

    tinyxml2::XMLElement* pminred2 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "red2" );
    const char* pminred2_value = pminred2->GetText();
    std::cout<<"pred2_value : "<<pminred2_value<<std::endl;

    tinyxml2::XMLElement* pmingreen = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "green" );
    const char* pmingreen_value = pmingreen->GetText();
    std::cout<<"pmingreen_value : "<<pmingreen_value<<std::endl;

    tinyxml2::XMLElement* pminblue = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "minInterval" )->FirstChildElement( "blue" );
    const char* pminblue_value = pminblue->GetText();
    std::cout<<"pminblue_value : "<<pminblue_value<<std::endl;


    // Inferior bound
    tinyxml2::XMLElement* pmaxred1 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "red1" );
    const char* pmaxred1_value = pmaxred1->GetText();
    std::cout<<"\npmaxred1_value : "<<pmaxred1_value<<std::endl;

    tinyxml2::XMLElement* pmaxred2 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "red2" );
    const char* pmaxred2_value = pmaxred2->GetText();
    std::cout<<"pmaxred2_value : "<<pmaxred2_value<<std::endl;

    tinyxml2::XMLElement* pmaxgreen = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "green" );
    const char* pmaxgreen_value = pmaxgreen->GetText();
    std::cout<<"pmaxgreen_value : "<<pmaxgreen_value<<std::endl;

    tinyxml2::XMLElement* pmaxblue = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "blue" );
    const char* pmaxblue_value = pmaxblue->GetText();
    std::cout<<"pmaxblue_value : "<<pmaxblue_value<<std::endl;

    char* pEnd;
    mredmin1 = std::strtof(pminred1_value, &pEnd);
    mredmin2 = std::strtof(pminred2_value, &pEnd);
    mgreenmin = std::strtof(pmingreen_value, &pEnd);
    mbluemin = std::strtof(pminblue_value, &pEnd);

    mredmax1 = std::strtof(pmaxred1_value, &pEnd);
    mredmax2 = std::strtof(pmaxred2_value, &pEnd);
    mgreenmax = std::strtof(pmaxgreen_value, &pEnd);
    mbluemax = std::strtof(pmaxblue_value, &pEnd);

    std::cout<<"\npminred1_float : "<<mredmin1<<std::endl;
    std::cout<<"pminred2_float : "<<mredmin2<<std::endl;
    std::cout<<"pmingreen_float : "<<mgreenmin<<std::endl;
    std::cout<<"pminblue_float : "<<mbluemin<<std::endl;

    std::cout<<"\npmaxred1_float : "<<mredmax1<<std::endl;
    std::cout<<"pmaxred2_float : "<<mredmax2<<std::endl;
    std::cout<<"pmaxgreen_float : "<<mgreenmax<<std::endl;
    std::cout<<"pmaxblue_float : "<<mbluemax<<std::endl;


}





void MainWindow::choosePly()
{
    // Gets back the image's path

    mplypath=QFileDialog::getOpenFileName(this,QObject::tr("Select ply file"), "/home",QObject::tr("XML files (*.ply)"));

    ui->ply_label->setText(mplypath);

    p_ply myply=ply_open(mplypath.toUtf8().constData(),NULL, 0, NULL);
    //p_ply ply_open(const char *name, p_ply_error_cb error_cb, long idata, void *pdata);

    std::cout<<"Ply read correctly"<<std::endl;

}
