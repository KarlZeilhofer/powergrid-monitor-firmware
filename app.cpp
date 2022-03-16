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


Screen screen;

void App::setup(){
    Serial.begin(115200);

    Serial.println("# Power Grid Monitor                             #");
    Serial.println("# Prints Voltage in V for L1, L2 and L3          #");
    Serial.println("# and frequency in 0.01Hz for each mains period. #");

}

void App::loop(){
    static int64_t quadSum = 0;
    static int16_t sumCount = 0;
    static uint32_t microsStart = 0;
    int16_t x;
    int32_t rms;
    int32_t freq; // in 0.01 Hz

    //x = testSignal();
    x = ((analogRead(A0)-512)*3*229ll)/290;
    // TODO: estimate DC-offset

    static enum Halbwelle{Positiv, Negativ}halbWelle = Positiv;
    static Halbwelle lastHW = Positiv;

    if(halbWelle == Positiv && x < -100){
        halbWelle = Negativ;
    }
    if(halbWelle == Negativ && x > 100){
        halbWelle = Positiv;
    }

    if(lastHW == Negativ && halbWelle == Positiv){
        uint32_t microsEnd = micros();
        uint32_t dt = microsEnd - microsStart;
        microsStart = microsEnd;

        rms = sqrt(quadSum/sumCount);
        Serial.print(rms);
        Serial.print(" ");
        Serial.println(uint16_t(100000000ull/dt));
//        Serial.print(" ");
//        Serial.println(sumCount);

        quadSum = 0;
        sumCount = 0;
    }


    lastHW = halbWelle;

    quadSum += int64_t(x)*x;
    sumCount++;

    if(Serial.available()){
        if(Serial.read() == 'p'){
            screen.clear();
            Serial.println("------");
            screen.writeToSerial();
            Serial.println("------");
        }
    }
}

int32_t App::iSqrt(int64_t y)
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
