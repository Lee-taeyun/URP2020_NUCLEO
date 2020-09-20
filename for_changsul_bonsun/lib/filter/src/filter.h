#ifndef FILTER_H
#define FILTER_H
class Filter{
    private:
        double previousData;
        bool isPreviousDataStored;
    public:
        Filter();
        double LPF(double newData, double alpha);
};

#endif