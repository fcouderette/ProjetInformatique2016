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
#include<fstream>

#include<readply.h>



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

    QObject::connect(ui->segment_pushButton, SIGNAL(clicked()), this , SLOT(filterPly()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
}

//RAS
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
}

//RAS
void MainWindow::maskDefinedInterval(QImage img)
{
    mScene.addPixmap(QPixmap::fromImage(img));
}

//RAS
void MainWindow::structurateXml()
{
    QString xmlname = QFileDialog::getSaveFileName(0, QObject::tr("Save color parameters"), "/home",
                                                   QObject::tr("*.xml"));
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

    pColorCriterias->InsertEndChild(pMinInterval);


    tinyxml2::XMLElement * pMaxInterval = xmlDoc.NewElement("maxInterval");

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

    pColorCriterias->InsertEndChild(pMaxInterval);




    xmlDoc.SaveFile(xmlname_good);

}


//RAS
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
    std::cout<<"\npmaxred1_value : "<<pmaxred1_value<<std::endl;

    tinyxml2::XMLElement* pmaxred2 = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "hue2" );
    const char* pmaxred2_value = pmaxred2->GetText();
    std::cout<<"pmaxred2_value : "<<pmaxred2_value<<std::endl;

    tinyxml2::XMLElement* pmaxgreen = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "saturation" );
    const char* pmaxgreen_value = pmaxgreen->GetText();
    std::cout<<"pmaxgreen_value : "<<pmaxgreen_value<<std::endl;

    tinyxml2::XMLElement* pmaxblue = xmlDoc.FirstChildElement( "colorCriterias" )->FirstChildElement( "maxInterval" )->FirstChildElement( "lightness" );
    const char* pmaxblue_value = pmaxblue->GetText();
    std::cout<<"pmaxblue_value : "<<pmaxblue_value<<std::endl;

    char* pEnd;
    mhuemin1 = std::strtof(pminred1_value, &pEnd);
    mhuemin2 = std::strtof(pminred2_value, &pEnd);
    msatmin = std::strtof(pmingreen_value, &pEnd);
    mlightmin = std::strtof(pminblue_value, &pEnd);

    mhuemax1 = std::strtof(pmaxred1_value, &pEnd);
    mhuemax2 = std::strtof(pmaxred2_value, &pEnd);
    msatmax = std::strtof(pmaxgreen_value, &pEnd);
    mlightmax = std::strtof(pmaxblue_value, &pEnd);

    std::cout<<"\npminhue1_float : "<<mhuemin1<<std::endl;
    std::cout<<"pminhue2_float : "<<mhuemin2<<std::endl;
    std::cout<<"pminsat_float : "<<msatmin<<std::endl;
    std::cout<<"pminlight_float : "<<mlightmin<<std::endl;

    std::cout<<"\npmaxhue1_float : "<<mhuemax1<<std::endl;
    std::cout<<"pmaxhue2_float : "<<mhuemax2<<std::endl;
    std::cout<<"pmaxsat_float : "<<msatmax<<std::endl;
    std::cout<<"pmaxlight_float : "<<mlightmax<<std::endl;


}

//RAS
void MainWindow::choosePly()
{
    // Gets back the image's path

    mplypath=QFileDialog::getOpenFileName(this,QObject::tr("Select ply file"), "/home",QObject::tr("XML files (*.ply)"));

    ui->ply_label->setText(mplypath);

}

/*
// Callbacks for ply reading
static int x_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectX.push_back(ply_get_argument_value(argument));
    std::cout<<"Yo ! "<<ply_get_argument_value(argument)<<std::endl;
    return 1;
}

static int y_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectY.push_back(ply_get_argument_value(argument));
    return 1;
}

static int z_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectZ.push_back(ply_get_argument_value(argument));
    return 1;
}

static int r_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectR.push_back(ply_get_argument_value(argument));
    return 1;
}

static int g_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectG.push_back(ply_get_argument_value(argument));
    return 1;
}

static int b_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectB.push_back(ply_get_argument_value(argument));
    return 1;
}

static int alpha_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    mvectA.push_back(ply_get_argument_value(argument));
    return 1;
}
*/


