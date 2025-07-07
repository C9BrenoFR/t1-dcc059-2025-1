#include "Gerenciador.h"
#include <fstream>
#include <map>

void Gerenciador::comandos(Grafo *grafo)
{
    cout << "Digite uma das opcoes abaixo e pressione enter:" << endl
         << endl;
    cout << "(a) Fecho transitivo direto de um no;" << endl;
    cout << "(b) Fecho transitivo indireto de um no;" << endl;
    cout << "(c) Caminho minimo (Djikstra);" << endl;
    cout << "(d) Caminho minimo (Floyd);" << endl;
    cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << endl;
    cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << endl;
    cout << "(g) Arvore de caminhamento em profundidade;" << endl;
    cout << "(h) Raio, diametro, centro e periferia do grafo;" << endl;
    cout << "(0) Sair;" << endl
         << endl;

    char resp;
    cin >> resp;
    switch (resp)
    {
    case 'a':
    {

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);

        cout << "Fecho transitivo direto do nó " << id_no << ": ";
        if (fecho_transitivo_direto.empty())
        {
            cout << "Nenhum nó alcançável." << endl;
        }
        else
        {
            for (char c : fecho_transitivo_direto)
                cout << c << " ";
            cout << endl;
        }
        cout << endl;

        if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
        {
            ofstream arquivo("fecho_trans_dir.txt");
            for (char c : fecho_transitivo_direto)
                arquivo << c << " ";
            arquivo.close();
            cout << "Resultado salvo em fecho_trans_dir.txt" << endl
                 << endl;
        }

        break;
    }

    case 'b':
    {

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);

        cout << "Fecho transitivo indireto do nó " << id_no << ": ";
        if (fecho_transitivo_indireto.empty())
        {
            cout << "Nenhum nó alcançável." << endl;
        }
        else
        {
            for (char c : fecho_transitivo_indireto)
                cout << c << " ";
            cout << endl;
        }
        cout << endl;

        if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
        {
            ofstream arquivo("fecho_trans_indir.txt");
            for (char c : fecho_transitivo_indireto)
                arquivo << c << " ";
            arquivo.close();
            cout << "Resultado salvo em fecho_trans_indir.txt" << endl
                 << endl;
        }

        break;
    }

    case 'c':
    {

        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

        if (caminho_minimo_dijkstra.empty())
        {
            cout << "Nenhum caminho encontrado entre os nós " << id_no_1 << " e " << id_no_2 << "." << endl;
        }
        else
        {
            cout << "Caminho mínimo de " << id_no_1 << " a " << id_no_2 << ": ";
            for (char c : caminho_minimo_dijkstra)
            {
                cout << c;
                if (c != id_no_2)
                    cout << ",";
            }
            cout << endl
                 << endl;
        }

        if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
        {
            ofstream arquivo("caminho_minimo_dijkstra.txt");
            if (arquivo.is_open())
            {
                for (char c : caminho_minimo_dijkstra)
                {
                    arquivo << c;
                    if (c != id_no_2)
                        arquivo << ",";
                }
                arquivo.close();
                cout << "Resultado salvo em caminho_minimo_dijkstra.txt" << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir o arquivo!" << endl
                     << endl;
            }
        }

        break;
    }

    case 'd':
    {

        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);
        if (caminho_minimo_floyd.empty())
        {
            cout << "Nenhum caminho encontrado entre os nós " << id_no_1 << "e " << id_no_2 << "." << endl;
        }
        else
        {
            cout << "Caminho mínimo de " << id_no_1 << " a " << id_no_2 << ": ";
            for (char c : caminho_minimo_floyd)
            {
                cout << c;
                if (c != id_no_2)
                    cout << ",";
            }
            cout << endl
                 << endl;
        }

        if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
        {
            ofstream arquivo("caminho_minimo_floyd.txt");
            if (arquivo.is_open())
            {
                for (char c : caminho_minimo_floyd)
                {
                    arquivo << c;
                    if (c != id_no_2)
                        arquivo << ",";
                }
                arquivo.close();
                cout << "Resultado salvo em caminho_minimo_floyd.txt" << endl
                     << endl;
            }
            else
                cout << "Erro ao abrir o arquivo!" << endl
                     << endl;
        }

        break;
    }
    case 'e':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->getOrdem())
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);

            if (arvore_geradora_minima_prim == nullptr)
            {
                cout << "Não é possivel calcular AGM de um grafo que não possui arestas ponderadas" << endl;
                break;
            }
            imprimeListaAdj(arvore_geradora_minima_prim);
            if (pergunta_imprimir_arquivo("agm_prim.txt"))
            {
                cout << "Metodo de impressao em arquivo nao implementado" << endl;
            }

            delete arvore_geradora_minima_prim;
        }
        else
        {
            cout << "Valor invalido" << endl;
        }

        break;
    }

    case 'f':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->getOrdem())
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);

            if (arvore_geradora_minima_kruskal == nullptr)
            {
                cout << "Não é possivel calcular AGM de um grafo que não possui arestas ponderadas" << endl;
                break;
            }

            imprimeListaAdj(arvore_geradora_minima_kruskal);

            if (pergunta_imprimir_arquivo("agm_kruskal.txt"))
            {
                cout << "Metodo de impressao em arquivo nao implementado" << endl;
            }

            delete arvore_geradora_minima_kruskal;
        }
        else
        {
            cout << "Valor invalido" << endl;
        }

        break;
    }

    case 'g':
    {
        char id_no = get_id_entrada();
        Grafo *arvore = grafo->arvore_caminhamento_profundidade(static_cast<int>(id_no));

        if (arvore)
        {
            cout << "Arvore de caminhamento em profundidade a partir de " << id_no << ":" << endl;
            for (No *no : arvore->getListaAdj())
            {
                cout << no->getId() << " -> ";
                for (Aresta *aresta : no->getArestas())
                {
                    cout << aresta->getIdNoAlvo() << " ";
                }
                cout << endl;
            }
            cout << endl;

            if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
            {
                ofstream arquivo("arvore_caminhamento_profundidade.txt");
                if (arquivo.is_open())
                {
                    for (No *no : arvore->getListaAdj())
                    {
                        arquivo << no->getId() << " -> ";
                        for (Aresta *aresta : no->getArestas())
                        {
                            arquivo << aresta->getIdNoAlvo() << " ";
                        }
                        arquivo << endl;
                    }
                    arquivo.close();
                    cout << "Arvore salva em arvore_caminhamento_profundidade.txt" << endl;
                }
                else
                {
                    cout << "Erro ao abrir o arquivo!" << endl;
                }
            }
        }
        else
        {
            cout << "Arvore nao pode ser gerada para o no " << id_no << endl;
        }

        delete arvore;
        break;
    }

    case 'h':
    {
        // map<char, int> excentricidades = grafo->calcular_excentricidades();

        // for (auto &par : excentricidades) {
        //     cout << "Vertice " << par.first << ": " << par.second << endl;
        // }

        if(grafo->raio() == -1){
            cout << "Erro ao calcular o raio: grafo nao conectado.";
            break;
        }

        cout << grafo->raio() << endl;

        if(grafo->diametro() == -1){
            cout << "Erro ao calcular o diametro: grafo nao conectado.";
            break;
        }

        cout << grafo->diametro() << endl;

        

        if (pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        break;
    }

    case 'i':
    {

        vector<char> articulacao = grafo->vertices_de_articulacao();
        cout << "Metodo de impressao em tela nao implementado" << endl
             << endl;

        if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        break;
    }

    case '0':
    {
        exit(0);
    }
    default:
        {
            cout << "Opção inválida" << endl;
        }
    }

    comandos(grafo);
}

