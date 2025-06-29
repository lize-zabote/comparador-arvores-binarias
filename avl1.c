#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int contAVL = 0;

typedef struct noAVL {
    struct noAVL* pai;
    struct noAVL* esquerda;
    struct noAVL* direita;
    int valor;
    int altura;
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* raiz;
} ArvoreAVL;

void balanceamento(ArvoreAVL*, NoAVL*);
int altura(NoAVL*);
int fb(NoAVL*);
NoAVL* rsd(ArvoreAVL*, NoAVL*);
NoAVL* rse(ArvoreAVL*, NoAVL*);
NoAVL* rdd(ArvoreAVL*, NoAVL*);
NoAVL* rde(ArvoreAVL*, NoAVL*);

int maxx(int a, int b) {
    return a > b ? a : b;
}

ArvoreAVL* criar() {
    ArvoreAVL *arvoreAVL = malloc(sizeof(ArvoreAVL));
    arvoreAVL->raiz = NULL;
  
    return arvoreAVL;
}

int vazia(ArvoreAVL* arvoreAVL) {
    return arvoreAVL->raiz == NULL;
}

void adicionar(ArvoreAVL* arvoreAVL, int valor) {
    NoAVL* noAVL = arvoreAVL->raiz;

    contAVL++;

    while (noAVL != NULL) {
        if (valor > noAVL->valor) {
            if (noAVL->direita != NULL) {
                noAVL = noAVL->direita;
            } else {
                break;
            }
        } else {
            if (noAVL->esquerda != NULL) {
                noAVL = noAVL->esquerda;
            } else {
                break;
            }
        }
    }

    NoAVL* novo = malloc(sizeof(NoAVL));
    novo->valor = valor;
    novo->pai = noAVL;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1;

    if (noAVL == NULL) {    
        arvoreAVL->raiz = novo;
    } else {
        if (valor > noAVL->valor) {
            noAVL->direita = novo;
        } else {
            noAVL->esquerda = novo;
        }
        
        balanceamento(arvoreAVL, noAVL);
    }
}

NoAVL* localizar(NoAVL* noAVL, int valor) {
    while (noAVL != NULL) {
        if (noAVL->valor == valor) {
            return noAVL;
        }
        
        noAVL = valor < noAVL->valor ? noAVL->esquerda : noAVL->direita;
    }

    return NULL;
}

void percorrer(NoAVL* noAVL, void (*callback)(int)) {
    if (noAVL != NULL) {
        percorrer(noAVL->esquerda,callback);
        callback(noAVL->valor);
        percorrer(noAVL->direita,callback);
    }
}

void percorrerProfundidadeInOrder(ArvoreAVL* arvoreAVL, NoAVL* noAVL, void (*callback)(int)) {
    if (noAVL != NULL) {
        
        
        percorrerProfundidadeInOrder(arvoreAVL, noAVL->esquerda,callback);
        callback(noAVL->valor);
        percorrerProfundidadeInOrder(arvoreAVL, noAVL->direita,callback);
    }
}

void visitar(int valor){
    printf("%d ", valor);
}

