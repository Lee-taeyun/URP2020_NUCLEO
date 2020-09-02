#ifndef REGRESSION_H
#include "mbed.h"

#define ABS(x) ( ((x)<0)?-(x):(x) )
#define MAX_DEPTH 5
#define LEN_OF_DATASET 500

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

Line* getTrendLine(double*, const int, const int);
int getMaxLenOFContainer(int maxDepth);
int sizeofLineContainer(Line* container, int maxLenOfContainer);
double getCurrentValue(double speed, const int maxSpeed, const int lenOfData, 
                        Line* lineContainer, int lenOfContainer);
#endif