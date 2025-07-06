#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <algorithm>

using namespace std;

Grafo::Grafo(int ordem, string regras, vector<string> lista_vertices, vector<string> lista_arestas)
{
    this->ordem = ordem;
    in_direcionado = (regras[0] == '1');
    in_ponderado_aresta = (regras[2] == '1');
    in_ponderado_vertice = (regras[4] == '1');

    for (string lista_vertice : lista_vertices)
    {
        int peso = 0;
        if (in_ponderado_vertice && lista_vertice.size() > 2)
            peso = stoi(lista_vertice.substr(2));
        No *vertice = new No(lista_vertice[0], peso);
        lista_adj.emplace_back(vertice);
    }

    for (string lista_aresta : lista_arestas)
    {
        for (No *vertice : lista_adj)
        {
            if (vertice->getId() == lista_aresta[0])
            {
                int peso = 0;
                if (in_ponderado_aresta && lista_aresta.size() > 4)
                    peso = stoi(lista_aresta.substr(4));
                vertice->setAresta(new Aresta(vertice->getId(), lista_aresta[2], peso));
            }
        }
    }
}

Grafo::Grafo(int ordem, bool in_direcionado, bool in_ponderado_aresta, bool in_ponderado_vertice, vector<No *> lista_adj)
{
    this->ordem = ordem;
    this->in_direcionado = in_direcionado;
    this->in_ponderado_aresta = in_ponderado_aresta;
    this->in_ponderado_vertice = in_ponderado_vertice;
    this->lista_adj = lista_adj;
}

vector<No *> Grafo::getListaAdj()
{
    return lista_adj;
}

int Grafo::getOrdem()
{
    return ordem;
}

