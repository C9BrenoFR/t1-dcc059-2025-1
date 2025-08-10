#ifndef GRASP_H
#define GRASP_H

#include "Grafo.h"
#include "GrafoUtils.h"
#include <vector>
#include <unordered_set>
#include <random>

using namespace std;

class GRASP
{
public:
    static vector<char> conjunto_dominante_independente_grasp(Grafo *grafo, int max_iteracoes = 100, int tamanho_lrc = 3, double alpha = 0.3, bool busca_local = true);

private:
    struct Candidato
    {
        char id;
        int pontuacao;

        Candidato(char _id, int _pontuacao) : id(_id), pontuacao(_pontuacao) {}
    };

    static vector<char> construir_solucao_gulosa_randomizada(Grafo *grafo, int tamanho_lrc, double alpha, mt19937 &gerador);
    static vector<char> busca_local_melhoramento(Grafo *grafo, vector<char> solucao_inicial);

    static vector<Candidato> construir_lista_candidatos_restrita(Grafo *grafo,
                                                                 const unordered_set<char> &vertices_dominados,
                                                                 const vector<char> &conjunto_atual,
                                                                 int tamanho_lrc,
                                                                 double alpha);
    static char selecionar_candidato_aleatorio(const vector<Candidato> &lrc, mt19937 &gerador);

    static bool tentar_remover_vertice(Grafo *grafo, vector<char> &conjunto, char vertice);
    static bool tentar_substituir_vertice(Grafo *grafo, vector<char> &conjunto, char antigo, char novo);
    static bool eh_conjunto_dominante_valido(Grafo *grafo, const vector<char> &conjunto);
    static bool eh_conjunto_independente_valido(Grafo *grafo, const vector<char> &conjunto);
    static unordered_set<char> obter_vertices_dominados(Grafo *grafo, const vector<char> &conjunto);
};

#endif // GRASP_H
