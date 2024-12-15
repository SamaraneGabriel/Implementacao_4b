#ifndef ARESTA_H
#define ARESTA_H

class Aresta
{
private:
    int u;
    int v;
    int w;

public:
    Aresta();
    Aresta(int u, int v, int w);
    int getU();
    int getV();
    int getW();

    void setW(int w);
};

#endif