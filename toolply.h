#ifndef READPLY_H
#define READPLY_H

#include <iostream>
#include<rply.h>
#include<rplyfile.h>
#include <vector>
#include <math.h>
#include"mainwindow.h"

// Reads ply file. Calls writeply when done.
void readply(MainWindow* win,const char* name1, const char* name2,const char* name3);

// Converts RGB to TSL when reading ply point's values
std::vector<float> convertRGBtoTSL(int R,int G,int B);

// Writes new ply file (header + XYZ + RGB)
void writeply(const char* name2, const char* name3);

#endif // READPLY_H