bool Grafo::getInDirecionado()
{
    return in_direcionado;
}
bool Grafo::getInPonderadoAresta()
{
    return in_ponderado_aresta;
}
bool Grafo::getInPonderadoVertice()
{
    return in_ponderado_vertice;
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

    unordered_set<char> visitados;  // Conjunto de nós visitados
    stack<No *> pilha;
    pilha.push(inicio);

    while (!pilha.empty())
    {
        No *atual = pilha.top();
        pilha.pop();

        if (visitados.count(atual->getId()))
            continue;

        visitados.insert(atual->getId());

         // Percorre as arestas de saída
        for (Aresta *aresta : atual->getArestas())
        {
            No *proximo = getNoPorId(aresta->getIdNoAlvo());
            if (proximo && !visitados.count(proximo->getId()))
            {
                pilha.push(proximo);
            }
        }

        // Se não direcionado, também considerar conexões que "entram" no nó
        if (!this->getInDirecionado())
        {
            for (No *vizinho : this->getListaAdj())
            {
                for (Aresta *aresta : vizinho->getArestas())
                {
                    if (aresta->getIdNoAlvo() == atual->getId())
                    {
                        if (!visitados.count(vizinho->getId()))
                            pilha.push(vizinho);
                    }
                }
            }
        }
    }

    // Constrói resultado ignorando o nó inicial
    vector<char> resultado;
    for (char c : visitados)
    {
        if (c != id_char)
            resultado.push_back(c);
    }
    return resultado;
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no)
{
    char id_char = static_cast<char>(id_no);
    No *no_inicial = getNoPorId(id_char);
    if (!no_inicial)
        return {};

    unordered_set<char> visitados; // Conjunto de nós visitados
    stack<No *> pilha;
    pilha.push(no_inicial);

    while (!pilha.empty())
    {
        No *atual = pilha.top();
        pilha.pop();

        if (visitados.find(atual->getId()) != visitados.end())
            continue;

        visitados.insert(atual->getId());

        // Percorre todos os nós do grafo para verificar conexões que chegam em 'atual'
        for (No *no : lista_adj)
        {
            // Para cada aresta do nó
            for (Aresta *aresta : no->getArestas())
            {
                char origem = aresta->getIdNoOrigem();
                char destino = aresta->getIdNoAlvo();
                bool conexao_valida = false;
                No *vizinho = nullptr;

                if (in_direcionado)
                {
                    // Grafo direcionado: busca predecessores (arestas que chegam no nó atual)
                    if (destino == atual->getId())
                    {
                        conexao_valida = true;
                        vizinho = no; // 'no' é a origem da aresta que chega em 'atual'
                    }
                }
                else
                {
                    // Em grafos não direcionados, qualquer conexão é válida
                    if (origem == atual->getId())
                    {
                        conexao_valida = true;
                        vizinho = getNoPorId(destino);
                    }
                    else if (destino == atual->getId())
                    {
                        conexao_valida = true;
                        vizinho = getNoPorId(origem);
                    }
                }

                // Se a conexão é válida e ainda não visitada, adiciona à pilha
                if (conexao_valida && vizinho &&
                    visitados.find(vizinho->getId()) == visitados.end())
                {
                    pilha.push(vizinho);
                }
            }
        }
    }

    // Remove o nó inicial do resultado
    visitados.erase(id_char);
    return vector<char>(visitados.begin(), visitados.end());
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b)
{
    // Converte os ids dos nos de int para char
    char origem = static_cast<char>(id_no_a);
    char destino = static_cast<char>(id_no_b);

    map<char, int> distancias;    // Mapa para armazenar as menores distâncias conhecidas ate cada no
    map<char, char> antecessores; // Mapa para armazenar o antecessor de cada no no caminho minimo
    set<char> visitados;          // Armazena os nos visitados

    // Inicializa todos os nós com distancia infinita e sem antecessor
    for (No *no : lista_adj)
    {
        distancias[no->getId()] = __INT_MAX__;
        antecessores[no->getId()] = '\0';
    }

    distancias[origem] = 0; // Define a distancia da origem como 0 (pois o custo para chegar nela mesma é zero)

    while (visitados.size() < lista_adj.size())
    {                                      // Enquanto houver nos nao visitados
        char u = '\0';                     // Variável para armazenar o nó com a menor distância
        int menor_distancia = __INT_MAX__; // Inicializa a menor distância como infinito

        // Encontra o no com a menor distância que ainda não foi visitado
        for (auto &par : distancias)
        {
            if (visitados.find(par.first) == visitados.end() && par.second < menor_distancia)
            {
                menor_distancia = par.second;
                u = par.first;
            }
        }

        // Se não encontrou nenhum no alcançável - encerra o loop
        if (u == '\0')
        {
            break;
        }

        visitados.insert(u); // Marca o no atual como visitado

        // Obtem o no atual a partir do id
        // Se o no atual não existir, continua para o próximo loop
        No *no_atual = getNoPorId(u);
        if (!no_atual)
        {
            continue;
        }

        // Percorre as arestas do no atual
        for (Aresta *aresta : no_atual->getArestas())
        {
            char v = aresta->getIdNoAlvo();      // Obtem o id do no alvo da aresta
            int peso_aresta = aresta->getPeso(); // Obtem o peso da aresta

            if (distancias[u] != __INT_MAX__ && distancias[u] + peso_aresta < distancias[v])
            {
                distancias[v] = distancias[u] + peso_aresta; // Atualiza a menor distância para o no alvo
                antecessores[v] = u;                         // Atualiza o antecessor do no alvo
            }
        }
    }

    // Se a distância do destino for infinita, significa que não há caminho
    if (distancias[destino] == __INT_MAX__)
    {
        cout << "Nao ha caminho entre os nos " << origem << " e " << destino << endl;
        return {};
    }

    // Reconstrói o caminho a partir do destino até a origem usando os antecessores
    vector<char> caminho;
    char atual = destino;
    while (atual != '\0')
    {
        caminho.insert(caminho.begin(), atual); // Insere o no atual no início do caminho
        atual = antecessores[atual];            // Move para o antecessor do no atual
    }

    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b)
{
    const int INF = __INT_MAX__; // Valor para representar a ausencia de caminho - infinito
    int n = lista_adj.size();    // Número de nós no grafo

    // Cria dois mapas para converter entre:
    map<char, int> id_para_indice; // - char (id do vértice) → índice numérico (para usar na matriz)
    map<int, char> indice_para_id; // índice numérico → char (para converter de volta ao final)

    for (int i = 0; i < n; i++)
    {
        id_para_indice[lista_adj[i]->getId()] = i;
        indice_para_id[i] = lista_adj[i]->getId();
    }

    vector<vector<int>> dist(n, vector<int>(n, INF)); // Cria e inicializa a matriz de distâncias com infinito
    vector<vector<int>> prox(n, vector<int>(n, -1));  // Cria e inicializa a matriz de predecessores com -1

    // Inicializa as distâncias diretas entre os nos
    for (int i = 0; i < n; i++)
    {
        dist[i][i] = 0; // Distância de um nó para ele mesmo é 0
        prox[i][i] = i; // Predecessor de um nó para ele mesmo é ele mesmo

        for (Aresta *aresta : lista_adj[i]->getArestas())
        {
            int j = id_para_indice[aresta->getIdNoAlvo()]; // Indice do nó de destino
            dist[i][j] = aresta->getPeso();                // Peso da aresta direta
            prox[i][j] = j;                                // Próximo nó no caminho
        }
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
                {                                         // Só atualiza se ambos os caminhos  existirem
                    dist[i][j] = dist[i][k] + dist[k][j]; // Atualiza o menor custo
                    prox[i][j] = prox[i][k];              // Atualiza o próximo nó
                }
            }
        }
    }

    // Pega os indices dos nos de origem e destino
    int origem = id_para_indice[static_cast<char>(id_no)];
    int destino = id_para_indice[static_cast<char>(id_no_b)];

    if (dist[origem][destino] == INF)
    {
        cout << "Nao ha caminho entre os nos " << static_cast<char>(id_no) << " e " << static_cast<char>(id_no_b) << endl;
        return {};
    }

    // Reconstrução do caminho
    vector<char> caminho;
    int atual = origem;
    while (atual != destino)
    {
        caminho.push_back(indice_para_id[atual]);
        atual = prox[atual][destino];
    }
    caminho.push_back(indice_para_id[destino]);

    cout << "Distancia minima de " << static_cast<char>(id_no) << " para " << static_cast<char>(id_no_b) << " = " << dist[origem][destino] << endl;

    return caminho;
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    if (!in_ponderado_aresta)
        return nullptr;
    vector<No *> agm = {};
    for (No *no : this->lista_adj)
        if (no->getId() == ids_nos[0])
            agm.emplace_back(new No(no->getId(), no->getPeso()));
    algoritimo_prim(agm, ids_nos, ids_nos.size());
    Grafo *grafo = new Grafo(ordem, true, in_ponderado_aresta, false, agm);
    return grafo;
}

