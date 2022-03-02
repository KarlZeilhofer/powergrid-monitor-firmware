#include "histogram.h"
#include "screen.h"
#include "stdlib.h"
#include <Arduino.h>

Histogram::Histogram(int32_t min, int32_t delta, int32_t count)
    :binMin(min), binDelta(delta), binCount(count), totalElements(0)
{
    clear();
}

void Histogram::clear()
{
    for(uint8_t b=0; b < binCount; b++){
        bins[b]=0;
    }
    totalElements = 0;
}

void Histogram::addHistogram(Histogram *hist)
{
    if(totalElements >= MaxTotalElements){
        if(nextHistoram != nullptr){
            nextHistoram->addHistogram(this);
        }
        clear();
    }

    for(uint8_t b=0; b < binCount; b++){
        bins[b] += hist->bins[b];
    }
}

void Histogram::add(int32_t value)
{
    if(totalElements >= MaxTotalElements){
        if(nextHistoram != nullptr){
            nextHistoram->addHistogram(this);
        }
        clear();
    }

    int32_t index = (value-binMin)/binDelta;
    if(index < 0){
        bins[0]++;
    }else if(index > binCount){
        bins[binCount-1]++;
    }else{
        bins[index]++;
    }
    totalElements++;
}

void Histogram::printHeader(Screen *screen, uint8_t x, uint8_t y)
{
    char str[10];
    for(uint8_t line=0; line < binCount; line++){
        ltoa(binMin + line*binDelta, str, 10);
        screen->setCursor(x, y+line);
        screen->print(str);
    }
}

void Histogram::printData(Screen *screen, uint8_t x, uint8_t y)
{
    char str[10];
    for(uint8_t line=0; line < binCount; line++){
        ltoa(bins[line], str, 10);
        screen->setCursor(x, y+line);
        screen->print(str);
    }
}

void Histogram::linkHistogram(int32_t maxTotalElements1, Histogram *hist2)
{
    nextHistoram = hist2;
    MaxTotalElements = maxTotalElements1;
}
