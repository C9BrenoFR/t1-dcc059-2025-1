#include "Guloso.h"
#include "GrafoUtils.h"
#include "colors.h"
#include <iostream>
#include <algorithm>
#include <chrono>

using namespace std;

vector<char> Guloso::conjunto_dominante_independente(Grafo *grafo)
{
    auto inicio = chrono::high_resolution_clock::now();
    
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

            if (!GrafoUtils::eh_independente(grafo, id_candidato, conjunto_dominante)) {
                continue;
            }

            int novos_dominados_count = GrafoUtils::contar_novos_dominados(grafo, id_candidato, vertices_dominados);

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
        unordered_set<char> vizinhos = GrafoUtils::obter_vizinhos(grafo, melhor_candidato);
        for (char vizinho : vizinhos) {
            vertices_dominados.insert(vizinho);
        }
    }

    sort(conjunto_dominante.begin(), conjunto_dominante.end());
    
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    
    cout << GREEN << "Conjunto Dominante Independente encontrado: ";
    for (char v : conjunto_dominante) {
        cout << v << " ";
    }
    cout << RESET << endl;
    cout << "Tamanho da solução: " << conjunto_dominante.size() << endl;
    cout << "Tempo de execução: " << duracao.count() << " ms" << endl;
    
    return conjunto_dominante;
}