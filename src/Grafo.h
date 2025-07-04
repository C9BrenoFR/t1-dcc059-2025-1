//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <vector>

using namespace std;

class Grafo
{

private:
    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;
    vector<Aresta *> getListaArestaOrdenada(vector<char> id_nos);
    void ordenaListaAresta(vector<Aresta *> &lista, int min, int max);
    void algoritimo_prim(vector<No *> &agm, vector<char> ids_nos, size_t max);

public:
    Grafo(int ordem, string regras, vector<string> lista_vertices, vector<string> lista_arestas);
    Grafo(int ordem, bool in_direcionado, bool in_ponderado_aresta, bool in_ponderado_vertice, vector<No *> lista_adj);
    ~Grafo();

    No *getNoPorId(char id);
    vector<char> fecho_transitivo_direto(int id_no);                // a
    vector<char> fecho_transitivo_indireto(int id_no);              // b
    vector<char> caminho_minimo_dijkstra(int id_no_a, int id_no_b); // c
    vector<char> caminho_minimo_floyd(int id_no, int id_no_b);      // d
    Grafo *arvore_geradora_minima_prim(vector<char> ids_nos);       // e
    Grafo *arvore_geradora_minima_kruskal(vector<char> ids_nos);    // f
    Grafo *arvore_caminhamento_profundidade(int id_no);             // g
    int raio();                                                     // h 1
    int diametro();                                                 // h 2
    vector<char> centro();                                          // h 3
    vector<char> periferia();                                       // h 4
    vector<char> vertices_de_articulacao();                         // i

    vector<No *> getListaAdj();
    int getOrdem();
    bool getInDirecionado();
    bool getInPonderadoAresta();
    bool getInPonderadoVertice();
};

#endif // GRAFO_H
