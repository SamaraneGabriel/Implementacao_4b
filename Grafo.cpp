#include "Grafo.h"
#include "Aresta.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
using namespace std;

Grafo::Grafo() {}

vector<Vertice> &Grafo::getVertices()
{
    return this->vertices;
}

vector<Aresta> &Grafo::getArestas()
{
    return this->arestas;
}

void Grafo::addAresta(int u, int v, int w)
{
    if (hashArestas.find("(" + to_string(u) + ", " + to_string(v) + ")") != hashArestas.end())
    {
        return;
    }

    Aresta e(u, v, w);
    arestas.push_back(e);

    hashArestas["(" + to_string(u) + ", " + to_string(v) + ")"] = e;
}

void Grafo::addVertice(Vertice v)
{
    vertices.push_back(v);
}

void Grafo::ordenarArestas()
{
    sort(arestas.begin(), arestas.end(), [](Aresta e, Aresta edge)
         { return e.getW() < edge.getW(); });
}

void Grafo::printaGrafo()
{
    cout << "Vértices: " << endl;
    cout << "{";
    for (Vertice v : this->vertices)
    {
        cout << v.getV() << " ";
    }
    cout << "}" << endl;

    cout << "Arestas:" << endl;
    cout << "{";
    for (Aresta aresta : this->arestas)
    {
        cout << "{" << aresta.getU() << ", " << aresta.getV() << ", " << aresta.getW() << "}";
    }
    cout << "}";
}

void Grafo::geraGrid(vector<Pixel> pixels)
{
    int i = 0;
    for (Pixel p : pixels)
    {
        Vertice temp(p, i);
        addVertice(temp);
        i++;
    }
}

void Grafo::criaArestas()
{
    int width = vertices.back().getPixel().getX() + 1;
    int height = vertices.back().getPixel().getY() + 1;
    double sigma = 15.0; // Example value, adjust based on your application

    for (Vertice &v : vertices)
    {
        Pixel p = v.getPixel();
        int x = p.getX();
        int y = p.getY();
        int currentVerticeIndex = v.getV();

        vector<pair<int, int>> vizinhos = {
            {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

        for (auto [dx, dy] : vizinhos)
        {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height)
            {
                int neighborIndex = ny * width + nx;
                Pixel neighborPixel = vertices[neighborIndex].getPixel();

                // Calculate the weight using intensity similarity
                double intensityDiff = p.getIntensity() - neighborPixel.getIntensity();
                double spatialDist = std::sqrt(dx * dx + dy * dy);
                double weight = std::exp(-intensityDiff * intensityDiff / (2 * sigma * sigma)) / spatialDist;

                // Create an edge with the calculated weight
                addAresta(currentVerticeIndex, neighborIndex, static_cast<int>(weight * 1000)); // Scale for integer representation
            }
        }
    }
}

float threshold(int num_componente, int k)
{
    return k / num_componente;
}

float MInt(int componente1, int componente2,
           int card_componente1, int card_componente2,
           unordered_map<int, int> &maioresArestas, int k)
{
    int maior_aresta_componente_1 = maioresArestas[componente1];
    int maior_aresta_componente_2 = maioresArestas[componente2];
    return min((maior_aresta_componente_1 + threshold(card_componente1, k)),
               (maior_aresta_componente_2 + threshold(card_componente2, k)));
}

void applyProbabilities(const string &json_path, vector<pair<double, double>> &capacities)
{
    string command = "python probabilidade-2.py " + json_path;

    cout << "Size of command: " << command.size() << endl;

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        cerr << "Error: Unable to open the pipe to execute the Python script." << endl;
        return;
    }

    char buffer[128];
    string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        result += buffer;
    }

    int returnCode = pclose(pipe);
    if (returnCode != 0)
    {
        cerr << "Error occurred while executing the Python script." << endl;
        return;
    }

    stringstream ss(result);
    string line;
    while (getline(ss, line))
    {
        double source, sink;
        if (sscanf(line.c_str(), "%lf %lf", &source, &sink) == 2)
        {
            capacities.emplace_back(source, sink);
        }
        else
        {
            cerr << "Error parsing line: " << line << endl;
        }
    }
}

void gerarJson(string imagemPath, vector<Vertice> &vertices)
{
    ostringstream json;
    json << "{\"image\": \"" << imagemPath << "\",";
    json << "\"pixels\":[";
    int contador = 1;

    for (Vertice &v : vertices)
    {
        Pixel p = v.getPixel();
        json << "[" << p.getX() << "," << p.getY() << "]";

        if (contador < vertices.size())
        {
            json << ",";
        }

        contador++;
    }

    json << "]}";

    ofstream file("json.json");
    if (file.is_open())
    {
        file.clear();
        file << json.str();
    }
    else
    {
        cout << "Erro ao criar o arquivo json.json" << endl;
    }
}

void Grafo::criarArestasTerminais(string imagemPath)
{
    gerarJson(imagemPath, vertices);
    vector<pair<double, double>> capacities;
    applyProbabilities("json.json", capacities);

    Vertice source = Vertice(Pixel(-1, -1, 0, 0, 0), vertices.size());
    Vertice sink = Vertice(Pixel(-1, -1, 0, 0, 0), vertices.size() + 1);

    addVertice(source);
    addVertice(sink);
    for (Vertice &v : vertices)
    {
        if (v.getPixel().getX() == -1)
        {
            break;
        }

        addAresta(source.getV(), v.getV(), static_cast<int>(capacities[v.getV()].first * 1000));

        addAresta(v.getV(), sink.getV(), static_cast<int>(capacities[v.getV()].second * 1000));
    }
}
