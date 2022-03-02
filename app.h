#ifndef APP_H
#define APP_H
#include <stdint.h>

class App
{
public:
    void setup();
    void loop();
    int32_t sqrt(int64_t x);
    int16_t testSignal();
};

#endif // APP_H
