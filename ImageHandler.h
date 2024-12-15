#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H
#include <string>
#include <vector>
#include "Pixel.h"
#include "Grafo.h"
#include <unordered_map>
using namespace std;

class ImageHandler
{
private:
    string path;
    int width, height, maxValue;

public:
    ImageHandler(const string &path);
    vector<Pixel> loadImage();
    void saveImage(unordered_map<int, int> &hashComponentes, vector<Vertice> &vertices);
    string getPath();
};

#endif