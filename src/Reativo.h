#ifndef REATIVO_H
#define REATIVO_H

#include "Grafo.h"
#include "GrafoUtils.h"
#include <vector>
#include <unordered_set>
#include <random>
#include <map>

using namespace std;

class Reativo
{
public:
    static vector<char> conjunto_dominante_independente_reativo(Grafo *grafo, int max_iteracoes = 300, int tamanho_bloco = 30, bool busca_local = true);

private:
    struct Candidato
    {
        char id;
        int pontuacao;

        Candidato(char _id, int _pontuacao) : id(_id), pontuacao(_pontuacao) {}
    };

    struct EstatisticasAlpha
    {
        int num_execucoes;
        double soma_qualidade;
        double media_qualidade;
        double probabilidade;

        EstatisticasAlpha() : num_execucoes(0), soma_qualidade(0.0), media_qualidade(0.0), probabilidade(0.0) {}
    };

    static const vector<double> niveis_alpha;
    
    static vector<char> construir_solucao_gulosa_randomizada(Grafo *grafo, double alpha, mt19937 &gerador);
    static vector<char> busca_local_melhoramento(Grafo *grafo, vector<char> solucao_inicial);

    static vector<Candidato> construir_lista_candidatos_restrita(Grafo *grafo,
                                                                 const unordered_set<char> &vertices_dominados,
                                                                 const vector<char> &conjunto_atual,
                                                                 double alpha);
    static char selecionar_candidato_aleatorio(const vector<Candidato> &lrc, mt19937 &gerador);

    static double selecionar_alpha_probabilistico(const map<double, EstatisticasAlpha> &estatisticas, mt19937 &gerador);
    static void atualizar_probabilidades(map<double, EstatisticasAlpha> &estatisticas);
    static double calcular_qualidade_solucao(const vector<char> &solucao);

    static bool tentar_remover_vertice(Grafo *grafo, vector<char> &conjunto, char vertice);
    static bool tentar_substituir_vertice(Grafo *grafo, vector<char> &conjunto, char antigo, char novo);
    static bool eh_conjunto_dominante_valido(Grafo *grafo, const vector<char> &conjunto);
    static bool eh_conjunto_independente_valido(Grafo *grafo, const vector<char> &conjunto);
    static unordered_set<char> obter_vertices_dominados(Grafo *grafo, const vector<char> &conjunto);
};

#endif
