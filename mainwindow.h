#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "sceneclickable.h"
#include <vector>
#include <tinyxml2.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //bool eventFilter(QObject *obj, QEvent *event);
    std::vector<float> mvectorHSL;
    std::vector<float> mvectorAmpliHSL;


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
    float mValeur_AH;
    float mValeur_AS;
    float mValeur_AL;
    std::vector<float> mselectionInterval;

    float mredmin1;
    float mredmin2;
    float mgreenmin;
    float mbluemin;

    float mredmax1;
    float mredmax2;
    float mgreenmax;
    float mbluemax;

    QString mplypath;





public slots:
    void chooseImage();
    void chooseXml();
    void choosePly();
    void setReferenceColor(QColor coul);
    std::vector<float> setColorAmplitude();
    std::vector<float> convertRGBtoTSL(int R,int G,int B);
    void maskDefinedInterval(QImage img);
    void defineSelection(std::vector<float> vectorHSL,std::vector<float> vectorAmpliHSL);
    std::vector<float> defineSelection2(std::vector<float> vectorHSL,std::vector<float> vectorAmpliHSL);
    void structurateXml();
/*
signals:
    void pressLabel(QColor);
    void changeSelec(std::vector<float>);
*/
};

#endif // MAINWINDOW_H
