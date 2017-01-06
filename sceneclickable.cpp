#include "sceneclickable.h"
#include<iostream>
#include <string>
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include<QGraphicsPixmapItem>

SceneClickable::SceneClickable(QObject *parent) :
    QGraphicsScene(parent)
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

    std::cout<<"Mask"<<std::endl;


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
            std::cout<<"largeur image="<<imageWidth<<std::endl;
            int imageHeight=myImage.height();
            std::cout<<"hauteur image="<<imageHeight<<std::endl;


            // Color of selected pixel
            QRgb colorMask=QColor(224,44,224).rgb();

            for(int i=0;i<imageWidth;i++)
            {
                for(int j=0;j<imageHeight;j++)
                {
                    QColor colorPix=myImage.pixel(i,j);
                    //vect=MainWindow::convertRGBtoTSL(colorPix.red(),colorPix.green(),colorPix.blue());

                    // color all of the image for now

                    myImage.setPixel(i,j, colorMask);

                }
            }


            // usual return of item in list, error else
            // Convert image from QImage to QPixmap
            pixmapitem->setPixmap(QPixmap::fromImage(myImage));

            return;
        }
    }
}


void SceneClickable::addImage(QPixmap pixmap)
{
    pixmapitem=addPixmap(pixmap);
}

