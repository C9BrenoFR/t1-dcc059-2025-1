#include "GRASP.h"
#include "colors.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <climits>

using namespace std;

vector<char> GRASP::conjunto_dominante_independente_grasp(Grafo *grafo, int max_iteracoes, int tamanho_lrc, bool busca_local)
{
    if (grafo->getListaAdj().empty())
    {
        cout << RED << "Grafo vazio!" << RESET << endl;
        return {};
    }

    cout << CYAN << "Iniciando algoritmo GRASP para Conjunto Dominante Independente..." << RESET << endl;
    cout << "Parametros: max_iteracoes=" << max_iteracoes << ", tamanho_lrc=" << tamanho_lrc << ", busca_local=" << (busca_local ? "sim" : "nao") << endl
         << endl;

    vector<char> melhor_solucao;
    int melhor_tamanho = INT_MAX;

    mt19937 gerador(chrono::steady_clock::now().time_since_epoch().count());

    for (int iteracao = 1; iteracao <= max_iteracoes; iteracao++)
    {
        cout << YELLOW << "Iteracao " << iteracao << "/" << max_iteracoes << ":" << RESET << endl;

        vector<char> solucao_atual = construir_solucao_gulosa_randomizada(grafo, tamanho_lrc, gerador);

        cout << "  Solucao construida com " << solucao_atual.size() << " vertices" << endl;

        if (busca_local && !solucao_atual.empty())
        {
            vector<char> solucao_melhorada = busca_local_melhoramento(grafo, solucao_atual);
            if (solucao_melhorada.size() < solucao_atual.size())
            {
                cout << "  Busca local melhorou: " << solucao_atual.size() << " -> " << solucao_melhorada.size() << " vertices" << endl;
                solucao_atual = solucao_melhorada;
            }
            else
            {
                cout << "  Busca local nao encontrou melhorias" << endl;
            }
        }

        if (!solucao_atual.empty() && (int)solucao_atual.size() < melhor_tamanho)
        {
            melhor_solucao = solucao_atual;
            melhor_tamanho = solucao_atual.size();
            cout << GREEN << "  Nova melhor solucao com " << melhor_tamanho << " vertices!" << RESET << endl;
        }
    }

    if (!melhor_solucao.empty())
    {
        sort(melhor_solucao.begin(), melhor_solucao.end());
        cout << GREEN << "Melhor Conjunto Dominante Independente (GRASP): ";
        for (char v : melhor_solucao)
        {
            cout << v << " ";
        }
        cout << "(" << melhor_solucao.size() << " vertices)" << RESET << endl;
    }
    else
    {
        cout << RED << "Nenhuma solucao valida encontrada!" << RESET << endl;
    }

    return melhor_solucao;
}

vector<char> GRASP::construir_solucao_gulosa_randomizada(Grafo *grafo, int tamanho_lrc, mt19937 &gerador)
{
    vector<char> conjunto_dominante;
    unordered_set<char> vertices_dominados;
    unordered_set<char> todos_os_vertices;

    for (No *no : grafo->getListaAdj())
    {
        todos_os_vertices.insert(no->getId());
    }

    while (vertices_dominados.size() < todos_os_vertices.size())
    {
        vector<Candidato> lrc = construir_lista_candidatos_restrita(grafo, vertices_dominados, conjunto_dominante, tamanho_lrc);

        if (lrc.empty())
        {
            cout << RED << "    Nenhum candidato valido encontrado. Construcao encerrada." << RESET << endl;
            break;
        }

        char candidato_selecionado = selecionar_candidato_aleatorio(lrc, gerador);

        cout << "    Candidato selecionado aleatoriamente: " << candidato_selecionado << " (de " << lrc.size() << " candidatos na LRC)" << endl;

        conjunto_dominante.push_back(candidato_selecionado);

        vertices_dominados.insert(candidato_selecionado);
        unordered_set<char> vizinhos = obter_vizinhos(grafo, candidato_selecionado);
        for (char vizinho : vizinhos)
        {
            vertices_dominados.insert(vizinho);
        }
    }

    return conjunto_dominante;
}

