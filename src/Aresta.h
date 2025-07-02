#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta
{
private:
    char id_no_origem;
    char id_no_alvo;
    int peso;

public:
    Aresta(char id_no_origem, char id_no_alvo, int peso);
    ~Aresta();

    char getIdNoAlvo();
    char getIdNoOrigem();
    int getPeso();

    void setIdNoAlvo(char id_no_alvo);
    void setIdNoOrigem(char id_no_origem);
    void setPeso(int peso);
};

#endif // ARESTA_H