//RAS
void error_cb(p_ply ply, const char *message)
{
    //ply_get_argument_user_data(argument, NULL, &eol);
    printf("\nYou made an Error\n");
}




/*
void MainWindow::filterPly()
{
    QString filtered = QFileDialog::getSaveFileName(0, QObject::tr("Save filtered ply"), "/home", QObject::tr("*.ply"));
    std::string filtered_text = filtered.toUtf8().constData(); //(char*)deux.c_str();
    char* filtered_good = (char*)filtered_text.c_str();

    QString remaining = QFileDialog::getSaveFileName(0, QObject::tr("Save remaining ply"), "/home", QObject::tr("*.ply"));
    std::string remaining_text = remaining.toUtf8().constData(); //(char*)deux.c_str();
    char* remaining_good = (char*)remaining_text.c_str();

    // Opens ply file
    p_ply myply=ply_open(mplypath.toUtf8().constData(),NULL, 0, NULL);

    // Reads header
    int res=ply_read_header(myply);

    // Opens files
    std::fstream filteredFile(filtered_good, std::ios::out | std::ios::trunc);
    std::fstream remainingFile(remaining_good, std::ios::out | std::ios::trunc);

    // If opening is a success
    if(filteredFile && remainingFile)
    {
        int countdown_filtered=0;
        int countdown_remaining=0;


        // Returns number of vertices
        long nbVertices=ply_set_read_cb(myply, "vertex", "x", vertex_cb, NULL, 0);
        printf("Number of vertices : %ld\n", nbVertices);

        std::string useless;
        for(int i=0;i<14;i++)
        {
            getline(myply,useless);
            std::cout<<useless<<std::endl;
        }


        // comment ne pas lire le header ?
        //myply.seek[g | p](??, ios::cur);

        for (int iLine=0; iLine<nbVertices; iLine++)
        {
            //std::cout<<"opening of ply file ok"<<std::endl;

            // Get next line
            // Get rvb
            // Returns value of argument
            //p_ply_element elem=ply_get_next_element(myply, last);
            //int reres=ply_get_element_info(elem, "x", resread);
            //std::cout<<"ply_get_element_info() ended"<<reres<<std::endl;
            //double value=ply_get_argument_value("x");


            // check if rvb->tsl is in interval (members values)
            std::vector<float> vect=convertRGBtoTSL( R, G, B);
            if(vect[0]<mhuemax1 & vect[0]<mhuemax2 & vect[0]>mhuemin1 & vect[0]>mhuemin2 & vect[1]<msatmax &
                    vect[1]>msatmin & vect[2]<mlightmax & vect[2]>mlightmin)
            {
                // +1 vertex
                countdown_filtered+=1;
                // write line in filtered file
                std::string containing;
                getline(myply,containing);  // to change line at the same time as libply
                filteredFile<<containing;


            }
            else
            {
                // +1 vertex
                countdown_remaining+=1;
                // write line in remaining file
                std::string containing;
                getline(myply,containing);   // to change line at the same time as libply
                remainingFile<<containing;

            }


            //get next element...


        }


        // add new header in ply file (count number of lines for number of vertices)
        filteredFile.close();
        remainingFile.close();

    }



    ply_close(myply);
    std::cout<<"filterPly() ended"<<std::endl;
}//filterPly
*/




