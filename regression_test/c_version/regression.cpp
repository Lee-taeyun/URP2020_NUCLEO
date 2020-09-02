#include <iostream>
#include <vector>
#define MAX_DEPTH 5 // 2^MAX_DEPTH sections will be gereated
#define LEN_OF_DATASET 500
#define ABS(x) ( ((x)<0)?-(x):(x) )

using namespace std;
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

double testData[LEN_OF_DATASET] = {447.37, 447.22, 447.01, 447.08, 447.12, 447.12, 447.0, 447.16, 447.05, 446.91, 446.94, 447.15, 446.96, 446.83, 447.28, 447.05, 447.03, 447.15, 447.14, 447.18, 447.3, 447.39, 447.23, 447.37, 447.18, 447.34, 447.17, 447.35, 447.43, 447.48, 447.45, 447.43, 447.1, 447.28, 447.43, 447.55, 447.42, 447.42, 447.4, 447.4, 447.48, 447.52, 447.49, 447.48, 447.54, 447.6, 447.9, 448.07, 447.95, 447.93, 447.97, 447.94, 448.01, 447.99, 447.9, 447.89, 447.8, 447.65, 447.8, 447.71, 447.55, 447.72, 447.76, 447.99, 448.01, 448.18, 448.09, 448.18, 448.37, 448.17, 448.29, 448.41, 448.49, 448.37, 448.74, 448.64, 448.65, 448.9, 448.85, 448.71, 448.83, 448.98, 448.9, 449.03, 448.9, 448.9, 448.97, 448.91, 449.02, 449.03, 448.79, 449.04, 449.12, 448.97, 449.13, 448.88, 449.06, 449.16, 449.11, 449.03, 449.04, 448.88, 448.87, 448.87, 448.88, 448.84, 448.66, 448.9, 448.69, 448.56, 448.46, 448.37, 448.52, 448.54, 449.29, 449.26, 449.17, 449.0, 448.99, 449.22, 449.16, 449.75, 450.08, 450.1, 450.12, 449.78, 449.45, 449.2, 448.91, 449.1, 448.83, 449.05, 449.28, 449.91, 450.05, 450.24, 450.37, 450.49, 450.5, 450.51, 450.78, 450.91, 450.85, 450.71, 450.73, 450.77, 450.7, 450.72, 450.86, 450.9, 450.78, 450.84, 451.12, 450.83, 450.93, 450.96, 450.93, 450.99, 451.08, 451.15, 451.25, 451.24, 450.97, 450.96, 451.14, 451.26, 451.24, 451.16, 451.36, 451.09, 451.33, 451.45, 451.42, 451.54, 451.48, 451.38, 451.65, 451.77, 451.93, 451.91, 451.91, 451.83, 451.92, 452.03, 452.14, 452.15, 452.01, 452.06, 452.19, 452.29, 452.28, 452.38, 452.43, 452.49, 452.56, 452.43, 452.53, 452.55, 452.63, 452.68, 452.69, 452.47, 452.56, 452.73, 452.79, 452.9, 452.95, 452.96, 452.92, 453.08, 453.19, 453.1, 452.99, 453.33, 453.29, 453.14, 453.08, 453.07, 453.15, 453.26, 453.19, 453.16, 453.26, 453.16, 453.23, 453.28, 453.36, 453.24, 453.37, 453.45, 453.5, 453.54, 453.51, 453.6, 453.61, 453.68, 453.84, 453.87, 453.81, 453.92, 453.77, 453.83, 453.88, 453.97, 454.17, 454.03, 453.95, 454.07, 453.95, 454.14, 454.23, 453.91, 454.04, 454.09, 454.05, 454.11, 454.15, 454.19, 454.27, 454.37, 454.31, 454.32, 454.36, 454.41, 454.47, 454.57, 454.54, 454.44, 454.47, 454.42, 454.35, 454.24, 454.37, 454.44, 454.57, 454.75, 454.71, 454.66, 454.82, 454.86, 454.72, 454.78, 454.91, 454.8, 454.75, 454.67, 454.66, 454.88, 454.96, 455.08, 455.05, 455.11, 455.14, 455.13, 455.16, 455.08, 455.11, 455.1, 455.14, 455.39, 455.55, 455.53, 455.54, 455.61, 455.65, 455.54, 455.48, 455.42, 455.4, 455.38, 455.57, 455.7, 455.73, 455.72, 455.7, 455.75, 455.51, 455.68, 455.64, 455.71, 455.78, 455.77, 455.69, 455.52, 455.74, 455.88, 455.87, 455.98, 455.98, 456.02, 456.05, 455.94, 455.88, 455.96, 456.06, 456.15, 456.13, 455.99, 456.0, 455.92, 456.11, 456.13, 456.14, 456.06, 455.85, 455.9, 456.23, 456.3, 456.1, 456.28, 456.3, 456.38, 456.29, 456.27, 456.28, 456.41, 456.3, 456.26, 456.31, 456.33, 456.49, 456.56, 456.43, 456.47, 456.51, 456.53, 456.48, 456.62, 456.69, 456.81, 456.76, 456.74, 456.66, 456.75, 456.53, 456.7, 456.77, 456.84, 456.84, 456.71, 456.86, 457.03, 457.0, 456.97, 456.92, 456.88, 457.02, 457.06, 456.98, 457.0, 457.04, 456.9, 456.98, 456.88, 457.07, 457.21, 457.42, 457.27, 457.2, 457.21, 457.15, 457.18, 457.27, 457.48, 457.31, 457.14, 457.25, 457.35, 457.33, 457.26, 457.2, 457.34, 457.45, 457.54, 457.6, 457.69, 457.56, 457.49, 457.68, 457.59, 457.52, 457.62, 457.57, 457.59, 457.58, 457.55, 457.52, 457.47, 457.47, 457.57, 457.68, 457.81, 457.86, 458.08, 458.02, 457.93, 457.95, 457.9, 457.79, 457.77, 457.79, 457.74, 457.83, 457.81, 457.88, 457.86, 457.9, 457.94, 457.92, 457.85, 457.84, 457.78, 457.92, 458.07, 458.05, 457.95, 458.12, 458.24, 458.55, 458.46, 458.71, 458.5, 458.25, 458.15, 457.87, 458.0, 458.23, 458.21, 458.26, 458.33, 458.24, 458.15, 458.11, 458.2, 458.54, 458.28, 458.21, 458.32, 458.23, 458.33, 458.15, 458.17, 458.42, 458.39, 458.46, 458.39, 458.47, 458.47, 458.33, 458.54, 458.4, 458.4, 458.37, 458.25, 458.25, 458.28, 458.27, 458.38, 458.45, 458.4};