void Grafo::algoritimo_prim(vector<No *> &agm, vector<char> ids_nos, size_t max)
{
    if (agm.size() == max)
        return;
    vector<Aresta *> lista_arestas;
    for (No *no_agm : agm)
    {
        for (No *no_lista : lista_adj)
        {
            for (Aresta *aresta : no_lista->getArestas())
            {
                if (aresta->getIdNoAlvo() == no_agm->getId() || aresta->getIdNoOrigem() == no_agm->getId())
                {
                    bool origem_valida = false, alvo_valido = false;
                    for (char id_no : ids_nos)
                    {
                        if (id_no == aresta->getIdNoOrigem())
                            origem_valida = true;
                        if (id_no == aresta->getIdNoAlvo())
                            alvo_valido = true;
                    }

                    if (origem_valida && alvo_valido)
                        lista_arestas.emplace_back(aresta);
                }
            }
        }
    }
    ordenaListaAresta(lista_arestas, 0, lista_arestas.size() - 1);
    for (Aresta *aresta : lista_arestas)
    {
        bool origem_valida = true;
        bool alvo_valido = true;
        No *no_aux = nullptr;
        for (No *no : agm)
        {
            if (no->getId() == aresta->getIdNoOrigem())
            {
                origem_valida = false;
                no_aux = no;
            }
            if (no->getId() == aresta->getIdNoAlvo())
            {
                alvo_valido = false;
                no_aux = no;
            }
        }
        if (alvo_valido)
        {
            no_aux->setAresta(aresta);
            agm.emplace_back(new No(aresta->getIdNoAlvo(), 0));
            break;
        }
        else if (origem_valida)
        {
            No *novo_no = new No(aresta->getIdNoOrigem(), 0, {aresta});
            agm.emplace_back(novo_no);
            break;
        }
    }

    algoritimo_prim(agm, ids_nos, max);
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    if (!in_ponderado_aresta)
        return nullptr;
    vector<Aresta *> lista_arestas_ordenadas = getListaArestaOrdenada(ids_nos);

    vector<No *> agm;
    for (No *no_original : lista_adj)
    {
        No *no_copia = new No(no_original->getId(), no_original->getPeso());
        agm.push_back(no_copia);
    }

    vector<char> nos_encontrados;

    for (Aresta *aresta : lista_arestas_ordenadas)
    {
        bool encontrou_origem = false, encontrou_alvo = false;
        for (char no : nos_encontrados)
        {
            if (aresta->getIdNoOrigem() == no)
                encontrou_origem = true;
            else if (aresta->getIdNoAlvo() == no)
                encontrou_alvo = true;
        }
        if (!encontrou_origem || !encontrou_alvo)
        {
            for (No *no : agm)
            {
                if (no->getId() == aresta->getIdNoOrigem())
                {
                    no->setAresta(aresta);
                    break;
                }
            }
            if (!encontrou_origem)
                nos_encontrados.emplace_back(aresta->getIdNoOrigem());
            if (!encontrou_alvo)
                nos_encontrados.emplace_back(aresta->getIdNoAlvo());
        }
    }

    Grafo *grafo = new Grafo(ordem, true, in_ponderado_aresta, false, agm);
    return grafo;
}

