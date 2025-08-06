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
    
private:
    static bool eh_adjacente(Grafo *grafo, char vertice1, char vertice2);
    static bool eh_independente(Grafo *grafo, char candidato, const vector<char> &conjunto_dominante);
    static int contar_novos_dominados(Grafo *grafo, char candidato, const unordered_set<char> &vertices_dominados);
    static unordered_set<char> obter_vizinhos(Grafo *grafo, char vertice);
};

#endif // GULOSO_H