vector<GRASP::Candidato> GRASP::construir_lista_candidatos_restrita(Grafo *grafo,
                                                                    const unordered_set<char> &vertices_dominados,
                                                                    const vector<char> &conjunto_atual,
                                                                    int tamanho_lrc)
{
    vector<Candidato> todos_candidatos;

    for (No *candidato_no : grafo->getListaAdj())
    {
        char id_candidato = candidato_no->getId();

        if (vertices_dominados.count(id_candidato) > 0)
        {
            continue;
        }

        if (!eh_independente(grafo, id_candidato, conjunto_atual))
        {
            continue;
        }

        int pontuacao = contar_novos_dominados(grafo, id_candidato, vertices_dominados);
        todos_candidatos.emplace_back(id_candidato, pontuacao);
    }

    if (todos_candidatos.empty())
    {
        return {};
    }

    sort(todos_candidatos.begin(), todos_candidatos.end(),
         [](const Candidato &a, const Candidato &b)
         {
             if (a.pontuacao != b.pontuacao)
             {
                 return a.pontuacao > b.pontuacao;
             }
             return a.id < b.id;
         });

    vector<Candidato> lrc;
    int melhor_pontuacao = todos_candidatos[0].pontuacao;
    int pior_pontuacao = todos_candidatos.back().pontuacao;
    
    double alpha = 0.3; 
    int limite_pontuacao = melhor_pontuacao - (int)(alpha * (melhor_pontuacao - pior_pontuacao));

    for (const Candidato &candidato : todos_candidatos)
    {
        if (candidato.pontuacao >= limite_pontuacao && (int)lrc.size() < tamanho_lrc)
        {
            lrc.push_back(candidato);
        }
        else if ((int)lrc.size() >= tamanho_lrc)
        {
            break;
        }
    }

    return lrc;
}

char GRASP::selecionar_candidato_aleatorio(const vector<Candidato> &lrc, mt19937 &gerador)
{
    uniform_int_distribution<int> distribuicao(0, lrc.size() - 1);
    int indice_selecionado = distribuicao(gerador);
    return lrc[indice_selecionado].id;
}

vector<char> GRASP::busca_local_melhoramento(Grafo *grafo, vector<char> solucao_inicial)
{
    vector<char> melhor_solucao = solucao_inicial;
    bool melhorou = true;
    int iter = 0;
    int limite_iteracoes = 50; 
    int tentativas_sem_melhoria = 0;
    int max_tentativas_sem_melhoria = 10; 

    while (melhorou && iter < limite_iteracoes && tentativas_sem_melhoria < max_tentativas_sem_melhoria)
    {
        iter++;
        melhorou = false;
        vector<char> solucao_atual = melhor_solucao;

        for (int i = 0; i < (int)solucao_atual.size() && !melhorou; i++)
        {
            char vertice = solucao_atual[i];
            if (tentar_remover_vertice(grafo, solucao_atual, vertice))
            {
                if (solucao_atual.size() < melhor_solucao.size())
                {
                    melhor_solucao = solucao_atual;
                    melhorou = true;
                    tentativas_sem_melhoria = 0;
                    break;
                }
            }
            else
            {
                solucao_atual = melhor_solucao;
            }
        }

        if (!melhorou)
        {
            int tentativas_substituicao = 0;
            int max_substituicoes = min(20, (int)grafo->getListaAdj().size() * 2); 
            
            for (No *no : grafo->getListaAdj())
            {
                if (tentativas_substituicao >= max_substituicoes) break;
                
                char novo_vertice = no->getId();

                if (find(melhor_solucao.begin(), melhor_solucao.end(), novo_vertice) != melhor_solucao.end())
                    continue;

                for (char vertice_antigo : melhor_solucao)
                {
                    tentativas_substituicao++;
                    if (tentativas_substituicao > max_substituicoes) break;
                    
                    solucao_atual = melhor_solucao;
                    if (tentar_substituir_vertice(grafo, solucao_atual, vertice_antigo, novo_vertice))
                    {
                        if (solucao_atual.size() <= melhor_solucao.size() &&
                            eh_conjunto_dominante_valido(grafo, solucao_atual) &&
                            eh_conjunto_independente_valido(grafo, solucao_atual))
                        {
                            melhor_solucao = solucao_atual;
                            melhorou = true;
                            tentativas_sem_melhoria = 0;
                            break;
                        }
                    }
                }

                if (melhorou) break;
            }
        }
        
        if (!melhorou) {
            tentativas_sem_melhoria++;
        }
    }

    return melhor_solucao;
}


