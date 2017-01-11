#ifndef SCENECLICKABLE_H
#define SCENECLICKABLE_H

#include <QGraphicsScene>
#include <vector>
#include <QImage>

class SceneClickable : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SceneClickable(QObject *parent = 0);
    void addImage(QString);
    std::vector<float> sceneConvertRGBtoTSL(int R,int G,int B);


signals:
    void colorSelected(QColor);
public slots:
    void maskThings(std::vector<float> vect);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    QImage mOriginalPic;
    QImage mWorkingPic;
    QGraphicsPixmapItem* pixmapitem;
    void fromOneImageToAnother();

private:
    float mValeur_H;
    float mValeur_S;
    float mValeur_L;

};

#endif // SCENECLICKABLE_H
