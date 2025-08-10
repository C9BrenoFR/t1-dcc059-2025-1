#include "Reativo.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <climits>
#include <chrono>

const vector<double> Reativo::niveis_alpha = {0.1, 0.4, 0.7};

vector<char> Reativo::conjunto_dominante_independente_reativo(Grafo *grafo, int max_iteracoes, int tamanho_bloco, bool busca_local)
{
    auto inicio = chrono::high_resolution_clock::now();
    
    vector<char> melhor_solucao;
    int melhor_tamanho = INT_MAX;

    map<double, EstatisticasAlpha> estatisticas;
    for (double alpha : niveis_alpha)
    {
        estatisticas[alpha].probabilidade = 1.0 / niveis_alpha.size();
    }

    random_device rd;
    mt19937 gerador(rd());

    cout << "Executando algoritmo reativo com " << max_iteracoes << " iterações..." << endl;
    
    int iteracao = 0;
    while (iteracao < max_iteracoes)
    {
        int bloco_atual = tamanho_bloco + (iteracao % 21);
        
        for (int i = 0; i < bloco_atual && iteracao < max_iteracoes; i++, iteracao++)
        {
            double alpha_selecionado = selecionar_alpha_probabilistico(estatisticas, gerador);

            vector<char> solucao = construir_solucao_gulosa_randomizada(grafo, alpha_selecionado, gerador);

            if (busca_local && !solucao.empty())
            {
                solucao = busca_local_melhoramento(grafo, solucao);
            }

            if (!solucao.empty())
            {
                double qualidade = calcular_qualidade_solucao(solucao);
                
                estatisticas[alpha_selecionado].num_execucoes++;
                estatisticas[alpha_selecionado].soma_qualidade += qualidade;
                estatisticas[alpha_selecionado].media_qualidade = 
                    estatisticas[alpha_selecionado].soma_qualidade / estatisticas[alpha_selecionado].num_execucoes;

                if (solucao.size() < melhor_tamanho)
                {
                    melhor_solucao = solucao;
                    melhor_tamanho = solucao.size();
                }
            }
        }

        atualizar_probabilidades(estatisticas);
    }

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::microseconds>(fim - inicio);
    double tempo_segundos = duracao.count() / 1000000.0;

    cout << "\n=== RESULTADOS DO ALGORITMO REATIVO ===" << endl;
    cout << "Melhor solução encontrada: ";
    if (!melhor_solucao.empty()) {
        cout << "Tamanho = " << melhor_solucao.size() << ", Vértices = {";
        for (int i = 0; i < melhor_solucao.size(); i++) {
            cout << melhor_solucao[i];
            if (i < melhor_solucao.size() - 1) cout << ", ";
        }
        cout << "}" << endl;
    } else {
        cout << "Nenhuma solução encontrada" << endl;
    }
    cout << "Tempo de execução: " << tempo_segundos << " segundos" << endl;
    
    cout << "\nEstatísticas dos níveis de aleatoriedade:" << endl;
    for (const auto &par : estatisticas) {
        double alpha = par.first;
        const EstatisticasAlpha &stats = par.second;
        cout << "Alpha " << alpha << ": " << stats.num_execucoes << " execuções, "
             << "média qualidade = " << stats.media_qualidade << ", "
             << "probabilidade final = " << stats.probabilidade << endl;
    }
    cout << "========================================\n" << endl;

    return melhor_solucao;
}

vector<char> Reativo::construir_solucao_gulosa_randomizada(Grafo *grafo, double alpha, mt19937 &gerador)
{
    vector<char> conjunto_dominante;
    unordered_set<char> vertices_dominados;

    vector<char> todos_vertices;
    for (No *no : grafo->getListaAdj())
    {
        todos_vertices.push_back(no->getId());
    }

    while (vertices_dominados.size() < todos_vertices.size())
    {
        vector<Candidato> lrc = construir_lista_candidatos_restrita(grafo, vertices_dominados, conjunto_dominante, alpha);

        if (lrc.empty())
        {
            break;
        }

        char candidato_selecionado = selecionar_candidato_aleatorio(lrc, gerador);

        conjunto_dominante.push_back(candidato_selecionado);

        vertices_dominados.insert(candidato_selecionado);
        unordered_set<char> vizinhos = GrafoUtils::obter_vizinhos(grafo, candidato_selecionado);
        for (char vizinho : vizinhos)
        {
            vertices_dominados.insert(vizinho);
        }
    }

    return conjunto_dominante;
}

vector<Reativo::Candidato> Reativo::construir_lista_candidatos_restrita(Grafo *grafo,
                                                                        const unordered_set<char> &vertices_dominados,
                                                                        const vector<char> &conjunto_atual,
                                                                        double alpha)
{
    vector<Candidato> candidatos;

    for (No *no : grafo->getListaAdj())
    {
        char candidato = no->getId();

        if (GrafoUtils::eh_independente(grafo, candidato, conjunto_atual))
        {
            int pontuacao = GrafoUtils::contar_novos_dominados(grafo, candidato, vertices_dominados);
            if (pontuacao > 0)
            {
                candidatos.emplace_back(candidato, pontuacao);
            }
        }
    }

    if (candidatos.empty())
    {
        return candidatos;
    }

    sort(candidatos.begin(), candidatos.end(), [](const Candidato &a, const Candidato &b)
         { return a.pontuacao > b.pontuacao; });

    int melhor_pontuacao = candidatos[0].pontuacao;
    int pior_pontuacao = candidatos.back().pontuacao;
    int limite_pontuacao = melhor_pontuacao - alpha * (melhor_pontuacao - pior_pontuacao);

    vector<Candidato> lrc;
    for (const Candidato &candidato : candidatos)
    {
        if (candidato.pontuacao >= limite_pontuacao)
        {
            lrc.push_back(candidato);
        }
    }

    return lrc;
}

