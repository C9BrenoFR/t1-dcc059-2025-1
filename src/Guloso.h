#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <vector>
#include <unordered_set>

using namespace std;

class Guloso
{
public:
    static vector<char> conjunto_dominante_independente(Grafo *grafo);
};

#endif // GULOSO_H