void balanceamento(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    while (noAVL != NULL) {
        noAVL->altura = maxx(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
        int fator = fb(noAVL);

        if (fator > 1) { //árvore mais pesada para esquerda
            //rotação para a direita
            if (fb(noAVL->esquerda) > 0) {
                rsd(arvoreAVL, noAVL); //rotação simples a direita, pois o FB do filho tem sinal igual
            } else {
                rdd(arvoreAVL, noAVL); //rotação dupla a direita, pois o FB do filho tem sinal diferente
            }
        } else if (fator < -1) { //árvore mais pesada para a direita
            //rotação para a esquerda
            if (fb(noAVL->direita) < 0) {
                rse(arvoreAVL, noAVL); //rotação simples a esquerda, pois o FB do filho tem sinal igual
            } else {
                rde(arvoreAVL, noAVL); //rotação dupla a esquerda, pois o FB do filho tem sinal diferente
            }
        }

        noAVL = noAVL->pai; 
    }
}

int altura(NoAVL* noAVL){
    return noAVL != NULL ? noAVL->altura : 0;
}

int fb(NoAVL* noAVL) {
    int esquerda = 0,direita = 0;
  
    if (noAVL->esquerda != NULL) {
        esquerda = noAVL->esquerda->altura;
    }

    if (noAVL->direita != NULL) {
        direita = noAVL->direita->altura;
    }
  
    return esquerda - direita;
}

NoAVL* rse(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    NoAVL* pai = noAVL->pai;
    NoAVL* direita = noAVL->direita;

    if (direita->esquerda != NULL) {
        direita->esquerda->pai = noAVL;
    } 
  
    noAVL->direita = direita->esquerda;
    noAVL->pai = direita;

    direita->esquerda = noAVL;
    direita->pai = pai;

    if (pai == NULL) {
        arvoreAVL->raiz = direita;
    } else {
        if (pai->esquerda == noAVL) {
            pai->esquerda = direita;
        } else {
            pai->direita = direita;
        }
    }

    noAVL->altura = maxx(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
    direita->altura = maxx(altura(direita->esquerda), altura(direita->direita)) + 1;

    return direita;
}

NoAVL* rsd(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    NoAVL* pai = noAVL->pai;
    NoAVL* esquerda = noAVL->esquerda;

    if (esquerda->direita != NULL) {
        esquerda->direita->pai = noAVL;
    } 
  
    noAVL->esquerda = esquerda->direita;
    noAVL->pai = esquerda;
  
    esquerda->direita = noAVL;
    esquerda->pai = pai;

    if (pai == NULL) {
        arvoreAVL->raiz = esquerda;
    } else {
        if (pai->esquerda == noAVL) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }

    noAVL->altura = maxx(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
    esquerda->altura = maxx(altura(esquerda->esquerda), altura(esquerda->direita)) + 1;

    return esquerda;
}

NoAVL* rde(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    noAVL->direita = rsd(arvoreAVL, noAVL->direita);
    return rse(arvoreAVL, noAVL);
}

NoAVL* rdd(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    noAVL->esquerda = rse(arvoreAVL, noAVL->esquerda);
    return rsd(arvoreAVL, noAVL);
}

int main() {
    ArvoreAVL* a = criar();
    ArvoreAVL* b = criar();
    ArvoreAVL* c = criar();

    //Ordem 1 = qtdDeFolhas: 1+2 = 3
    //Ordem 5 = qtdDeFolhas: 1+2+4+8+16+32 = 63
    //Ordem 10 = qtdDeFolhas: 1+2+4+8+16+32+64+128+256+512+1024 = 2047
    int qtdDeFolhas[3] = {3,63,2047};
    ArvoreAVL* arvores[3] = {a,b,c};

    srand(time(NULL));

    // for (int x = 0; x < 3; x++)
    // {
    //     for (int i = 0; i < qtdDeFolhas[x]; i++)
    //     {
    //         adicionar(arvores[x],(rand() % 10000) + 1);
    //     }
    //     printf("In-order: ");
    //     percorrerProfundidadeInOrder(arvores[x], arvores[x]->raiz,visitar);
    //     printf("\n");
    // }

    int media[3] = {0,0,0};

    for (int x = 0; x < 3; x++) //interação em cada ordem
    {
        for (int v = 0; v < 10; v++) //interação para definir as médias por ordem
        {
            contAVL = 0;
            for (int i = 0; i < qtdDeFolhas[x]; i++)
            {
                adicionar(arvores[x],(rand() % 10000) + 1);
            }
            printf("In-order: ");
            percorrerProfundidadeInOrder(arvores[x], arvores[x]->raiz,visitar);
            printf("\nNúmero de operações: %d\n", contAVL);
            media[x] += contAVL;
            printf("\n");
        }
        
        
    }
    printf("\nMédia de operações: %d\n", (media[0] / 10));
    printf("\nMédia de operações: %d\n", (media[1] / 10));
    printf("\nMédia de operações: %d\n", (media[2] / 10));
}
