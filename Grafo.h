#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Vertice.h"
#include "Aresta.h"
class Segmentacao; // Declaração antecipada
using namespace std;

class Grafo
{
private:
    vector<Vertice> vertices;
    vector<Aresta> arestas;
    unordered_map<string, Aresta> hashArestas;

public:
    Grafo();
    void addVertice(Vertice v);
    void addAresta(int u, int v, int w);
    void ordenarArestas();
    void printaGrafo();
    vector<Vertice> &getVertices();
    vector<Aresta> &getArestas();
    void criaArestas();
    void criarArestasTerminais(string imagemPath);
    void geraGrid(vector<Pixel> pixels);
    Segmentacao segmentar(int k);
};

#endif
