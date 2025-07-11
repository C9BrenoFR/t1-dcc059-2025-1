#include "Gerenciador.h"
#include <fstream>
#include <map>

void Gerenciador::comandos(Grafo *grafo, string pasta)
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
            imprimeVetorChar(fecho_transitivo_direto);
            cout << endl;

            if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
            {
                salvaVetorChar(fecho_transitivo_direto, pasta + "/fecho_trans_dir.txt");
            }
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
            imprimeVetorChar(fecho_transitivo_indireto);
            cout << endl;

            if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
            {
                salvaVetorChar(fecho_transitivo_indireto, pasta + "/fecho_trans_indir.txt");
            }
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
            imprimeVetorChar(caminho_minimo_dijkstra);

            if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
            {
                salvaVetorChar(caminho_minimo_dijkstra, pasta + "/caminho_minimo_dijkstra.txt");
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
            imprimeVetorChar(caminho_minimo_floyd);

            if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
            {
                salvaVetorChar(caminho_minimo_floyd, pasta + "/caminho_minimo_floyd.txt");
            }
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
                salvaListaAdj(arvore_geradora_minima_prim, pasta + "/agm_prim.txt");
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
                salvaListaAdj(arvore_geradora_minima_kruskal, pasta + "/agm_kruskal.txt");
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
            imprimeListaAdj(arvore);

            if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
            {
                salvaListaAdj(grafo, pasta + "/arvore_caminhamento_profundidade.txt");
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

        int raio = grafo->raio();
        int diametro = grafo->diametro();
        vector<char> centro = grafo->centro();
        vector<char> periferia = grafo->periferia();

        if (raio == -1)
        {
            cout << "Erro ao calcular o raio: grafo nao conectado.";
            break;
        }

        if (diametro == -1)
        {
            cout << "Erro ao calcular o diametro: grafo nao conectado.";
            break;
        }

        if (centro.empty())
        {
            cout << "Erro ao exibir os vertices de centro: grafo desconectado";
            break;
        }

        if (centro.empty())
        {
            cout << "Erro ao exibir os vertices de centro: grafo desconectado";
            break;
        }

        if (periferia.empty())
        {
            cout << "Erro ao exibir os vertices de periferia: grafo desconectado";
            break;
        }

        imprimeFuncoesBasicas(raio, diametro, centro, periferia);

        if (pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt"))
        {
            salvaFuncoesBasicas(raio, diametro, centro, periferia, pasta + "/raio_diametro_centro_periferia.txt");
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

    comandos(grafo, pasta);
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

    for (No *no : grafo_impressao->getListaAdj())
    {
        cout << no->getId() << ": ";
        vector<Aresta *> arestas = no->getArestas();
        for (int i = 0; i < arestas.size(); i++)
        {
            cout << arestas[i]->getIdNoAlvo();
            if (i != arestas.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    }
}

void Gerenciador::salvaListaAdj(Grafo *grafo, string caminho)
{
    ofstream arquivo(caminho);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo " << caminho << " para escrita!" << endl;
        return;
    }

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

    for (No *no : grafo_impressao->getListaAdj())
    {
        arquivo << no->getId() << ": ";
        vector<Aresta *> arestas = no->getArestas();
        for (int i = 0; i < arestas.size(); i++)
        {
            arquivo << arestas[i]->getIdNoAlvo();
            if (i != arestas.size() - 1)
                arquivo << " -> ";
        }
        arquivo << endl;
    }

    arquivo.close();
    cout << "Lista de adjacência salva em " << caminho << endl;
}

void Gerenciador::imprimeVetorChar(vector<char> lista)
{
    cout << lista[0];
    for (int i = 1; i < lista.size(); i++)
    {
        cout << "," << lista[i];
    }
    cout << endl;
}

void Gerenciador::salvaVetorChar(vector<char> lista, string caminho)
{
    ofstream arquivo(caminho, ios::app);
    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo " << caminho << " para escrita!" << endl;
        return;
    }

    arquivo << lista[0];
    for (int i = 1; i < lista.size(); i++)
    {
        arquivo << "," << lista[i];
    }
    arquivo << endl;
}

void Gerenciador::imprimeFuncoesBasicas(int raio, int diametro, vector<char> centro, vector<char> periferia)
{
    cout << raio << endl
         << diametro << endl;
    imprimeVetorChar(centro);
    imprimeVetorChar(periferia);
}

void Gerenciador::salvaFuncoesBasicas(int raio, int diametro, vector<char> centro, vector<char> periferia, string caminho)
{
    ofstream arquivo(caminho);
    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo " << caminho << " para escrita!" << endl;
        return;
    }
    arquivo << raio << endl
            << diametro << endl;
    arquivo.close();
    salvaVetorChar(centro, caminho);
    salvaVetorChar(periferia, caminho);
}