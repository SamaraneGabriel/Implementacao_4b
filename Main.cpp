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

// void applyProbabilities(const string &inputFile, int x, int y, double &source_capacity, double &sink_capacity)
// {
//     // Construct the Python command
//     string command = "python probabilidade-2.py " + to_string(x) + " " + to_string(y) + " \"" + inputFile + "\"";

//     // Open a pipe to capture the output of the Python script
//     FILE *pipe = popen(command.c_str(), "r");
//     if (!pipe)
//     {
//         cerr << "Error: Unable to open the pipe to execute the Python script." << endl;
//         return;
//     }

//     // Read the output of the Python script
//     char buffer[128];
//     string result;
//     while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
//     {
//         result += buffer;
//     }

//     // Close the pipe
//     int returnCode = pclose(pipe);
//     if (returnCode != 0)
//     {
//         cerr << "Error occurred while executing the Python script." << endl;
//         return;
//     }

//     // Trim any trailing newlines or spaces
//     result.erase(result.find_last_not_of(" \n\r\t") + 1);

//     // Parse the result to extract source_capacity and sink_capacity
//     istringstream iss(result);
//     string source_str, sink_str;
//     if (!(iss >> source_str >> sink_str))
//     {
//         cerr << "Error: Unable to parse the Python script output." << endl;
//         return;
//     }

//     // Handle "inf" or "infinity" cases
//     if (source_str == "inf" || source_str == "infinity")
//     {

//         source_capacity = static_cast<double>(numeric_limits<int>::max());
//     }
//     else
//     {
//         source_capacity = std::stod(source_str);
//     }

//     if (sink_str == "inf" || sink_str == "infinity")
//     {
//         sink_capacity = static_cast<double>(numeric_limits<int>::max());
//     }
//     else
//     {
//         sink_capacity = std::stod(sink_str);
//     }
// }

// void applyProbabilities(const string &json_payload, vector<pair<double, double>> &capacities)
// {
//     string command = "python probabilidade-2.py '" + json_payload + "'";

//     FILE *pipe = popen(command.c_str(), "r");
//     if (!pipe)
//     {
//         cerr << "Error: Unable to open the pipe to execute the Python script." << endl;
//         return;
//     }

//     char buffer[128];
//     string result;
//     while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
//     {
//         result += buffer;
//     }

//     int returnCode = pclose(pipe);
//     if (returnCode != 0)
//     {
//         cerr << "Error occurred while executing the Python script." << endl;
//         return;
//     }

//     stringstream ss(result);
//     string line;
//     while (getline(ss, line))
//     {
//         double source, sink;
//         if (sscanf(line.c_str(), "%lf %lf", &source, &sink) == 2)
//         {
//             capacities.emplace_back(source, sink);
//         }
//         else
//         {
//             cerr << "Error parsing line: " << line << endl;
//         }
//     }
// }

int main()
{

    // string json_payload = R"({"image":"paint.ppm","pixels":[[0,0],[1,1],[2,2],[3,3]]})";

    // // Vetor para armazenar os resultados
    // vector<pair<double, double>> capacities;

    // // Chamar a função
    // applyProbabilities(json_payload, capacities);

    // cout << "Calculou" << endl;
    // // Exibir os resultados
    // for (size_t i = 0; i < capacities.size(); ++i)
    // {
    //     if (capacities[i].first == numeric_limits<double>::infinity())
    //     {
    //         cout << "INFINITO" << endl;
    //     }

    //     cout << "Pixel " << i << ": Source Capacity: " << capacities[i].first
    //          << ", Sink Capacity: " << capacities[i].second << endl;
    // }

    ImageHandler ih("cavalo.ppm");

    vector<Pixel> pixels = ih.loadImage();

    Grafo grafo;

    grafo.geraGrid(pixels);
    // grafo.criaArestas();
    grafo.criarArestasTerminais(ih.getPath());
    cout << "Criou todas arestas" << endl;

    vector<Aresta> arestas = grafo.getArestas();
    for (Aresta &a : arestas)
    {
      cout << a.getU() << " " << a.getV() << " " << a.getW() << endl;
    }

    FordFulkerson ff(grafo);
    vector<int> minCutSet;

    int maxFlow = ff.maxFlowMinCut(0, 2, minCutSet);

    cout << "Fluxo máximo: " << maxFlow << endl;
    cout << "Corte mínimo (vértices no lado do source): ";
    for (int v : minCutSet) {
        if(v % 1000)
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
