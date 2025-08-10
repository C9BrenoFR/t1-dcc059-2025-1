#include "GrafoUtils.h"

using namespace std;

bool GrafoUtils::eh_adjacente(Grafo *grafo, char vertice1, char vertice2)
{
    No* no1 = grafo->getNoPorId(vertice1);
    if (!no1) return false;

    for (Aresta* aresta : no1->getArestas()) {
        if (aresta->getIdNoAlvo() == vertice2) {
            return true;
        }
    }

    if (!grafo->getInDirecionado()) {
        No* no2 = grafo->getNoPorId(vertice2);
        if (!no2) return false;
        
        for (Aresta* aresta : no2->getArestas()) {
            if (aresta->getIdNoAlvo() == vertice1) {
                return true;
            }
        }
    }

    return false;
}

bool GrafoUtils::eh_independente(Grafo *grafo, char candidato, const vector<char> &conjunto_dominante)
{
    for (char vertice_dominante : conjunto_dominante) {
        if (eh_adjacente(grafo, candidato, vertice_dominante)) {
            return false;
        }
    }
    return true;
}

int GrafoUtils::contar_novos_dominados(Grafo *grafo, char candidato, const unordered_set<char> &vertices_dominados)
{
    int novos_dominados = 0;
    
    if (vertices_dominados.find(candidato) == vertices_dominados.end()) {
        novos_dominados++;
    }
    
    unordered_set<char> vizinhos = obter_vizinhos(grafo, candidato);
    for (char vizinho : vizinhos) {
        if (vertices_dominados.find(vizinho) == vertices_dominados.end()) {
            novos_dominados++;
        }
    }
    
    return novos_dominados;
}

unordered_set<char> GrafoUtils::obter_vizinhos(Grafo *grafo, char vertice)
{
    unordered_set<char> vizinhos;
    No* no = grafo->getNoPorId(vertice);
    
    if (!no) return vizinhos;
    
    for (Aresta* aresta : no->getArestas()) {
        vizinhos.insert(aresta->getIdNoAlvo());
    }
    
    if (!grafo->getInDirecionado()) {
        for (No* outro_no : grafo->getListaAdj()) {
            for (Aresta* aresta : outro_no->getArestas()) {
                if (aresta->getIdNoAlvo() == vertice) {
                    vizinhos.insert(outro_no->getId());
                }
            }
        }
    }
    
    return vizinhos;
}
