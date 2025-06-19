#include "Aresta.h"

Aresta::Aresta(char id_no_alvo, int peso)
{
    this->id_no_alvo = id_no_alvo;
    this->peso = peso;
}

char Aresta::getIdNoAlvo()
{
    return id_no_alvo;
}

int Aresta::getPeso()
{
    return peso;
}

void Aresta::setIdNoAlvo(char id_no_alvo)
{
    this->id_no_alvo = id_no_alvo;
}

void Aresta::setPeso(int peso)
{
    this->peso = peso;
}

Aresta::~Aresta()
{
    // implments destructor
}