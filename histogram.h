#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdint.h>

class Screen;

class Histogram
{
public:
    Histogram(int32_t binMin, int32_t binDelta, int32_t binCount);
    void clear();
    void addHistogram(Histogram* hist);
    void add(int32_t value);
    void printHeader(Screen* screen, uint8_t x, uint8_t y);
    void printData(Screen *screen, uint8_t x, uint8_t y);
    void linkHistogram(int32_t maxTotalElements1, Histogram* hist2);

    int32_t binMin;
    int32_t binDelta;
    int32_t binCount;
    static const int32_t MaxCount = 100;
    uint32_t bins[MaxCount];
    uint32_t totalElements;

    uint32_t MaxTotalElements;
    Histogram* nextHistoram;
};


#endif // HISTOGRAM_H
