#include "Gerenciador.h"
#include <fstream>

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
        cout << "Metodo de impressao em tela nao implementado" << endl
             << endl;

        if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        break;
    }

    case 'd':
    {

        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);
        cout << "Metodo de impressao em tela nao implementado" << endl
             << endl;

        if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
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
            cout << "Metodo de impressao em tela nao implementado" << endl
                 << endl;

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
            // Imprimir na tela
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

            // Salvar em arquivo se solicitado
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
        vector<char> articulacao = grafo->vertices_de_articulacao();
        cout << "Metodo de impressao em tela nao implementado" << endl
             << endl;

        if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
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
    cout << endl;
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