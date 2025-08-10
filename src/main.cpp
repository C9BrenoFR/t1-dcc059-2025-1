#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "colors.h"
#include "Gerenciador.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << RED << "Uso: ./main <nome_do_arquivo>\n"
             << RESET;
        return 1;
    }
    string linha, regras, nome_arquivo = argv[1];
    vector<string> vertices, arestas;
    int ordem, linha_atual = 0;

    nome_arquivo = "../instancias_t2/" + nome_arquivo;
    ifstream arquivo(nome_arquivo);

    if (arquivo.is_open())
    {
        while (getline(arquivo, linha))
        {
            linha_atual++;
            if (linha_atual == 1)
                regras = linha;
            else if (linha_atual == 2)
                ordem = stoi(linha);
            else if (linha_atual > 2 && linha_atual <= 2 + ordem)
                vertices.emplace_back(linha);
            else
                arestas.emplace_back(linha);
        }
        arquivo.close();
    }
    else
    {
        cout << WARNING << "Erro ao abrir " << nome_arquivo << " para leitura\n"
             << RESET;
        cout << RED << "Digite apenas o nome do arquivo contido na pasta instancias_t2 ex:\n"
             << RESET
             << YELLOW << "./execGrupoX grafo01.txt (Caso esteja compilando via g++)\n"
             << "./compile.sh grafo01.txt (Caso esteja compilando com o script)\n"
             << RESET;
        return 1;
    }

    Grafo *grafo = new Grafo(ordem, regras, vertices, arestas);
    cout << "Grafo escolhido: " << endl;
    Gerenciador::imprimeListaAdj(grafo);

    string nome_sem_extensao = string(argv[1]).substr(0, string(argv[1]).length() - 4);
    std::filesystem::create_directory(nome_sem_extensao);

    Gerenciador::comandos(grafo, nome_sem_extensao);

    return 0;
}
