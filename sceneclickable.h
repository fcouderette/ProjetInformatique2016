#ifndef SCENECLICKABLE_H
#define SCENECLICKABLE_H

#include <QGraphicsScene>

class SceneClickable : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SceneClickable(QObject *parent = 0);


signals:
    void colorSelected(QColor);
public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

};

#endif // SCENECLICKABLE_H