void MainWindow::filterPly()
{
    std::cout<<"\nfilterPly()"<<std::endl;

    std::vector<float> vectXin;
    std::vector<float> vectYin;
    std::vector<float> vectZin;
    std::vector<int> vectRin;
    std::vector<int> vectGin;
    std::vector<int> vectBin;
    std::vector<int> vectAin;

    std::vector<float> vectXout;
    std::vector<float> vectYout;
    std::vector<float> vectZout;
    std::vector<int> vectRout;
    std::vector<int> vectGout;
    std::vector<int> vectBout;
    std::vector<int> vectAout;

    // Chooses a path for filtered ply
    QString filtered = QFileDialog::getSaveFileName(0, QObject::tr("Save filtered ply"), "/home", QObject::tr("*.ply"));
    std::string filtered_text = filtered.toUtf8().constData(); //(char*)deux.c_str();
    char* filtered_good = (char*)filtered_text.c_str();

    // Chooses a path for remaining ply
    QString remaining = QFileDialog::getSaveFileName(0, QObject::tr("Save remaining ply"), "/home", QObject::tr("*.ply"));
    std::string remaining_text = remaining.toUtf8().constData(); //(char*)deux.c_str();
    char* remaining_good = (char*)remaining_text.c_str();

    // Opens ply file
    p_ply myply=ply_open(mplypath.toUtf8().constData(),NULL, 0, NULL);
    std::cout<<"nom ply lu"<<mplypath.toStdString()<<std::endl;


    // Reads header
    int reshead=ply_read_header(myply);
    std::cout<<"reshead"<<reshead<<std::endl;

    /*
    p_ply filteredPly= ply_create(filtered_good, PLY_ASCII, error_cb, NULL, 0);
    p_ply remainingPly= ply_create(remaining_good, PLY_ASCII, error_cb, NULL, 0);


    // Returns number of vertices
    //long nbVertices=ply_set_read_cb(myply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "x", x_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "y", y_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "z", z_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "red", r_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "green", g_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "blue", b_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "alpha", alpha_cb, NULL, 0);

    ply_read(myply);

    int lenVect=mvectX.size();
    std::cout<<"taille vecteur : "<<mvectX.size()<<std::endl;

    // Loop i on all elements
    for(int i=0; i<lenVect;i++)
    {
        // Count j for elements in color interval
        int j=0;
        // Count k for elements out of color interval
        int k=0;
        std::vector<float>newVector=convertRGBtoTSL(mvectR[i],mvectG[i],mvectB[i]);
        if     (((newVector[0]>=mhuemin1 && newVector[0]<=mhuemax1) ||
                (newVector[0]>=mhuemin2 && newVector[0]<=mhuemax2)) &&
                newVector[1]>=msatmin && newVector[1]<=msatmax &&
                newVector[2]>=mlightmin && newVector[2]<=mlightmax)
        {
            vectXin[j]=mvectX[i];
            vectYin[j]=mvectY[i];
            vectZin[j]=mvectZ[i];
            vectRin[j]=mvectR[i];
            vectGin[j]=mvectG[i];
            vectBin[j]=mvectB[i];
            vectAin[j]=mvectA[i];
            j+=1;
        }
        else
        {
            vectXout[k]=mvectX[i];
            vectYout[k]=mvectY[i];
            vectZout[k]=mvectZ[i];
            vectRout[k]=mvectR[i];
            vectGout[k]=mvectG[i];
            vectBout[k]=mvectB[i];
            vectAout[k]=mvectA[i];
            k+=1;
        }

    }
    std::cout<<"vect in = "<<vectRin[0]<<std::endl;



    //long res= ply_set_read_cb(p_ply ply, const char *element_name, const char *property_name, p_ply_read_cb read_cb,
    //                     void *pdata, long idata);
    //ply_get_element_info("vertex", ,,);

    //int ply_get_argument_property(p_ply_argument argument,"x", nbVertices, 3);

    //int res=ply_get_element_info("vertex",  "x",nbVertices);
    //std::cout<<"res = "<<res<<std::endl;


    // Writing
    p_ply oply = ply_create("output.ply", PLY_LITTLE_ENDIAN, NULL, 0, NULL);



    // add new header in ply file (count number of lines for number of vertices)



   // Writing
   //p_ply ply_create(const char *name, e_ply_storage_mode storage_mode, p_ply_error_cb error_cb, long idata, void *pdata);
   //int ply_add_element(p_ply ply, const char *name, long ninstances);
   //int ply_add_property(p_ply ply, const char *name, e_ply_type type, e_ply_type length_type, e_ply_type value_type);
   //int ply_write_header(p_ply ply);



    ply_close(filteredPly);
    ply_close(remainingPly);
    */
    ply_close(myply);

    std::cout<<"filterPly() ended"<<std::endl;

}//filterPly






