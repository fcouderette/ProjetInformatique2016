/*
@author: Frederique Couderette
12/2016-01-2017
*/

#include "sceneclickable.h"
#include<mainwindow.h>

#include <iostream>
#include <string>
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QGraphicsPixmapItem>


SceneClickable::SceneClickable(QObject *parent) :
    QGraphicsScene(parent),
    mValeur_H(0),
    mValeur_S(0),
    mValeur_L(0)
{


}

void SceneClickable::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Get position of clicked pixel inside the scene
    QPointF clickedPosition=mouseEvent->scenePos();

    // To manipulate directly the image :
    QList<QGraphicsItem *> listeItems=QGraphicsScene::items();
    if (listeItems.size()==1)
    {
        if (listeItems[0]->type()==7) // 7:graphicsPixmapItem
        {
            // Gets first element of list
            QGraphicsPixmapItem* pixmapitem=(QGraphicsPixmapItem*)listeItems[0];

            // Convert qgraphicspixmapitem to qimage
            QImage myImage=pixmapitem->pixmap().toImage();

            // Get color of clicked pixel inside the scene
            QColor colorPix=myImage.pixel(int(clickedPosition.x()),int(clickedPosition.y()));
            std::cout<<"R="<<colorPix.red()<<" ; G="<<colorPix.green()<<" ; B="<<colorPix.blue()<<std::endl;

            // Emit signal containing the value, the color of clicked pixel
            emit colorSelected(colorPix);

            // Usual return if item in list, error else
            return;
        }
    }
    // Else error
} //end mousePressEvent()


void SceneClickable::maskThings(std::vector<float> vect)
{
    mWorkingPic=mOriginalPic;
    mVect=vect;

    // Gets size of image
    int imageWidth=mWorkingPic.width();
    //std::cout<<"largeur image="<<imageWidth<<std::endl;
    int imageHeight=mWorkingPic.height();
    //std::cout<<"hauteur image="<<imageHeight<<std::endl;

    // Change of color of selected pixel
    QRgb colorMask=QColor(224,44,224).rgb();

    //std::cout<<"bound T- = "<<vect[0]<<std::endl;
    //std::cout<<"bound T+ = "<<vect[1]<<std::endl;

    //std::cout<<"bound T- = "<<vect[2]<<std::endl;
    //std::cout<<"bound T+ = "<<vect[3]<<std::endl;

    //std::cout<<"bound S- = "<<vect[4]<<std::endl;
    //std::cout<<"bound S+ = "<<vect[5]<<std::endl;

    //std::cout<<"bound L- = "<<vect[6]<<std::endl;
    //std::cout<<"bound L+ = "<<vect[7]<<std::endl;

    //int compteur=0;
    for(int iLine=0;iLine<imageWidth;iLine++)
    {
        for(int iCol=0;iCol<imageHeight;iCol++)
        {
            //std::cout<<"Got in loop"<<std::endl;

            // Gets color of pixel
            QColor colorPix=mOriginalPic.pixel(iLine,iCol);
            // Converts color of that pixel
            std::vector<float> localVect=sceneConvertRGBtoTSL(colorPix.red(),colorPix.green(),colorPix.blue());

            // if hue in [huemin;huemax] and saturation in [saturationmin;saturationmax] and and luminosity in [luminositymin;luminositymax]
            if     (((localVect[0]>=vect[0] && localVect[0]<=vect[1]) ||
                    (localVect[0]>=vect[2] && localVect[0]<=vect[3])) &&
                    localVect[1]>=vect[4] && localVect[1]<=vect[5] &&
                    localVect[2]>=vect[6] && localVect[2]<=vect[7])
            {
                //myImage.setPixel(iLine,iCol, colorMask);
                mWorkingPic.setPixel(iLine,iCol, colorMask);
            }
            else
            {}
        }
    }
    pixmapitem->setPixmap(QPixmap::fromImage(mWorkingPic));
} // end maskThings()


void SceneClickable::addImage(QString filename)
{
    mOriginalPic=QImage(filename);
    mWorkingPic=mOriginalPic;
    pixmapitem=addPixmap(QPixmap::fromImage(mWorkingPic));
} // end addImage()


void SceneClickable::fromOneImageToAnother(int stateofbox)
{
    // If checkbox is unenabled
    if(stateofbox==Qt::Unchecked)
    {
        std::cout<<"state 1"<<std::endl;
        maskThings(mVect);
    }
    // If checkbox is enabled
    else if(stateofbox==Qt::Checked)
    {
        std::cout<<"state 0"<<std::endl;
        mWorkingPic=mOriginalPic;
        pixmapitem->setPixmap(QPixmap::fromImage(mWorkingPic));
    }
} // end fromOneImageToAnoher


std::vector<float> SceneClickable::sceneConvertRGBtoTSL(int R,int G,int B)
{
    //std::cout<<"\n*** SceneClickable::sceneConvertRGBtoTSL() ***"<<std::endl;

    //determination of maximum among R,G,B
    //std::cout<<"\nR : "<<R<<", G : "<<G<<", B : "<<B<<std::endl;
    int Max=std::max(std::max(R,G),B);
    int Min=std::min(std::min(R,G),B);

    //std::cout<<"Max : "<<Max<<std::endl;
    //std::cout<<"Min : "<<Min<<std::endl;

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
        //std::cout<<"LuminosityRef : "<<mValeur_L<<"\n"<<std::endl;
    }
    // return a vector with HSL float values
    std::vector<float> vectorHSL {mValeur_H,mValeur_S,mValeur_L};

    return vectorHSL;
} // end sceneConvertRGBtoTSL()











