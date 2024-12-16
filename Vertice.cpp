#include "Vertice.h"
#include "Pixel.h"

Vertice::Vertice(Pixel pixel, int v)
    : pixel(pixel), v(v) {}

Pixel Vertice::getPixel() { return pixel; }
int Vertice::getV() { return v; }
void Vertice::setPixel(Pixel pixel) { this->pixel = pixel; }