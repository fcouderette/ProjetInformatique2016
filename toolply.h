#ifndef READPLY_H
#define READPLY_H

#include <iostream>
#include<rply.h>
#include<rplyfile.h>
#include <vector>
#include <math.h>
#include"mainwindow.h"

void readply(MainWindow* win,const char* name1, const char* name2,const char* name3);
std::vector<float> convertRGBtoTSL(int R,int G,int B);
void writeply(const char* name2, const char* name3);

#endif // READPLY_H
