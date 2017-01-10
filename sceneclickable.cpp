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

            // usual return if item in list, error else
            return;
        }
    }
    // Else error
}


void SceneClickable::maskThings(std::vector<float> vect)
{

    //std::cout<<"Mask"<<std::endl;


    // Creates a list of QGraphicsScene objects in scene
    QList<QGraphicsItem *> listeItems=QGraphicsScene::items();
    if (listeItems.size()==1)
    {
        // If only type of list is a graphicsPixmapItem
        if (listeItems[0]->type()==7) // 7:graphicsPixmapItem
        {
            // Gets first element of list
            QGraphicsPixmapItem* pixmapitem=(QGraphicsPixmapItem*)listeItems[0];

            // Convert qgraphicspixmapitem to qimage
            QImage myImage=pixmapitem->pixmap().toImage();

            int imageWidth=myImage.width();
            //std::cout<<"largeur image="<<imageWidth<<std::endl;
            int imageHeight=myImage.height();
            //std::cout<<"hauteur image="<<imageHeight<<std::endl;

            // Creates a temporary image : it will be destroyed when slider value changed
            temporaryImage=myImage;


            // Change of color of selected pixel
            QRgb colorMask=QColor(224,44,224).rgb();
            //std::cout<<"Got color for mask\n"<<std::endl;



            int compteur=0;
            for(int iLine=0;iLine<imageWidth;iLine++)
            {
                for(int iCol=0;iCol<imageHeight;iCol++)
                {
                    //std::cout<<"Got in loop"<<std::endl;

                    // Gets color of pixel
                    QColor colorPix=myImage.pixel(iLine,iCol);
                    // Converts color of that pixel
                    std::vector<float> localVect=sceneConvertRGBtoTSL(colorPix.red(),colorPix.green(),colorPix.blue());

                    //std::vector<float> localVect={colorPix.hslHue(),colorPix.hslSaturation(),colorPix.lightness()};
                    //std::cout<<colorPix.hslHue()<<std::endl;
                    //std::cout<<"colorPix.h : "<<localVect[0]<<std::endl;
                    //std::cout<<"colorPix.s : "<<localVect[1]<<std::endl;
                    //std::cout<<"colorPix.l : "<<localVect[2]<<"\n"<<std::endl;

                    //std::cout<<"Color converted"<<std::endl;

                    //std::cout<<"Intervalle : "<<vect[0]<<" ; "<<vect[1]<<std::endl;
                    //std::cout<<"Intervalle : "<<vect[2]<<" ; "<<vect[3]<<std::endl;
                    //std::cout<<"Intervalle : "<<vect[4]<<" ; "<<vect[5]<<"\n"<<std::endl;

                    // if hue in [huemin;huemax] and saturation in [saturationmin;saturationmax] and and luminosity in [luminositymin;luminositymax]
                    if(localVect[0]>=vect[0] && localVect[0]<=vect[1] && localVect[1]>=vect[2] && localVect[1]<=vect[3] && localVect[2]>=vect[4] && localVect[2]<=vect[5])
                    {
                        compteur+=1;
                        //std::cout<<"********** HELLO compteur pixels : "<<compteur<<std::endl;
                        // color all of the image for now
                        //temporaryImage.setPixel(iLine,iCol, colorMask);

                        //myImage.setPixel(iLine,iCol, colorMask);
                        temporaryImage.setPixel(iLine,iCol, colorMask);
                    }
                    else
                    {
                        //couleur originale
                        //myImage.setPixel(iLine,iCol, myImage.pixel(iLine,iCol).rgb());
                    }



                }
            }


            // usual return of item in list, error else
            // Convert image from QImage to QPixmap

            //pixmapitem->setPixmap(QPixmap::fromImage(myImage));
            pixmapitem->setPixmap(QPixmap::fromImage(temporaryImage));

            return;
        }
    }
}


void SceneClickable::addImage(QPixmap pixmap)
{
    pixmapitem=addPixmap(pixmap);
}

void SceneClickable::fromOneImageToAnother(QPixmap pixmap1, QPixmap pixmap2)
{

    QList<QGraphicsItem *> listeItems=QGraphicsScene::items();


    if (listeItems.size()==2)
    {
        // If only type of list is a graphicsPixmapItem
        if (listeItems[0]->type()==7) // 7:graphicsPixmapItem
        {
            // Gets first element of list
            QGraphicsPixmapItem* pixmapitem=(QGraphicsPixmapItem*)listeItems[0];

            //Comment afficher l'image ??
        }

    }
}




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
}











