#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "sceneclickable.h"
#include<vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::MainWindow *ui;
    SceneClickable mScene;
    int mValeur_R;
    int mValeur_G;
    int mValeur_B;
    float mValeur_H;
    float mValeur_S;
    float mValeur_L;
    std::string mValeur_R_str;
    std::string mValeur_G_str;
    std::string mValeur_B_str;
    double mValeur_AH;
    double mValeur_AS;
    double mValeur_AL;
    int colourPixel_R;
    int colourPixel_G;
    int colourPixel_B;


public slots:
    void chooseImage();
    void setReferenceColor(QColor coul);
    void setColorAmplitude();
    std::vector <float> convertRGBtoTSL(int R,int G,int B);
    int getColourOfClickedPixel();
    //void maskSelectedAmplitude();
    //void defineAmplitudeSelection(int Rref, int Gref, int Bref, float ampliT, float ampliS, float ampliL);

signals:
    void pressLabel();
};

#endif // MAINWINDOW_H
