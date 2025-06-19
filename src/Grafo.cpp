#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <iostream>
#include <vector>

using namespace std;

Grafo::Grafo(int ordem, string regras, vector<string> lista_vertices, vector<string> lista_arestas)
{
    this->ordem = ordem;
    in_direcionado = (regras[0] == '1');
    in_ponderado_aresta = (regras[2] == '1');
    in_ponderado_vertice = (regras[3] == '1');

    for (string lista_vertice : lista_vertices)
    {
        No *vertice = new No(lista_vertice[0], in_ponderado_vertice ? lista_vertice[2] - '0' : 0);
        lista_adj.emplace_back(vertice);
    }

    for (string lista_aresta : lista_arestas)
    {
        for (No *vertice : lista_adj)
        {
            if (vertice->getId() == lista_aresta[0])
                vertice->setAresta(new Aresta(lista_aresta[2], in_ponderado_aresta ? lista_aresta[4] - '0' : 0));
            else if (!in_direcionado && vertice->getId() == lista_aresta[2])
                vertice->setAresta(new Aresta(lista_aresta[0], in_ponderado_aresta ? lista_aresta[4] - '0' : 0));
        }
    }
}

vector<No *> Grafo::getListaAdj()
{
    return lista_adj;
}

int Grafo::getOrdem()
{
    return ordem;
}

vector<char> Grafo::fecho_transitivo_direto(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no)
{
    std::cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b)
{
    std::cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b)
{
    std::cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    std::cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    std::cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_caminhamento_profundidade(int id_no)
{
    std::cout << "Metodo nao implementado" << endl;
    return nullptr;
}

int Grafo::raio()
{
    std::cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    std::cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    std::cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    std::cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    std::cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo::~Grafo()
{
    for (No *vertice : lista_adj)
        delete vertice;
}
