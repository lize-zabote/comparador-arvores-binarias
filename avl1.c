#include <stdlib.h>
#include <stdio.h>

// Contador de esforço algorítmico
int contAVL = 0;

// Estruturas da Árvore AVL
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

// --- PROTÓTIPOS DAS FUNÇÕES ---
ArvoreAVL* criar();
void adicionar(ArvoreAVL* arvoreAVL, int valor);
void remover(ArvoreAVL* arvoreAVL, int valor);
NoAVL* localizar(NoAVL* noAVL, int valor);
void balanceamento(ArvoreAVL*, NoAVL*);
int altura(NoAVL*);
int fb(NoAVL*);
NoAVL* rsd(ArvoreAVL*, NoAVL*);
NoAVL* rse(ArvoreAVL*, NoAVL*);
NoAVL* rdd(ArvoreAVL*, NoAVL*);
NoAVL* rde(ArvoreAVL*, NoAVL*);
void visitar(int valor);
void percorrer(NoAVL* noAVL, void (*callback)(int));

int maximo(int a, int b) {
    return a > b ? a : b;
}

ArvoreAVL* criar() {
    ArvoreAVL *arvoreAVL = malloc(sizeof(ArvoreAVL));
    arvoreAVL->raiz = NULL;
    return arvoreAVL;
}

int altura(NoAVL* noAVL) {
    return noAVL != NULL ? noAVL->altura : 0;
}

int fb(NoAVL* noAVL) {
    if (noAVL == NULL) return 0;
    return altura(noAVL->esquerda) - altura(noAVL->direita);
}

void adicionar(ArvoreAVL* arvoreAVL, int valor) {
    NoAVL* noAVL = arvoreAVL->raiz;

    // Desce na árvore para encontrar a posição de inserção
    while (noAVL != NULL) {
        contAVL++; // Incrementa ao "descer"
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

    // Cria o novo nó
    NoAVL* novo = malloc(sizeof(NoAVL));
    novo->valor = valor;
    novo->pai = noAVL;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1;

    // Insere o novo nó
    if (noAVL == NULL) {
        arvoreAVL->raiz = novo;
    } else {
        if (valor > noAVL->valor) {
            noAVL->direita = novo;
        } else {
            noAVL->esquerda = novo;
        }
        // Inicia o processo de balanceamento subindo a partir do pai do novo nó
        balanceamento(arvoreAVL, noAVL);
    }
}

void balanceamento(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    // Sobe na árvore atualizando alturas e rebalanceando
    while (noAVL != NULL) {
        contAVL++; // Incrementa ao "subir"
        noAVL->altura = maximo(altura(noAVL->esquerda), altura(noAVL->direita)) + 1;
        int fator = fb(noAVL);

        if (fator > 1) { // Árvore mais pesada para esquerda
            if (fb(noAVL->esquerda) >= 0) { // Sinais iguais: Rotação Simples
                rsd(arvoreAVL, noAVL);
            } else { // Sinais diferentes: Rotação Dupla
                rdd(arvoreAVL, noAVL);
            }
        } else if (fator < -1) { // Árvore mais pesada para a direita
            if (fb(noAVL->direita) <= 0) { // Sinais iguais: Rotação Simples
                rse(arvoreAVL, noAVL);
            } else { // Sinais diferentes: Rotação Dupla
                rde(arvoreAVL, noAVL);
            }
        }
        noAVL = noAVL->pai;
    }
}

// --- FUNÇÕES DE ROTAÇÃO ---

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

// --- FUNÇÕES DE BUSCA E REMOÇÃO ---

NoAVL* localizar(NoAVL* noAVL, int valor) {
    while (noAVL != NULL) {
        if (noAVL->valor == valor) {
            return noAVL;
        }
        contAVL++; // Incrementa ao "descer" na busca
        noAVL = valor < noAVL->valor ? noAVL->esquerda : noAVL->direita;
    }
    return NULL;
}

NoAVL* minimo(NoAVL* no) {
    while (no != NULL && no->esquerda != NULL) {
        contAVL++; // Incrementa ao "descer" para encontrar o mínimo
        no = no->esquerda;
    }
    return no;
}

void removerNo(ArvoreAVL* arvore, NoAVL* noRemover) {
    NoAVL* noPai = noRemover->pai;
    
    // Caso 1 e 2: O nó a ser removido tem 0 ou 1 filho
    if (noRemover->esquerda == NULL || noRemover->direita == NULL) {
        NoAVL* filho = noRemover->esquerda != NULL ? noRemover->esquerda : noRemover->direita;

        if (noPai == NULL) {
            arvore->raiz = filho;
        } else if (noPai->esquerda == noRemover) {
            noPai->esquerda = filho;
        } else {
            noPai->direita = filho;
        }

        if (filho != NULL) {
            filho->pai = noPai;
        }
        free(noRemover);
    // Caso 3: O nó a ser removido tem 2 filhos
    } else {
        // Encontra o sucessor em-ordem (o menor nó da subárvore direita)
        NoAVL* sucessor = minimo(noRemover->direita);
        noRemover->valor = sucessor->valor; // Copia o valor do sucessor
        removerNo(arvore, sucessor); // Remove o nó sucessor (que terá 0 ou 1 filho)
        return; // Retorna para evitar rebalanceamento duplicado
    }

    // Inicia o balanceamento a partir do pai do nó fisicamente removido
    if (noPai != NULL) {
        balanceamento(arvore, noPai);
    }
}

void remover(ArvoreAVL* arvore, int valor) {
    printf("Removendo %d...\n", valor);
    NoAVL* noRemover = localizar(arvore->raiz, valor);
    if (noRemover != NULL) {
        removerNo(arvore, noRemover);
    } else {
        printf("Valor %d nao encontrado para remocao.\n", valor);
    }
}

// --- FUNÇÕES DE EXIBIÇÃO ---

void percorrer(NoAVL* noAVL, void (*callback)(int)) {
    if (noAVL != NULL) {
        percorrer(noAVL->esquerda, callback);
        callback(noAVL->valor);
        percorrer(noAVL->direita, callback);
    }
}

void visitar(int valor) {
    printf("%d ", valor);
}

int main() {
    ArvoreAVL* a = criar();

    printf("--- INSERINDO ELEMENTOS ---\n");
    adicionar(a, 10);
    adicionar(a, 20);
    adicionar(a, 30);
    adicionar(a, 40);
    adicionar(a, 50);
    adicionar(a, 25);

    printf("\nIn-order inicial: ");
    percorrer(a->raiz, visitar);
    printf("\n");
    printf("Contador AVL apos insercoes: %d\n\n", contAVL);

    printf("--- REMOVENDO ELEMENTOS ---\n");
    contAVL = 0; // Reseta para medir apenas a remoção
    remover(a, 40); // Remove um nó que causa rebalanceamento

    printf("\nIn-order apos remover 40: ");
    percorrer(a->raiz, visitar);
    printf("\n");
    printf("Contador AVL na remocao do 40: %d\n\n", contAVL);
    
    contAVL = 0;
    remover(a, 10);

    printf("\nIn-order apos remover 10: ");
    percorrer(a->raiz, visitar);
    printf("\n");
    printf("Contador AVL na remocao do 10: %d\n", contAVL);
    
    // free(a); // Idealmente, deveria haver uma função para liberar toda a memória
    return 0;
}