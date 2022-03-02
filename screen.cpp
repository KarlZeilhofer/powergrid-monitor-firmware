#include "screen.h"
#include <string.h>
#include <Arduino.h>

Screen::Screen()
{
    cursorX = 0;
    cursorY = 0;
}

void Screen::clear()
{
    for(uint32_t i=0; i<Width*Height; i++){
        data[i] = ' ';
    }
}

void Screen::print(char *str)
{
    int len = strlen(str);
    uint32_t iStart = cursorY*Width + cursorX;
    uint32_t iStop = iStart + len - 1;
    if(iStart > Width * Height){
        return;
    }

    for(int i = 0; i<len && cursorY < Height; i++){
        if(str[i] != '\n'){
            data[cursorY*Width + cursorX] = str[i];
            cursorX++;

        }else{
            cursorY++;
        }
    }
}

void Screen::setCursor(uint8_t x, uint8_t y)
{
    cursorX = x;
    cursorY = y;
}

void Screen::writeToSerial()
{
    for(uint8_t y=0; y<Height; y++){
        for(uint8_t x=0; x<Width; x++){
            char c = data[y*Width + x];
            if(c == 0){
                c = ' ';
            }
            Serial.write(c);
        }
        Serial.write('\n');
    }
}

void Screen::wrapCursor()
{
    cursorY += cursorX/Width;
    cursorX = cursorX%Width;
}
