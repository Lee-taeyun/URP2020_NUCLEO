#include "filter.h"
Filter::Filter(){
    this->previousData = 0;
    this->isPreviousDataStored = false;
}

double Filter::LPF(double newData, double alpha){
    if(this->isPreviousDataStored){
    //update previous data after LPF with newData
        this->previousData = alpha*this->previousData + (1-alpha)*newData;
        return this->previousData; //return the result of LPF
    }else{
        this->previousData = newData;
        this->isPreviousDataStored = true;
        return newData;
    }
}

void Filter::reset(){
    this->isPreviousDataStored = false;
    this->previousData = 0;
}