char Gerenciador::get_id_entrada()
{
    cout << "Digite o id de um no: ";
    char id;
    cin >> id;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam)
{
    vector<char> ids = {};
    while ((int)ids.size() < tam)
    {
        char id_no = get_id_entrada();
        bool existe = false;
        for (No *no : grafo->getListaAdj())
        {
            if (no->getId() == id_no)
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            cout << "Vertice nao existe" << endl;
        }
        else
        {
            bool repetido = find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido)
            {
                cout << "Valor repetido" << endl;
            }
            else
            {
                ids.push_back(id_no);
            }
        }
    }

    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo)
{

    cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "(1) Sim;" << endl;
    cout << "(2) Nao." << endl;
    int resp;
    cin >> resp;
    cout << endl;

    switch (resp)
    {
    case 1:
        return true;
    case 2:
        return false;
    default:
        cout << "Resposta invalida" << endl;
        return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

void Gerenciador::imprimeListaAdj(Grafo *grafo)
{
    Grafo *grafo_impressao = grafo;
    if (!grafo->getInDirecionado())
    {
        vector<No *> lista_adj;

        // Reescrita da lista de adjacencia, para não alterar os nós originais
        for (No *no : grafo->getListaAdj())
            lista_adj.emplace_back(new No(no->getId(), no->getPeso(), no->getArestas()));

        // Prepara novo objeto para impressão, possibilitando manipulação sem alteração do objeto original
        grafo_impressao = new Grafo(grafo->getOrdem(), grafo->getInDirecionado(), grafo->getInPonderadoAresta(), grafo->getInPonderadoVertice(), lista_adj);

        // Colando as arestas também nos alvos, em caso de grafo não direcionado
        for (No *no : grafo->getListaAdj())
        {
            for (Aresta *aresta : no->getArestas())
            {
                No *no_aux = grafo_impressao->getNoPorId(aresta->getIdNoAlvo());
                no_aux->setAresta(new Aresta(aresta->getIdNoAlvo(), aresta->getIdNoOrigem(), aresta->getPeso()));
            }
        }
    }

    cout << "---------------------------------------------------------\n";
    for (No *no : grafo_impressao->getListaAdj())
    {
        cout << "|" << no->getId();
        if (grafo_impressao->getInPonderadoVertice())
            cout << "(" << no->getPeso() << ")";
        cout << "|";
        for (Aresta *aresta : no->getArestas())
        {
            cout << aresta->getIdNoAlvo();
            if (grafo_impressao->getInPonderadoAresta())
                cout << "(" << aresta->getPeso() << ")";
            cout << " ";
        }
        cout << endl;
    }

    cout << "---------------------------------------------------------\n";
}