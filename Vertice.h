#ifndef VERTICE_H
#define VERTICE_H
#include "Pixel.h"

class Vertice
{
private:
    Pixel pixel;
    int v;

public:
    Vertice(Pixel pixel, int v);
    Pixel getPixel();
    int getV();
    void setPixel(Pixel pixel);
};

#endif