vector<Aresta *> Grafo::getListaArestaOrdenada(vector<char> ids_nos)
{
    vector<Aresta *> lista_arestas = {};

    for (No *vertice : lista_adj)
    {
        for (Aresta *aresta : vertice->getArestas())
        {
            bool origem_valida = false;
            bool alvo_valido = false;

            for (char id : ids_nos)
            {
                if (aresta->getIdNoOrigem() == id)
                    origem_valida = true;
                if (aresta->getIdNoAlvo() == id)
                    alvo_valido = true;
            }

            if (origem_valida && alvo_valido)
            {
                lista_arestas.emplace_back(aresta);
            }
        }
    }

    if (lista_arestas.size() > 1)
        ordenaListaAresta(lista_arestas, 0, lista_arestas.size() - 1);

    return lista_arestas;
}

void Grafo::ordenaListaAresta(vector<Aresta *> &lista, int min, int max)
{
    if (min >= max)
        return;

    int meio = (min + max) / 2;

    ordenaListaAresta(lista, min, meio);
    ordenaListaAresta(lista, meio + 1, max);

    vector<Aresta *> temp;
    int i = min;
    int j = meio + 1;

    while (i <= meio && j <= max)
    {
        if (lista[i]->getPeso() <= lista[j]->getPeso())
        {
            temp.push_back(lista[i]);
            i++;
        }
        else
        {
            temp.push_back(lista[j]);
            j++;
        }
    }

    while (i <= meio)
    {
        temp.push_back(lista[i]);
        i++;
    }

    while (j <= max)
    {
        temp.push_back(lista[j]);
        j++;
    }

    for (int k = 0; k < temp.size(); ++k)
    {
        lista[min + k] = temp[k];
    }
}

