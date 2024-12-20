#include "Grafo.h"
#include "ImageHandler.h"
#include "Pixel.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include "MaxFlow.cpp"

// Para Compilar use:
// g++ -o programa *.cpp -std=c++17

bool importarBibiliotecas()
{
  string command = "pip install numpy pillow scikit-learn scipy tqdm joblib";

  int result = system(command.c_str());

  if (result != 0)
  {
    return false;
  }
  return true;
}

int main()
{

  if (!importarBibiliotecas())
  {
    cerr << "Erro ao importar as bibiliotecas, certifique que você possua o pip instalado." << endl;
    return 1;
  }

  ImageHandler ih("imagens-ppm/golden.ppm");

  vector<Pixel> pixels = ih.loadImage();

  Grafo grafo;

  grafo.geraGrid(pixels);
  grafo.criarArestasTerminais(ih.getPath());
  cout << "Criou todas arestas" << endl;

  vector<Aresta> arestas = grafo.getArestas();

  MaxFlow mf(grafo);
  unordered_map<int, int> minCutSet;
  cout << grafo.getVertices()[grafo.getVertices().size()].getV() << "  " << grafo.getVertices().size() + 1 << endl;
  int maxFlow = mf.boykovKolmogorovMinCut(grafo.getVertices().size() - 2, grafo.getVertices().size() - 1, minCutSet);

  cout << "Fluxo máximo: " << maxFlow << endl;
  cout << "Corte mínimo (vértices no lado do source): ";

  ih.saveImage(minCutSet, grafo.getVertices());

  return 0;
}
