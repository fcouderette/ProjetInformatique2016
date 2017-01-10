#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <math.h>
#include <tinyxml2.h>

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
    mvectorAmpliHSL({0,0,0})
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
    QObject::connect(this, SIGNAL(pressLabel()), this , SLOT(getColourOfClickedPixel()));

    // Initialize reference colour
    QObject::connect(this, SIGNAL(pressLabel()), this , SLOT(setReferenceColor()));

    QObject::connect(ui->horizontalSlider_AT, SIGNAL(valueChanged(int)), this , SLOT(setColorAmplitude()));
    QObject::connect(ui->horizontalSlider_AS, SIGNAL(valueChanged(int)), this , SLOT(setColorAmplitude()));
    QObject::connect(ui->horizontalSlider_AL, SIGNAL(valueChanged(int)), this , SLOT(setColorAmplitude()));



    //QObject::connect(this, SIGNAL(setColorAmplitude()), this , SLOT(setColorAmplitude()));

    // Connexion of signal from class sceneclickable and method setReferenceColor from mainwindow
    QObject::connect(&mScene, SIGNAL(colorSelected(QColor)), this , SLOT(setReferenceColor(QColor)));

    // Connexion of signal from class sceneclickable and method setReferenceColor from mainwindow
    //QObject::connect(this, SIGNAL(changeSelect(std::vector<float>)), &mScene , SLOT(maskThings(std::vector<float>)));

    // Show unfiltered image when clicked on "Show Original Image" button
    QObject::connect(ui->showOriginalImage_checkBox, SIGNAL(stateChanged()), this , SLOT(fromOneImageToAnother(QPixmap pixmap1, QPixmap pixmap2)
    ));
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

//RAS
void MainWindow::chooseImage()
{
    std::cout<<"\n*** chooseImage() ***"<<std::endl;

    // Gets back the image's path
    QString chemin_image;
    chemin_image=QFileDialog::getOpenFileName(this,"Select Image", "/home","Images(*.jpg *.png)");

    // Associates image to scene
    mScene.addImage(QPixmap(chemin_image));
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

    /*
    mValeur_H=coul.hslHue();
    mValeur_S=coul.hslSaturation();
    mValeur_L=coul.lightness();
    std::cout<<"coul teinte : "<<mValeur_H<<std::endl;
    std::cout<<"coul saturation : "<<mValeur_S<<std::endl;
    std::cout<<"coul luminosite : "<<mValeur_L<<std::endl;
    ui->text_value_H->setText(QString("%1").arg(mValeur_H));
    ui->text_value_S->setText(QString("%1").arg(mValeur_S));
    ui->text_value_L->setText(QString("%1").arg(mValeur_L));
    */

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
    ui->horizontalSlider_AT->setRange(0, 360-mValeur_H);
    mValeur_AH=ui->horizontalSlider_AT->value();
    //std::cout<<"mValeur_AH = "<<mValeur_AH<<std::endl;
    //mValeur_R=mValeurRef+ui.RedSlider->value();

    ui->horizontalSlider_AS->setRange(0, 100-mValeur_S);
    mValeur_AS=ui->horizontalSlider_AS->value();
    //std::cout<<"mValeur_AS = "<<mValeur_AS<<std::endl;

    ui->horizontalSlider_AL->setRange(0, 100-mValeur_L);
    mValeur_AL=ui->horizontalSlider_AL->value();
    //std::cout<<"mValeur_AL = "<<mValeur_AL<<std::endl;

    // Return a vector containing length of selection interval
    mvectorAmpliHSL={mValeur_AH,mValeur_AS,mValeur_AL};

    defineSelection(mvectorHSL,mvectorAmpliHSL);

    //ui->showOriginalImage_checkBox->;
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
    std::vector<float> selectionInterval{vectorHSL[0], vectorHSL[0]+vectorAmpliHSL[0], vectorHSL[1], vectorHSL[1]+vectorAmpliHSL[1], vectorHSL[2], vectorHSL[2]+vectorAmpliHSL[2]};

    /*
    std::cout<<"bound T- = "<<selectionInterval[0]<<std::endl;
    std::cout<<"bound T+ = "<<selectionInterval[1]<<std::endl;

    std::cout<<"bound S- = "<<selectionInterval[2]<<std::endl;
    std::cout<<"bound S+ = "<<selectionInterval[3]<<std::endl;

    std::cout<<"bound L- = "<<selectionInterval[4]<<std::endl;
    std::cout<<"bound L+ = "<<selectionInterval[5]<<std::endl;
    */

    mScene.maskThings(selectionInterval);

    //emit changeSelec(selectionInterval);

    // maskDefinedInterval

}



void MainWindow::maskDefinedInterval(QImage img)
{
    mScene.addPixmap(QPixmap::fromImage(img));

    // useless :
    /*
    // pour tout pixel de l'image
    //for (int x=0, x<XX, x++)
    //{for(int y=0, y<YY,y++){
        // on convertit rvb en tsl :
        //convertRGBtoTSL(int R,int G,int B);
        //si tsl_pixel est dans l'intervalle defini
        //if img.at(x,y){
            //on passe le pixel en rose
            //QImage::setPixelColor(x,y,couleurRose);
        //}}}
    //pbs : comment considerer une etendue de pixels avec Qt ? comment parcourir les pixels ?
    */
}

/*
void MainWindow::writeXmlFile()
{
    // Decide of file's path
    QString filepath="/home/frederique/Bureau/colorParameters.xml";
    filepath=QFileDialog::getSaveFileName(this, tr("Save File"), filepath, tr("XML files (*.xml)"));

    // Declaration and opening of file
    ofstream myFile("filepath", ios::out | ios::trunc);
    // If opening of file is a success
    if(myFile)
    {

        // File is closed
        myFile.close();
    }
    else
        cerr << "Error when opening the file" << endl;
    return 0;
}
*/

/*
void structurateXml( )
{
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild( decl );

    TiXmlElement * colorCriterias = new TiXmlElement( "colorCriterias" );


    TiXmlElement * minInterval = new TiXmlElement("minInterval");

    red = new TiXmlElement("red");
    red->LinkEndChild(new TiXmlText(mValeur_R));
    red->LinkEndChild(red);

    green = new TiXmlElement("green");
    green->LinkEndChild(new TiXmlText( mValeur_G));
    green->LinkEndChild(green);

    blue = new TiXmlElement("blue");
    blue->LinkEndChild(new TiXmlText( mValeur_B));
    blue->LinkEndChild(blue);

    colorCriterias->LinkEndChild(minInterval);

    // For end of coor interval
    TiXmlElement * maxInterval = new TiXmlElement("maxInterval");

    red = new TiXmlElement("red");
    red->LinkEndChild(new TiXmlText(// fin intervalle converti en rgb));
    red->LinkEndChild(red);

    green = new TiXmlElement("green");
    green->LinkEndChild(new TiXmlText(// fin intervalle converti en rgb));
    green->LinkEndChild(green);

    blue = new TiXmlElement("blue");
    blue->LinkEndChild(new TiXmlText(// fin intervalle converti en rgb));
    blue->LinkEndChild(blue);

    // Close maxInterval
    colorCriterias->LinkEndChild(maxInterval);

    // Close colorCriterias
    doc.LinkEndChild(colorCriterias);

    // Save xml file
    doc.SaveFile("colorCriterias.xml");
}
*/
