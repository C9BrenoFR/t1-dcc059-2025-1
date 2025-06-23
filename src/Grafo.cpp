#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

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

No *Grafo::getNoPorId(char id)
{
    for (No *no : lista_adj)
    {
        if (no->getId() == id)
            return no;
    }
    return nullptr;
}

vector<char> Grafo::fecho_transitivo_direto(int id_no)
{
    char id_char = id_no;
    No *inicio = getNoPorId(id_char);
    if (!inicio)
        return {};

    vector<char> visitados;
    stack<No *> pilha;
    pilha.push(inicio);

    while (!pilha.empty())
    {
        No *atual = pilha.top();
        pilha.pop();

        bool ja_visitado = false;
        for (char v : visitados)
        {
            if (v == atual->getId())
            {
                ja_visitado = true;
                break;
            }
        }
        if (ja_visitado)
            continue;

        visitados.push_back(atual->getId());

        for (Aresta *aresta : atual->getArestas())
        {
            No *proximo = getNoPorId(aresta->getIdNoAlvo());
            if (proximo)
            {
                bool visitado = false;
                for (char v : visitados)
                {
                    if (v == proximo->getId())
                    {
                        visitado = true;
                        break;
                    }
                }
                if (!visitado)
                {
                    pilha.push(proximo);
                }
            }
        }
    }

    vector<char> resultado;
    for (char c : visitados)
    {
        if (c != id_char)
        {
            resultado.push_back(c);
        }
    }
    return resultado;
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no)
{
    char id_char = id_no;
    No *alvo = getNoPorId(id_char);
    if (!alvo)
        return {};

    vector<char> visitados;
    stack<No *> pilha;
    pilha.push(alvo);

    while (!pilha.empty())
    {
        No *atual = pilha.top();
        pilha.pop();

        bool ja_visitado = false;
        for (char v : visitados)
        {
            if (v == atual->getId())
            {
                ja_visitado = true;
                break;
            }
        }
        if (ja_visitado)
            continue;

        visitados.push_back(atual->getId());

        for (No *no : lista_adj)
        {
            for (Aresta *aresta : no->getArestas())
            {
                if (aresta->getIdNoAlvo() == atual->getId())
                {
                    bool visitado = false;
                    for (char v : visitados)
                    {
                        if (v == no->getId())
                        {
                            visitado = true;
                            break;
                        }
                    }
                    if (!visitado)
                    {
                        pilha.push(no);
                    }
                }
            }
        }
    }

    vector<char> resultado;
    for (char c : visitados)
    {
        if (c != id_char)
        {
            resultado.push_back(c);
        }
    }
    return resultado;
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
    char id_char = static_cast<char>(id_no);
    No *inicio = getNoPorId(id_char);
    if (!inicio)
        return nullptr;

    unordered_set<char> marcado;
    vector<char> visitados;
    vector<string> arestas_arvore;
    stack<pair<No *, char>> pilha;
    pilha.push({inicio, '\0'});

    while (!pilha.empty())
    {
        auto [atual, pai] = pilha.top();
        pilha.pop();

        char id_atual = atual->getId();
        if (marcado.count(id_atual))
            continue;

        marcado.insert(id_atual);
        visitados.push_back(id_atual);
        if (pai != '\0')
        {
            arestas_arvore.emplace_back(
                string(1, pai) + " " + string(1, id_atual));
        }

        auto adj = atual->getArestas();
        for (size_t i = adj.size(); i-- > 0;)
        {
            No *proximo = getNoPorId(adj[i]->getIdNoAlvo());
            if (proximo && !marcado.count(proximo->getId()))
            {
                pilha.push({proximo, id_atual});
            }
        }
    }

    vector<string> vertices_arvore;
    for (char id : visitados)
    {
        No *no = getNoPorId(id);
        string s(1, id);
        if (in_ponderado_vertice)
            s += " " + to_string(no->getPeso());
        vertices_arvore.push_back(s);
    }

    string regras = "1 " +
                    string(in_ponderado_aresta ? "1" : "0") + " " +
                    string(in_ponderado_vertice ? "1" : "0");

    return new Grafo(visitados.size(), regras, vertices_arvore, arestas_arvore);
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