Grafo *Grafo::arvore_caminhamento_profundidade(int id_no)
{
    char id_char = static_cast<char>(id_no);
    No *inicio = getNoPorId(id_char);
    if (!inicio)
        return nullptr;

    unordered_set<char> marcado;   // Guarda nós já visitados
    vector<char> visitados;        // Ordem dos nós visitados
    vector<string> arestas_arvore; // Armazena as arestas da árvore de caminhamento
    stack<pair<No *, char>> pilha; // Pilha para DFS, com par (nó atual, pai)
    pilha.push({inicio, '\0'});    // '\0' como pai do nó inicial

    while (!pilha.empty())
    {
        auto [atual, pai] = pilha.top(); // Obtém o topo da pilha
        pilha.pop();

        char id_atual = atual->getId();
        if (marcado.count(id_atual)) // Se já foi visitado, ignora
            continue;

        marcado.insert(id_atual);
        visitados.push_back(id_atual); // Marca como visitado

        if (pai != '\0') // Se não é o nó inicial, registra a aresta (pai -> atual)
        {
            arestas_arvore.emplace_back(string(1, pai) + " " + string(1, id_atual));
        }

        // Coleta os vizinhos ainda não visitados
        vector<No *> vizinhos;
        unordered_set<char> adicionados; // Evita adicionar o mesmo nó duas vezes

        // Sempre considera arestas de saída
        for (Aresta *a : atual->getArestas())
        {
            char id_alvo = a->getIdNoAlvo();
            if (!marcado.count(id_alvo) && !adicionados.count(id_alvo))
            {
                No *proximo = getNoPorId(id_alvo);
                if (proximo)
                {
                    vizinhos.push_back(proximo);
                    adicionados.insert(id_alvo);
                }
            }
        }

        // Se for não direcionado, também considera arestas de entrada
        if (!in_direcionado)
        {
            for (No *no : lista_adj)
            {
                for (Aresta *a : no->getArestas())
                {
                    if (a->getIdNoAlvo() == id_atual)
                    {
                        char id_origem = no->getId();
                        if (!marcado.count(id_origem) && !adicionados.count(id_origem))
                        {
                            vizinhos.push_back(no);
                            adicionados.insert(id_origem);
                        }
                    }
                }
            }
        }

        // Adiciona os vizinhos à pilha em ordem reversa (opcional: para manter ordem de visita)
        for (size_t i = vizinhos.size(); i-- > 0;)
        {
            pilha.push({vizinhos[i], id_atual});
        }
    }

    // Monta os vértices com ou sem pesos
    vector<string> vertices_arvore;
    for (char id : visitados)
    {
        No *no = getNoPorId(id);
        string s(1, id);
        if (in_ponderado_vertice)
            s += " " + to_string(no->getPeso());
        vertices_arvore.push_back(s);
    }

    // Regras do grafo (direcionado, arestas ponderadas, vértices ponderados)
    string regras = (in_direcionado ? "1" : "0") + string(" ") +
                    (in_ponderado_aresta ? "1" : "0") + string(" ") +
                    (in_ponderado_vertice ? "1" : "0");

    // Retorna o novo grafo representando a árvore de caminhamento
    return new Grafo(visitados.size(), regras, vertices_arvore, arestas_arvore);
}

map<char, int> Grafo::calcular_excentricidades()
{
    const int INF = __INT_MAX__;
    int n = lista_adj.size();
    map<char, int> excentricidades;
    
    if (n == 0) 
        return excentricidades;
    
    vector<char> indice_para_id(n);
    map<char, int> id_para_indice;
    
    for (int i = 0; i < n; i++) {
        char id = lista_adj[i]->getId();
        indice_para_id[i] = id;
        id_para_indice[id] = i;
        excentricidades[id] = -1; // Inicializa como indefinido
    }
    
    vector<vector<int>> dist(n, vector<int>(n, INF));
    
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0; // Distância de um nó para ele mesmo é 0
        
        for (Aresta *aresta : lista_adj[i]->getArestas()) {
            int j = id_para_indice[aresta->getIdNoAlvo()];
            dist[i][j] = aresta->getPeso();
            
            if (!in_direcionado) {
                dist[j][i] = dist[i][j];
            }
        }
    }
    
    //Floyd-Warshall com early termination
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i][k] == INF) continue; // Pula se não há caminho
            
            for (int j = 0; j < n; j++) {
                if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    
    bool grafo_conectado = true;
    for (int i = 0; i < n && grafo_conectado; i++) {
        int excentricidade_atual = 0;
        
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                grafo_conectado = false;
                break;
            }
            
            // Calcula excentricidade durante a verificação
            if (i != j && dist[i][j] > excentricidade_atual) {
                excentricidade_atual = dist[i][j];
            }
        }
        
        if (grafo_conectado) {
            excentricidades[indice_para_id[i]] = excentricidade_atual;
        }
    }
    
    if (!grafo_conectado) {
        cout << "Grafo nao e conectado - metricas indefinidas" << endl;
        // Limpa excentricidades e retorna com valores -1
        for (auto &par : excentricidades) {
            par.second = -1;
        }
    }
    
    return excentricidades;
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