double distanceSquare(Line line, unsigned int idx, double y){
    // index of array work like X values
    double numerator, denumerator;    
    numerator = (line.a*idx - y + line.b);
    numerator = numerator*numerator;
    
    denumerator = line.a*line.a +1;
    return numerator/denumerator;
}

Line getLinearRegression(double* points, Range range){
    double xSum, xSumSqure, ySum, xySum;
    unsigned int length = range.end - range.start;
    xSum = 0;
    xSumSqure = 0;
    ySum = 0;
    xySum = 0;
    for(int i=range.start; i < range.end; i++){
        xSum += i;
        xSumSqure += i*i;
        ySum += points[i];
        xySum += i*points[i];
    }
    Line line;
    line.a = (length*xySum - xSum*ySum) / (length*xSumSqure - xSum*xSum);
    line.b = (ySum - line.a*xSum)/length;
    line.x.start = range.start;
    line.x.end = range.end;
    return line;
}
double yFrom(Line line, unsigned int x){
    return line.a*x + line.b;
}

void split3(Line line, double* points, Range range, unsigned int depth, const unsigned int maxDepth,
            Line* lineContainer, int* containerCurrentLen){
    float middleIdx = (range.start+range.end)/2;
    unsigned int needleMinIdx, needleMaxIdx, needleIdx;
    needleMinIdx = range.start;
    needleMaxIdx = range.start;

    for(int i = range.start; i<range.end; i++){
        //fine needleMax, needleMin
        if(distanceSquare(line, needleMinIdx, points[needleMinIdx]) > distanceSquare(line,i,points[i])){
            needleMinIdx = i;
        }
        if(distanceSquare(line, needleMaxIdx, points[needleMaxIdx]) < distanceSquare(line,i,points[i])){
            needleMaxIdx = i;
        }
    }
    if( ABS(middleIdx-needleMaxIdx) >  ABS(middleIdx-needleMinIdx) ){
        needleIdx = needleMinIdx;
    }else{
        needleIdx = needleMaxIdx;
    }

    if( ((needleIdx - range.start) > 1) && ((range.end - needleIdx) > 1) ){
        Range leftRange, rightRange;
        leftRange.start = range.start;
        leftRange.end = needleIdx;
        rightRange.start = needleIdx;
        rightRange.end = range.end;
        Line leftLine = getLinearRegression( points, leftRange);
        Line rightLine = getLinearRegression( points, rightRange);
        if(depth == maxDepth-1){
            cout<<"add Line MAX DEPTH"<<endl;
            cout<<*containerCurrentLen<<endl;
            lineContainer[*containerCurrentLen] = leftLine;
            lineContainer[*containerCurrentLen+1] = rightLine;
            *containerCurrentLen += 2;
        }else{
            split3(leftLine, points, leftRange, depth+1, maxDepth, lineContainer, containerCurrentLen);
            split3(rightLine, points, rightRange, depth+1, maxDepth, lineContainer, containerCurrentLen);
        }
    }else{
        cout<<"add Line"<<endl;
        cout<<*containerCurrentLen<<endl;
        lineContainer[*containerCurrentLen] = line;
        *containerCurrentLen += 1;
    }
}



int main(){
    unsigned int lenOfLines = 1;
    lenOfLines = lenOfLines << MAX_DEPTH;
    Line* lineContainer = (Line*) calloc(lenOfLines, sizeof(Line));
    int containerCurrentLen = 0;

    Range range0;
    range0.start = 0;
    range0.end = LEN_OF_DATASET;
    Line line0 = getLinearRegression(testData, range0);

    split3(line0, testData, range0, 0, MAX_DEPTH, lineContainer, &containerCurrentLen);

  
    realloc((void*) lineContainer, sizeof(Line)*containerCurrentLen);

    for(int i=0; i< containerCurrentLen;i++){
        cout<<" a "<<lineContainer[i].a;
        cout<<" b "<<lineContainer[i].b;
        cout<<" startX " << lineContainer[i].x.start;
        cout<<" endX "<< lineContainer[i].x.end;
        cout<<" startY " << yFrom(lineContainer[i],lineContainer[i].x.start)<<endl;
    }

    free(lineContainer);

    return 0;
}