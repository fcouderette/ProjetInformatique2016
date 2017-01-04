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
    //std::cout<<"Yo!\n";

    // Get position of clicked pixel inside the scene
    QPointF clickedPosition=mouseEvent->scenePos();

    // To manipulate directly the image : ??
    QList<QGraphicsItem *> listeItems=QGraphicsScene::items();
    if (listeItems.size()==1)
    {
        if (listeItems[0]->type()==7) // 7:graphicsPixmapItem
        {
            // Gets first element of list
            QGraphicsPixmapItem* pixmapitem=(QGraphicsPixmapItem*)listeItems[0];

            // Convert qgraphicspixmpaitem to qimage
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
    //erreur

}
