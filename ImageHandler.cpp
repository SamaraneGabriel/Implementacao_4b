#include "ImageHandler.h"
#include "Grafo.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

ImageHandler::ImageHandler(const string &path) : path(path) {}

vector<Pixel> ImageHandler::loadImage()
{
    ifstream file(path);
    if (!file.is_open())
    {
        throw runtime_error("Erro ao abrir o arquivo da imagem.");
    }

    string formato;
    file >> formato;

    cout << "Formato: " << formato << endl;

    if (formato != "P3")
    {
        throw runtime_error("Formato de imagem não suportado. Apenas PPM é suportado.");
    }

    file >> width >> height >> maxValue;

    cout << "Largura: " << width << endl;
    cout << "Altura: " << height << endl;
    cout << "Valor máximo: " << maxValue << endl;

    if (maxValue != 255)
    {
        throw runtime_error("Valor máximo de pixel diferente de 255.");
    }

    vector<Pixel> pixels;
    int r, g, b;
    int x = 0, y = 0;
    while (file >> r >> g >> b)
    {
        uint8_t red = static_cast<uint8_t>(r);
        uint8_t green = static_cast<uint8_t>(g);
        uint8_t blue = static_cast<uint8_t>(b);

        pixels.emplace_back(Pixel(x, y, red, green, blue));
        if (++x == width)
        {
            x = 0;
            y++;
        }
    }

    file.close();
    return pixels;
}

void aumentarCores(vector<Pixel> &coresPredefinadas, int coresIndex)
{
    int r = (coresIndex * 50) % 256;
    int g = (coresIndex * 80) % 256;
    int b = (coresIndex * 110) % 256;
    coresPredefinadas.emplace_back(Pixel(0, 0, r, g, b));
}

void ImageHandler::saveImage(unordered_map<int, int> &hashComponentes, vector<Vertice> &vertices)
{
    vector<Pixel> coresPredefinadas = {
        Pixel(0, 0, static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(0)),     // Vermelho
        Pixel(0, 0, static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(0)),     // Verde
        Pixel(0, 0, static_cast<uint8_t>(0), static_cast<uint8_t>(0), static_cast<uint8_t>(255)),     // Azul
        Pixel(0, 0, static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(0)),   // Amarelo
        Pixel(0, 0, static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(255)),   // Magenta
        Pixel(0, 0, static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(255)),   // Ciano
        Pixel(0, 0, static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255)), // Branco
    };

    ofstream file("output.ppm");

    if (!file.is_open())
    {
        throw runtime_error("Erro ao criar o arquivo de saída.");
    }

    file << "P3\n";
    file << width << " " << height << "\n";
    file << maxValue << "\n";

    vector<Pixel> pixelsOrdenados(width * height);

    int coresIndex = 0;
    int index = 0;

    int contador = 0;
    unordered_map<int, int> vetor;

    for (const auto &pair : hashComponentes)
    {
        int v = pair.first;
        int componente = pair.second;
        Vertice vertice = vertices[v];
        Pixel pixel = vertice.getPixel();

        if (vetor.count(componente) == 0)
        {
            vetor[componente] = contador;
            contador++;
        }

        if (contador > coresPredefinadas.size())
        {
            aumentarCores(coresPredefinadas, coresIndex);
            coresIndex++;
        }

        pixel = Pixel(pixel.getX(), pixel.getY(), coresPredefinadas[vetor[componente]].getR(), coresPredefinadas[vetor[componente]].getG(), coresPredefinadas[vetor[componente]].getB());

        int posicao = pixel.getY() * width + pixel.getX();
        pixelsOrdenados[posicao] = pixel;
    }

    for (Pixel &pixel : pixelsOrdenados)
    {
        file << static_cast<int>(pixel.getR()) << " "
             << static_cast<int>(pixel.getG()) << " "
             << static_cast<int>(pixel.getB()) << "\n";
    }

    file.close();
    cout << "Imagem final salva como 'output.ppm'." << endl;
}

string ImageHandler::getPath()
{
    return path;
}