char Reativo::selecionar_candidato_aleatorio(const vector<Candidato> &lrc, mt19937 &gerador)
{
    uniform_int_distribution<> dist(0, lrc.size() - 1);
    return lrc[dist(gerador)].id;
}

double Reativo::selecionar_alpha_probabilistico(const map<double, EstatisticasAlpha> &estatisticas, mt19937 &gerador)
{
    uniform_real_distribution<> dist(0.0, 1.0);
    double valor_aleatorio = dist(gerador);
    
    double probabilidade_acumulada = 0.0;
    for (const auto &par : estatisticas)
    {
        probabilidade_acumulada += par.second.probabilidade;
        if (valor_aleatorio <= probabilidade_acumulada)
        {
            return par.first;
        }
    }
    
    return estatisticas.rbegin()->first;
}

void Reativo::atualizar_probabilidades(map<double, EstatisticasAlpha> &estatisticas)
{
    double melhor_media = 0.0;
    for (const auto &par : estatisticas)
    {
        if (par.second.num_execucoes > 0)
        {
            melhor_media = max(melhor_media, par.second.media_qualidade);
        }
    }

    map<double, double> pesos;
    double soma_pesos = 0.0;
    
    for (const auto &par : estatisticas)
    {
        double alpha = par.first;
        const EstatisticasAlpha &stats = par.second;
        
        if (stats.num_execucoes > 0 && melhor_media > 0)
        {
            double peso = stats.media_qualidade / melhor_media;
            pesos[alpha] = peso;
            soma_pesos += peso;
        }
        else
        {
            pesos[alpha] = 1.0;
            soma_pesos += 1.0;
        }
    }

    for (auto &par : estatisticas)
    {
        double alpha = par.first;
        par.second.probabilidade = pesos[alpha] / soma_pesos;
    }
}

double Reativo::calcular_qualidade_solucao(const vector<char> &solucao)
{
    return 1.0 / solucao.size();
}

vector<char> Reativo::busca_local_melhoramento(Grafo *grafo, vector<char> solucao_inicial)
{
    vector<char> solucao_atual = solucao_inicial;
    bool melhorou = true;

    while (melhorou)
    {
        melhorou = false;
        vector<char> melhor_vizinho = solucao_atual;

        for (int i = 0; i < solucao_atual.size(); i++)
        {
            vector<char> vizinho = solucao_atual;
            if (tentar_remover_vertice(grafo, vizinho, solucao_atual[i]))
            {
                if (vizinho.size() < melhor_vizinho.size())
                {
                    melhor_vizinho = vizinho;
                    melhorou = true;
                }
            }
        }

        for (int i = 0; i < solucao_atual.size(); i++)
        {
            for (No *no : grafo->getListaAdj())
            {
                char novo_vertice = no->getId();
                if (find(solucao_atual.begin(), solucao_atual.end(), novo_vertice) == solucao_atual.end())
                {
                    vector<char> vizinho = solucao_atual;
                    if (tentar_substituir_vertice(grafo, vizinho, solucao_atual[i], novo_vertice))
                    {
                        if (vizinho.size() < melhor_vizinho.size())
                        {
                            melhor_vizinho = vizinho;
                            melhorou = true;
                        }
                    }
                }
            }
        }

        if (melhorou)
        {
            solucao_atual = melhor_vizinho;
        }
    }

    return solucao_atual;
}

bool Reativo::tentar_remover_vertice(Grafo *grafo, vector<char> &conjunto, char vertice)
{
    auto it = find(conjunto.begin(), conjunto.end(), vertice);
    if (it == conjunto.end())
        return false;

    conjunto.erase(it);

    if (eh_conjunto_dominante_valido(grafo, conjunto) && eh_conjunto_independente_valido(grafo, conjunto))
    {
        return true;
    }

    conjunto.push_back(vertice);
    return false;
}

bool Reativo::tentar_substituir_vertice(Grafo *grafo, vector<char> &conjunto, char antigo, char novo)
{
    auto it = find(conjunto.begin(), conjunto.end(), antigo);
    if (it == conjunto.end())
        return false;

    *it = novo;

    if (eh_conjunto_dominante_valido(grafo, conjunto) && eh_conjunto_independente_valido(grafo, conjunto))
    {
        return true;
    }

    *it = antigo;
    return false;
}

bool Reativo::eh_conjunto_dominante_valido(Grafo *grafo, const vector<char> &conjunto)
{
    unordered_set<char> dominados = obter_vertices_dominados(grafo, conjunto);

    for (No *no : grafo->getListaAdj())
    {
        if (dominados.find(no->getId()) == dominados.end())
        {
            return false;
        }
    }

    return true;
}

bool Reativo::eh_conjunto_independente_valido(Grafo *grafo, const vector<char> &conjunto)
{
    for (int i = 0; i < conjunto.size(); i++)
    {
        for (int j = i + 1; j < conjunto.size(); j++)
        {
            if (GrafoUtils::eh_adjacente(grafo, conjunto[i], conjunto[j]))
            {
                return false;
            }
        }
    }

    return true;
}

unordered_set<char> Reativo::obter_vertices_dominados(Grafo *grafo, const vector<char> &conjunto)
{
    unordered_set<char> dominados;

    for (char vertice : conjunto)
    {
        dominados.insert(vertice);

        unordered_set<char> vizinhos = GrafoUtils::obter_vizinhos(grafo, vertice);
        for (char vizinho : vizinhos)
        {
            dominados.insert(vizinho);
        }
    }

    return dominados;
}
