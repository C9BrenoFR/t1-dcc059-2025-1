#include <iostream>
#include <string>
#include <vector>
#include "Gerenciador.h"

using namespace std;

void imprimeListaAdj(Grafo *grafo)
{
    cout << "+-+-----------\n";
    for (No *no : grafo->getListaAdj())
    {
        cout << "|" << no->getId() << "|";
        for (Aresta *aresta : no->getArestas())
            cout << aresta->getIdNoAlvo() << " ";
        cout << endl;
    }

    cout << "+-+-----------\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Uso: ./main <nome_do_arquivo>\n";
        return 1;
    }
    std::string nome_arquivo = argv[1];
    nome_arquivo = "../instancias/" + nome_arquivo;
    std::cout << "Arquivo passado: " << nome_arquivo << std::endl;

    string regras = "1 1 1";
    string no_a = "a 1";
    string no_b = "b 2";
    string no_c = "c 3";
    string no_d = "d 4";
    string no_e = "e 5";
    string aresta_1 = "a b 2";
    string aresta_2 = "a c 3";
    string aresta_3 = "b c 1";
    string aresta_4 = "b d 4";
    string aresta_5 = "c e 5";
    string aresta_6 = "d e 6";

    vector<string> vertices = vector<string>{no_a, no_b, no_c, no_d, no_e};
    vector<string> arestas = vector<string>{aresta_1, aresta_2, aresta_3, aresta_4, aresta_5, aresta_6};

    Grafo *grafo = new Grafo(5, regras, vertices, arestas);
    Grafo *agmg1 = grafo->arvore_geradora_minima_kruskal({});
    imprimeListaAdj(agmg1);
    // Gerenciador::comandos(grafo);

    return 0;
}
