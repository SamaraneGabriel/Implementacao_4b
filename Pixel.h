#ifndef PIXEL_H
#define PIXEL_H
#include <cstdint>

class Pixel
{
private:
    int x, y;
    uint8_t r, g, b;

public:
    Pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    Pixel();
    int getX();
    int getY();
    uint8_t getR();
    uint8_t getG();
    uint8_t getB();
    float getIntensity();

    void setR(uint8_t r);
    void setG(uint8_t g);
    void setB(uint8_t b);
};

#endif
