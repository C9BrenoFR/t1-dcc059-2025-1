#include <iostream>
#include <string>
#include <vector>
#include "Gerenciador.h"

using namespace std;
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
    string aresta_ab = "a b 5";
    string aresta_be = "b e 4";
    string aresta_ec = "e c 3";
    string aresta_de = "d e 2";
    string aresta_ae = "a e 1";

    vector<string> vertices = vector<string>{no_a, no_b, no_c, no_d, no_e};
    vector<string> arestas = vector<string>{aresta_ab, aresta_ae, aresta_be, aresta_de, aresta_ec};

    Grafo *grafo = new Grafo(5, regras, vertices, arestas);
    grafo->arvore_geradora_minima_kruskal({});
    Gerenciador::comandos(grafo);

    return 0;
}
