#ifndef READPLY_H
#define READPLY_H

#include <iostream>
#include<rply.h>
#include<rplyfile.h>
#include <vector>
#include <math.h>

void readply(const char* name);
std::vector<float> convertRGBtoTSL(int R,int G,int B);


#endif // READPLY_H
