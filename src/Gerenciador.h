#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador
{
public:
    static void comandos(Grafo *grafo, string pasta);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo *grafo, int tam);
    static void imprimeListaAdj(Grafo *grafo);
    static void salvaListaAdj(Grafo *grafo, string caminho);
    static void imprimeVetorChar(vector<char> lista);
    static void salvaVetorChar(vector<char> lista, string caminho);
    static void imprimeFuncoesBasicas(int raio, int diametro, vector<char> centro, vector<char> periferia);
    static void salvaExcentricidade(int raio, int diametro, vector<char> centro, vector<char> periferia, string caminho);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
};

#endif // GERENCIADOR_H
