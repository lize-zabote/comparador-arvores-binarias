#include <stdlib.h>
#include <stdio.h>

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

int maximo(int a, int b) {
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

    printf("Adicionando %d\n", valor);
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

void visitar(int valor){
    printf("%d ", valor);
}

void balanceamento(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    while (noAVL != NULL) {
        noAVL->altura = maximo(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
        int fator = fb(noAVL);

        if (fator > 1) { //árvore mais pesada para esquerda
            //rotação para a direita
            if (fb(noAVL->esquerda) > 0) {
                printf("RSD(%d)\n",noAVL->valor);
                rsd(arvoreAVL, noAVL); //rotação simples a direita, pois o FB do filho tem sinal igual
            } else {
                printf("RDD(%d)\n",noAVL->valor);
                rdd(arvoreAVL, noAVL); //rotação dupla a direita, pois o FB do filho tem sinal diferente
            }
        } else if (fator < -1) { //árvore mais pesada para a direita
            //rotação para a esquerda
            if (fb(noAVL->direita) < 0) {
                printf("RSE(%d)\n",noAVL->valor);
                rse(arvoreAVL, noAVL); //rotação simples a esquerda, pois o FB do filho tem sinal igual
            } else {
                printf("RDE(%d)\n",noAVL->valor);
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

    noAVL->altura = maximo(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
    direita->altura = maximo(altura(direita->esquerda), altura(direita->direita)) + 1;

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

    noAVL->altura = maximo(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
    esquerda->altura = maximo(altura(esquerda->esquerda), altura(esquerda->direita)) + 1;

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

    adicionar(a,1);
    adicionar(a,2);
    adicionar(a,3);
    adicionar(a,7);
    adicionar(a,6);
    adicionar(a,5);
    adicionar(a,4);

    printf("In-order: ");
    percorrer(a->raiz,visitar);
    printf("\n");
}