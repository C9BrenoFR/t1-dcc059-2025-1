#include "No.h"

No::No(char id, int peso)
{
    this->id = id;
    this->peso = peso;
    arestas = vector<Aresta *>();
}

char No::getId()
{
    return id;
}

int No::getPeso()
{
    return peso;
}

vector<Aresta *> No::getArestas()
{
    return arestas;
}

void No::setId(char id)
{
    this->id = id;
}

void No::setPeso(int peso)
{
    this->peso = peso;
}

void No::setArestas(vector<Aresta *> arestas)
{
    this->arestas = arestas;
}

void No::setAresta(Aresta *aresta)
{
    arestas.emplace_back(aresta);
}

No::~No()
{
    for (Aresta *aresta : arestas)
    {
        delete aresta;
    }
}
