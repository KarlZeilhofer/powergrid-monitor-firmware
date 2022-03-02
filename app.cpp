#include "app.h"
#include "screen.h"
#include "histogram.h"
#include <Arduino.h>


// Zeitintervalle für separate Histogramme:
// aktuelle stunde
// letzte Stunde
// vorletzte Stunde
// aktueller Tag
// letzter Tag
// vorletzter Tag
// aktuelle Woche
// letzte Woche
// vorletzte Woche

//              h00    h01    h02    h10     h11     h12     h20      h2      h22
char kopf1 = "aktuel letzte vorlet  aktuel  letzt.  vorlet  aktuel   letzte   vorlet   ";
char kopf2 = "Stunde Stunde Stunde  Tag     Tag     Tag     Woche    Woche    Woche    ";


Histogram h00Volt(184, 2, 47); // 184 V ... 276 V
Histogram h01Volt(184, 2, 47); // 184 V ... 276 V
Histogram h02Volt(184, 2, 47); // 184 V ... 276 V
Histogram h10Volt(184, 2, 47); // 184 V ... 276 V
Histogram h11Volt(184, 2, 47); // 184 V ... 276 V
Histogram h12Volt(184, 2, 47); // 184 V ... 276 V
Histogram h20Volt(184, 2, 47); // 184 V ... 276 V
Histogram h21Volt(184, 2, 47); // 184 V ... 276 V
Histogram h22Volt(184, 2, 47); // 184 V ... 276 V



Histogram h00Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h01Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h02Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h10Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h11Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h12Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h20Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h21Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz
Histogram h22Freq(470, 1, 61); // 47.0 Hz ... 53.0 Hz

Screen screen;

void App::setup(){
    Serial.begin(115200);

    Serial.println("Power Grid Monitor");
    Serial.println("enter v to print voltage histogram");
    Serial.println("enter f to print frequency histogram");

    h00Volt.linkHistogram(50LL*3600,      &h01Volt);
    h01Volt.linkHistogram(50LL*3600,      &h02Volt);
    h02Volt.linkHistogram(50LL*3600,      &h10Volt);

    h10Volt.linkHistogram(50LL*3600*24,   &h11Volt);
    h11Volt.linkHistogram(50LL*3600*24,   &h12Volt);
    h12Volt.linkHistogram(50LL*3600*24,   &h20Volt);

    h20Volt.linkHistogram(50LL*3600*24*7, &h01Volt);
    h21Volt.linkHistogram(50LL*3600*24*7, &h02Volt);
    h22Volt.linkHistogram(50LL*3600*24*7, nullptr);


    h00Freq.linkHistogram(50LL*3600,      &h01Freq);
    h01Freq.linkHistogram(50LL*3600,      &h02Freq);
    h02Freq.linkHistogram(50LL*3600,      &h10Freq);

    h10Freq.linkHistogram(50LL*3600*24,   &h11Freq);
    h11Freq.linkHistogram(50LL*3600*24,   &h12Freq);
    h12Freq.linkHistogram(50LL*3600*24,   &h20Freq);

    h20Freq.linkHistogram(50LL*3600*24*7, &h01Freq);
    h21Freq.linkHistogram(50LL*3600*24*7, &h02Freq);
    h22Freq.linkHistogram(50LL*3600*24*7, nullptr);
}

void App::loop(){
    static int64_t quadSum = 0;
    static int16_t sumCount = 0;
    int16_t x = testSignal();
    int32_t rms;

    static enum Halbwelle{Positiv, Negativ}halbWelle = Positiv;
    static Halbwelle lastHW = Positiv;

    if(halbWelle == Positiv && x < -100){
        halbWelle = Negativ;
    }
    if(halbWelle == Negativ && x > 100){
        halbWelle = Positiv;
    }

    if(lastHW == Negativ && halbWelle == Positiv){
        rms = sqrt(quadSum/sumCount);
        Serial.println(rms);
//        Serial.print(" ");
//        Serial.println(sumCount);
        h00Volt.add(rms);

        quadSum = 0;
        sumCount = 0;
    }

    if(h00Volt.totalElements == 50*3600){

    }

    lastHW = halbWelle;

    quadSum += int64_t(x)*x;
    sumCount++;

    if(Serial.available()){
        if(Serial.read() == 'p'){
            screen.clear();
            h00Volt.printHeader(&screen, 0, 0);
            h00Volt.printData(&screen, 10, 0);
            Serial.println("------");
            screen.writeToSerial();
            Serial.println("------");
        }
    }
}

int32_t App::sqrt(int64_t y)
{
    // initial underestimate, L <= isqrt(y)
    int64_t L = 0;

    while( (L + 1) * (L + 1) <= y )
        L = L + 1;

    return L;
}

// Sinussignal mit ca. 650 Abtastwerte pro Periode
// Amplitude: 325
// Rauschen: 5
int16_t App::testSignal(){
    static int16_t x=3250;
    static int16_t y=0;

    y = y+(x/100);
    x = x-(y/100);

    return (x + (rand()%201-100))/10;
}
