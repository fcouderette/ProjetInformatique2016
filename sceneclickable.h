#ifndef SCENECLICKABLE_H
#define SCENECLICKABLE_H

#include <QGraphicsScene>

class SceneClickable : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SceneClickable(QObject *parent = 0);
    void addImage(QPixmap);


signals:
    void colorSelected(QColor);
public slots:
    void maskThings(std::vector<float> vect);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    QGraphicsPixmapItem* pixmapitem;

};

#endif // SCENECLICKABLE_H
