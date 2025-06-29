#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int contRubro = 0;

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct noRubro {
    struct noRubro* pai;
    struct noRubro* esquerda;
    struct noRubro* direita;
    Cor cor;
    int valor;
} NoRubro;

typedef struct arvore {
    struct noRubro* raiz;
    struct noRubro* nulo; 
} ArvoreRubro;

NoRubro* criarNo(ArvoreRubro*, NoRubro*, int);
void balancear(ArvoreRubro*, NoRubro*);
void rotacionarEsquerda(ArvoreRubro*, NoRubro*);
void rotacionarDireita(ArvoreRubro*, NoRubro*);

ArvoreRubro* criar();
int vazia(ArvoreRubro*);
NoRubro* adicionar(ArvoreRubro*, int);
NoRubro* localizar(ArvoreRubro* arvore, int valor);

ArvoreRubro* criar() {
    ArvoreRubro *arvore = malloc(sizeof(ArvoreRubro));
    arvore->nulo = NULL;
    arvore->raiz = NULL;

    arvore->nulo = criarNo(arvore, NULL, 0);
    arvore->nulo->cor = Preto;

    return arvore;
}

int vazia(ArvoreRubro* arvore) {
    return arvore->raiz == NULL;
}

NoRubro* criarNo(ArvoreRubro* arvore, NoRubro* pai, int valor) {
    NoRubro* noRubro = malloc(sizeof(NoRubro));

    noRubro->pai = pai;    
    noRubro->valor = valor;
    noRubro->direita = arvore->nulo;
    noRubro->esquerda = arvore->nulo;

    return noRubro;
}

NoRubro* adicionarNo(ArvoreRubro* arvore, NoRubro* noRubro, int valor) {
    if (valor > noRubro->valor) {
        if (noRubro->direita == arvore->nulo) {
            noRubro->direita = criarNo(arvore, noRubro, valor);     
            noRubro->direita->cor = Vermelho;       
        		
            return noRubro->direita;
        } else {
            return adicionarNo(arvore, noRubro->direita, valor);
        }
    } else {
        if (noRubro->esquerda == arvore->nulo) {
            noRubro->esquerda = criarNo(arvore, noRubro, valor);
            noRubro->esquerda->cor = Vermelho;
            
            return noRubro->esquerda;
        } else {
            return adicionarNo(arvore, noRubro->esquerda, valor);
        }
    }
}

NoRubro* adicionar(ArvoreRubro* arvore, int valor) {
    if (vazia(arvore)) {
        arvore->raiz = criarNo(arvore, arvore->nulo, valor);
        arvore->raiz->cor = Preto;
        	
        return arvore->raiz;
    } else {
        NoRubro* noRubro = adicionarNo(arvore, arvore->raiz, valor);
        balancear(arvore, noRubro);
        
        return noRubro;
    }
}

NoRubro* localizar(ArvoreRubro* arvore, int valor) {
    if (!vazia(arvore)) {
        NoRubro* noRubro = arvore->raiz;

        while (noRubro != arvore->nulo) {
            if (noRubro->valor == valor) {
                return noRubro;
            } else {
                noRubro = valor < noRubro->valor ? noRubro->esquerda : noRubro->direita;
            }
        }
    }

    return NULL;
}

void percorrerProfundidadeInOrder(ArvoreRubro* arvore, NoRubro* noRubro, void (*callback)(int)) {
    if (noRubro != arvore->nulo) {
        
        
        percorrerProfundidadeInOrder(arvore, noRubro->esquerda,callback);
        callback(noRubro->valor);
        percorrerProfundidadeInOrder(arvore, noRubro->direita,callback);
    }
}

void percorrerProfundidadePreOrder(ArvoreRubro* arvore, NoRubro* noRubro, void (*callback)(int)) {
    if (noRubro != arvore->nulo) {
        callback(noRubro->valor);
        percorrerProfundidadePreOrder(arvore, noRubro->esquerda,callback);
        percorrerProfundidadePreOrder(arvore, noRubro->direita,callback);
    }
}

void percorrerProfundidadePosOrder(ArvoreRubro* arvore, NoRubro* noRubro, void (callback)(int)) {
    if (noRubro != arvore->nulo) {
        percorrerProfundidadePosOrder(arvore, noRubro->esquerda,callback);
        percorrerProfundidadePosOrder(arvore, noRubro->direita,callback);
        callback(noRubro->valor);
    }
}

void visitar(int valor){
    printf("%d ", valor);
}

