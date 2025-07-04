#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"

using namespace std;

class No
{

private:
    char id;
    int peso;
    vector<Aresta *> arestas;

public:
    No(char id, int peso, vector<Aresta *> arestas = {});
    ~No();

    char getId();
    int getPeso();
    vector<Aresta *> getArestas();

    void setId(char id);
    void setPeso(int peso);
    void setArestas(vector<Aresta *> arestas);
    void setAresta(Aresta *aresta);
};

#endif // NO_H
