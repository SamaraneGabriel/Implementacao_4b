#include "Pixel.h"

Pixel::Pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
    : x(x), y(y), r(r), g(g), b(b) {};

Pixel::Pixel() : x(0), y(0), r(static_cast<uint8_t>(0)), g(static_cast<uint8_t>(0)), b(static_cast<uint8_t>(0)) {}

int Pixel::getX() { return x; }
int Pixel::getY() { return y; }
uint8_t Pixel::getR() { return r; }
uint8_t Pixel::getG() { return g; }
uint8_t Pixel::getB() { return b; }

float Pixel::getIntensity()
{
    return (0.299*r + 0.587*g + 0.114*b);
}
