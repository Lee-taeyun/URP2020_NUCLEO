#ifndef REGRESSION_H
#include "mbed.h"

#define ABS(x) ( ((x)<0)?-(x):(x) )

typedef struct _Range{
    // start is included
    // end is not included
    unsigned int start;
    unsigned int end;
} Range;

typedef struct _Line{
    // y = ax + b
    double a;
    double b;
    Range x;
} Line;

Line* getTrendLine(double* data, const int lenOfData, const int maxDepth);
int getMaxLenOFContainer(int maxDepth);
int sizeofLineContainer(Line* container, int maxLenOfContainer);
double getCurrentValue(double speed, const int maxSpeed, const int lenOfData, 
                        Line* lineContainer, int lenOfContainer);
#endif