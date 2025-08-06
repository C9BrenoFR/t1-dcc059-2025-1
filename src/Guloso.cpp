#include "Guloso.h"
#include "colors.h"
#include <iostream>
#include <algorithm>

using namespace std;

vector<char> Guloso::conjunto_dominante_independente(Grafo *grafo)
{
    if (grafo->getListaAdj().empty()) {
        cout << RED << "Grafo vazio!" << RESET << endl;
        return {};
    }

    vector<char> conjunto_dominante;
    unordered_set<char> vertices_dominados;
    unordered_set<char> todos_os_vertices;

    for (No* no : grafo->getListaAdj()) {
        todos_os_vertices.insert(no->getId());
    }

    cout << CYAN << "Iniciando algoritmo guloso para Conjunto Dominante Independente..." << RESET << endl;
    cout << "Total de vertices: " << todos_os_vertices.size() << endl << endl;

    while (vertices_dominados.size() < todos_os_vertices.size()) {
        char melhor_candidato = '\0';
        int max_novos_dominados = -1;

        cout << YELLOW << "Iteracao " << (conjunto_dominante.size() + 1) << ":" << RESET << endl;
        cout << "Vertices ja dominados: " << vertices_dominados.size() << "/" << todos_os_vertices.size() << endl;

        for (No* candidato_no : grafo->getListaAdj()) {
            char id_candidato = candidato_no->getId();

            if (vertices_dominados.count(id_candidato) > 0) {
                continue;
            }

            if (!eh_independente(grafo, id_candidato, conjunto_dominante)) {
                continue;
            }

            int novos_dominados_count = contar_novos_dominados(grafo, id_candidato, vertices_dominados);

            cout << "  Candidato " << id_candidato << ": dominaria " << novos_dominados_count << " novos vertices" << endl;

             if ((novos_dominados_count > max_novos_dominados) || ((novos_dominados_count == max_novos_dominados) && (id_candidato < melhor_candidato))) {
                max_novos_dominados = novos_dominados_count;
                melhor_candidato = id_candidato;
            }
        }

        if (melhor_candidato == '\0') {
            cout << RED << "Nenhum candidato valido encontrado. Algoritmo encerrado." << RESET << endl;
            break;
        }

        cout << GREEN << "Selecionado vertice: " << melhor_candidato << 
                " (domina " << max_novos_dominados << " novos vertices)" << RESET << endl;
        
        conjunto_dominante.push_back(melhor_candidato);

        vertices_dominados.insert(melhor_candidato); 
        unordered_set<char> vizinhos = obter_vizinhos(grafo, melhor_candidato);
        for (char vizinho : vizinhos) {
            vertices_dominados.insert(vizinho);
        }
    }

    sort(conjunto_dominante.begin(), conjunto_dominante.end());
    
    cout << GREEN << "Conjunto Dominante Independente encontrado: ";
    for (char v : conjunto_dominante) {
        cout << v << " ";
    }
    cout << RESET << endl;
    
    return conjunto_dominante;
}

bool Guloso::eh_adjacente(Grafo *grafo, char vertice1, char vertice2)
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

bool Guloso::eh_independente(Grafo *grafo, char candidato, const vector<char> &conjunto_dominante)
{
    for (char vertice_dominante : conjunto_dominante) {
        if (eh_adjacente(grafo, candidato, vertice_dominante)) {
            return false;
        }
    }
    return true;
}

int Guloso::contar_novos_dominados(Grafo *grafo, char candidato, const unordered_set<char> &vertices_dominados)
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

unordered_set<char> Guloso::obter_vizinhos(Grafo *grafo, char vertice)
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