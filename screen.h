#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

class Screen
{
public:
    Screen();
    void clear();
    void print(char* str);
    void setCursor(uint8_t x, uint8_t y);
    void writeToSerial();

private:
    void wrapCursor();


public:
    static const uint8_t Width = 30;
    static const uint8_t Height = 30;

    char data[Width*Height] = {0};
    uint8_t cursorX;
    uint8_t cursorY;
};

#endif // SCREEN_H