void balancear(ArvoreRubro* arvore, NoRubro* noRubro) {
    while (noRubro->pai->cor == Vermelho) {
        if (noRubro->pai == noRubro->pai->pai->esquerda) {
            NoRubro *tio = noRubro->pai->pai->direita;
            
            if (tio->cor == Vermelho) {
                tio->cor = Preto; //Caso 1
                noRubro->pai->cor = Preto; 

                noRubro->pai->pai->cor = Vermelho; //Caso 1
                noRubro = noRubro->pai->pai; //Caso 1
            } else {
                if (noRubro == noRubro->pai->direita) {
                    noRubro = noRubro->pai; //Caso 2
                    rotacionarEsquerda(arvore, noRubro); //Caso 2
                } else {
                    noRubro->pai->cor = Preto; 
                    noRubro->pai->pai->cor = Vermelho; //Caso 3
                    rotacionarDireita(arvore, noRubro->pai->pai); //Caso 3
                }
            }
        } else {
            NoRubro *tio = noRubro->pai->pai->esquerda;
            
            if (tio->cor == Vermelho) {
                tio->cor = Preto; //Caso 1
                noRubro->pai->cor = Preto; 

                noRubro->pai->pai->cor = Vermelho; //Caso 1
                noRubro = noRubro->pai->pai; //Caso 1
            } else {
                if (noRubro == noRubro->pai->esquerda) {
                    noRubro = noRubro->pai; //Caso 2
                    rotacionarDireita(arvore, noRubro); //Caso 2
                } else {
                    noRubro->pai->cor = Preto; 
                    noRubro->pai->pai->cor = Vermelho; //Caso 3
                    rotacionarEsquerda(arvore, noRubro->pai->pai); //Caso 3
                }
            }
        }
    }
    arvore->raiz->cor = Preto; //Conserta possível quebra regra 2
}

void rotacionarEsquerda(ArvoreRubro* arvore, NoRubro* noRubro) {
    NoRubro* direita = noRubro->direita;
    noRubro->direita = direita->esquerda; 

    if (direita->esquerda != arvore->nulo) {
        direita->esquerda->pai = noRubro;
    }

    direita->pai = noRubro->pai;
    
    if (noRubro->pai == arvore->nulo) {
        arvore->raiz = direita;
    } else if (noRubro == noRubro->pai->esquerda) {
        noRubro->pai->esquerda = direita;
    } else {
        noRubro->pai->direita = direita;
    }

    direita->esquerda = noRubro;
    noRubro->pai = direita;
}

void rotacionarDireita(ArvoreRubro* arvore, NoRubro* noRubro) {
    NoRubro* esquerda = noRubro->esquerda;
    noRubro->esquerda = esquerda->direita;
    
    if (esquerda->direita != arvore->nulo) {
        esquerda->direita->pai = noRubro;
    }
    
    esquerda->pai = noRubro->pai;
    
    if (noRubro->pai == arvore->nulo) {
        arvore->raiz = esquerda;
    } else if (noRubro == noRubro->pai->esquerda) {
        noRubro->pai->esquerda = esquerda;
    } else {
        noRubro->pai->direita = esquerda;
    }
    
    esquerda->direita = noRubro;
    noRubro->pai = esquerda;
}

int main() {
    ArvoreRubro* a = criar();
    ArvoreRubro* b = criar();
    ArvoreRubro* c = criar();

    //Ordem 1 = qtdDeFolhas: 1+2 = 3
    //Ordem 5 = qtdDeFolhas: 1+2+4+8+16+32 = 63
    //Ordem 10 = qtdDeFolhas: 1+2+4+8+16+32+64+128+256+512+1024 = 2047
    int qtdDeFolhas[3] = {3,63,2047};
    ArvoreRubro* arvores[3] = {a,b,c};

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
            contRubro = 0;
            for (int i = 0; i < qtdDeFolhas[x]; i++)
            {
                adicionar(arvores[x],(rand() % 10000) + 1);
            }
            printf("In-order: ");
            percorrerProfundidadeInOrder(arvores[x], arvores[x]->raiz,visitar);
            printf("\nNúmero de operações: %d\n", contRubro);
            media[x] += contRubro;
            printf("\n");
        }
        
        
    }
    printf("\nMédia de operações: %d\n", (media[0] / 10));
    printf("\nMédia de operações: %d\n", (media[1] / 10));
    printf("\nMédia de operações: %d\n", (media[2] / 10));
}