bool GRASP::tentar_remover_vertice(Grafo *grafo, vector<char> &conjunto, char vertice)
{
    auto it = find(conjunto.begin(), conjunto.end(), vertice);
    if (it == conjunto.end())
        return false;

    conjunto.erase(it);

    bool ainda_valido = eh_conjunto_dominante_valido(grafo, conjunto);

    if (!ainda_valido)
    {
        conjunto.push_back(vertice);
        return false;
    }

    return true;
}

bool GRASP::tentar_substituir_vertice(Grafo *grafo, vector<char> &conjunto, char antigo, char novo)
{
    vector<char> conjunto_sem_antigo = conjunto;
    auto it = find(conjunto_sem_antigo.begin(), conjunto_sem_antigo.end(), antigo);
    if (it != conjunto_sem_antigo.end())
    {
        conjunto_sem_antigo.erase(it);
    }

    if (!eh_independente(grafo, novo, conjunto_sem_antigo))
    {
        return false;
    }

    it = find(conjunto.begin(), conjunto.end(), antigo);
    if (it != conjunto.end())
    {
        *it = novo;
        return true;
    }

    return false;
}

bool GRASP::eh_conjunto_dominante_valido(Grafo *grafo, const vector<char> &conjunto)
{
    unordered_set<char> vertices_dominados = obter_vertices_dominados(grafo, conjunto);

    for (No *no : grafo->getListaAdj())
    {
        if (vertices_dominados.find(no->getId()) == vertices_dominados.end())
        {
            return false;
        }
    }

    return true;
}

bool GRASP::eh_conjunto_independente_valido(Grafo *grafo, const vector<char> &conjunto)
{
    for (int i = 0; i < (int)conjunto.size(); i++)
    {
        for (int j = i + 1; j < (int)conjunto.size(); j++)
        {
            if (eh_adjacente(grafo, conjunto[i], conjunto[j]))
            {
                return false;
            }
        }
    }
    return true;
}

unordered_set<char> GRASP::obter_vertices_dominados(Grafo *grafo, const vector<char> &conjunto)
{
    unordered_set<char> dominados;

    for (char vertice : conjunto)
    {
        dominados.insert(vertice);
        unordered_set<char> vizinhos = obter_vizinhos(grafo, vertice);
        for (char vizinho : vizinhos)
        {
            dominados.insert(vizinho);
        }
    }

    return dominados;
}

bool GRASP::eh_adjacente(Grafo *grafo, char vertice1, char vertice2)
{
    No *no1 = grafo->getNoPorId(vertice1);
    if (!no1)
        return false;

    for (Aresta *aresta : no1->getArestas())
    {
        if (aresta->getIdNoAlvo() == vertice2)
        {
            return true;
        }
    }

    if (!grafo->getInDirecionado())
    {
        No *no2 = grafo->getNoPorId(vertice2);
        if (!no2)
            return false;

        for (Aresta *aresta : no2->getArestas())
        {
            if (aresta->getIdNoAlvo() == vertice1)
            {
                return true;
            }
        }
    }

    return false;
}

bool GRASP::eh_independente(Grafo *grafo, char candidato, const vector<char> &conjunto_dominante)
{
    for (char vertice_dominante : conjunto_dominante)
    {
        if (eh_adjacente(grafo, candidato, vertice_dominante))
        {
            return false;
        }
    }
    return true;
}

int GRASP::contar_novos_dominados(Grafo *grafo, char candidato, const unordered_set<char> &vertices_dominados)
{
    int novos_dominados = 0;

    if (vertices_dominados.find(candidato) == vertices_dominados.end())
    {
        novos_dominados++;
    }

    unordered_set<char> vizinhos = obter_vizinhos(grafo, candidato);
    for (char vizinho : vizinhos)
    {
        if (vertices_dominados.find(vizinho) == vertices_dominados.end())
        {
            novos_dominados++;
        }
    }

    return novos_dominados;
}

unordered_set<char> GRASP::obter_vizinhos(Grafo *grafo, char vertice)
{
    unordered_set<char> vizinhos;
    No *no = grafo->getNoPorId(vertice);

    if (!no)
        return vizinhos;

    for (Aresta *aresta : no->getArestas())
    {
        vizinhos.insert(aresta->getIdNoAlvo());
    }

    if (!grafo->getInDirecionado())
    {
        for (No *outro_no : grafo->getListaAdj())
        {
            for (Aresta *aresta : outro_no->getArestas())
            {
                if (aresta->getIdNoAlvo() == vertice)
                {
                    vizinhos.insert(outro_no->getId());
                }
            }
        }
    }

    return vizinhos;
}
