#include "Grafo.h"
#include "ImageHandler.h"
#include "Pixel.h"
// #include "Segmentacao.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include "FordFulkerson.cpp"

// Para Compilar use:
// g++ -o programa *.cpp -std=c++17

int main()
{

  ImageHandler ih("150x150.ppm");

  vector<Pixel> pixels = ih.loadImage();

  Grafo grafo;

  grafo.geraGrid(pixels);
  // grafo.criaArestas();
  grafo.criarArestasTerminais(ih.getPath());
  cout << "Criou todas arestas" << endl;

  vector<Aresta> arestas = grafo.getArestas();
  // for (Aresta &a : arestas)
  // {
  //   cout << a.getU() << " " << a.getV() << " " << a.getW() << endl;
  // }

  FordFulkerson ff(grafo);
  unordered_map<int, int> minCutSet;
  cout << grafo.getVertices()[grafo.getVertices().size()].getV() << "  " << grafo.getVertices().size() + 1 << endl;
  int maxFlow = ff.boykovKolmogorovMinCut(grafo.getVertices().size() - 2, grafo.getVertices().size() - 1, minCutSet);

  cout << "Fluxo máximo: " << maxFlow << endl;
  cout << "Corte mínimo (vértices no lado do source): ";

  ih.saveImage(minCutSet, grafo.getVertices());

  // cout << endl;

  return 0;
}
