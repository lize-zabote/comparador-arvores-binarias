#include <stdlib.h>
#include <stdio.h>

// Contador de esforço algorítmico
int contRubro = 0;

// Estruturas da Árvore Rubro-Negra
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
    struct noRubro* nulo; // Nó sentinela
} ArvoreRubro;

// --- PROTÓTIPOS DAS FUNÇÕES ---
ArvoreRubro* criar();
NoRubro* criarNo(ArvoreRubro* arvore, NoRubro* pai, int valor);
NoRubro* adicionar(ArvoreRubro* arvore, int valor);
NoRubro* adicionarNo(ArvoreRubro* arvore, NoRubro* noRubro, int valor);
void balancear(ArvoreRubro* arvore, NoRubro* noRubro);
void remover(ArvoreRubro* arvore, int valor);
void removerNo(ArvoreRubro* arvore, NoRubro* noRemover);
void balancearRemocao(ArvoreRubro* arvore, NoRubro* no);
void transplantar(ArvoreRubro* arvore, NoRubro* u, NoRubro* v);
NoRubro* minimo(ArvoreRubro* arvore, NoRubro* no);
NoRubro* localizar(ArvoreRubro* arvore, int valor);
void rotacionarEsquerda(ArvoreRubro* arvore, NoRubro* noRubro);
void rotacionarDireita(ArvoreRubro* arvore, NoRubro* noRubro);
void percorrerProfundidadeInOrder(ArvoreRubro* arvore, NoRubro* noRubro, void (*callback)(int));
void visitar(int valor);

ArvoreRubro* criar() {
    ArvoreRubro *arvore = malloc(sizeof(ArvoreRubro));
    
    // O nó nulo (sentinela) é crucial para simplificar o código
    arvore->nulo = malloc(sizeof(NoRubro));
    arvore->nulo->cor = Preto;
    arvore->nulo->valor = 0;
    arvore->nulo->pai = NULL;
    arvore->nulo->esquerda = NULL;
    arvore->nulo->direita = NULL;
    
    arvore->raiz = arvore->nulo;
    return arvore;
}

int vazia(ArvoreRubro* arvore) {
    return arvore->raiz == arvore->nulo;
}

NoRubro* criarNo(ArvoreRubro* arvore, NoRubro* pai, int valor) {
    NoRubro* noRubro = malloc(sizeof(NoRubro));
    noRubro->pai = pai;
    noRubro->valor = valor;
    noRubro->direita = arvore->nulo;
    noRubro->esquerda = arvore->nulo;
    noRubro->cor = Vermelho; // Novos nós são sempre vermelhos inicialmente
    return noRubro;
}

// --- INSERÇÃO E BALANCEAMENTO PÓS-INSERÇÃO ---

NoRubro* adicionar(ArvoreRubro* arvore, int valor) {
    if (vazia(arvore)) {
        arvore->raiz = criarNo(arvore, arvore->nulo, valor);
        arvore->raiz->cor = Preto; // A raiz é sempre preta
        return arvore->raiz;
    } else {
        NoRubro* novoNo = adicionarNo(arvore, arvore->raiz, valor);
        balancear(arvore, novoNo);
        return novoNo;
    }
}

NoRubro* adicionarNo(ArvoreRubro* arvore, NoRubro* noRubro, int valor) {
    contRubro++; // Conta a "descida" recursiva
    if (valor > noRubro->valor) {
        if (noRubro->direita == arvore->nulo) {
            noRubro->direita = criarNo(arvore, noRubro, valor);
            return noRubro->direita;
        } else {
            return adicionarNo(arvore, noRubro->direita, valor);
        }
    } else {
        if (noRubro->esquerda == arvore->nulo) {
            noRubro->esquerda = criarNo(arvore, noRubro, valor);
            return noRubro->esquerda;
        } else {
            return adicionarNo(arvore, noRubro->esquerda, valor);
        }
    }
}

void balancear(ArvoreRubro* arvore, NoRubro* noRubro) {
    while (noRubro != arvore->raiz && noRubro->pai->cor == Vermelho) {
        contRubro++; // Conta a "subida" / verificação
        NoRubro* pai = noRubro->pai;
        NoRubro* avo = pai->pai;

        if (pai == avo->esquerda) {
            NoRubro *tio = avo->direita;
            if (tio->cor == Vermelho) { // Caso 1: Tio é vermelho
                tio->cor = Preto;
                pai->cor = Preto;
                avo->cor = Vermelho;
                noRubro = avo; // Sobe o ponteiro para o avô
                contRubro += 2; // Contabiliza a subida de 2 níveis
            } else { // Tio é preto
                if (noRubro == pai->direita) { // Caso 2: Zigue-zague
                    noRubro = pai; // Sobe um nível
                    contRubro++;
                    rotacionarEsquerda(arvore, noRubro);
                }
                // Caso 3: Linha reta
                noRubro->pai->cor = Preto;
                noRubro->pai->pai->cor = Vermelho;
                rotacionarDireita(arvore, noRubro->pai->pai);
            }
        } else { // Lado direito (simétrico)
            NoRubro *tio = avo->esquerda;
            if (tio->cor == Vermelho) { // Caso 1
                tio->cor = Preto;
                pai->cor = Preto;
                avo->cor = Vermelho;
                noRubro = avo;
                contRubro += 2;
            } else {
                if (noRubro == pai->esquerda) { // Caso 2
                    noRubro = pai;
                    contRubro++;
                    rotacionarDireita(arvore, noRubro);
                }
                // Caso 3
                noRubro->pai->cor = Preto;
                noRubro->pai->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, noRubro->pai->pai);
            }
        }
    }
    arvore->raiz->cor = Preto;
}


// --- REMOÇÃO E BALANCEAMENTO PÓS-REMOÇÃO ---

void remover(ArvoreRubro* arvore, int valor) {
    printf("Removendo %d...\n", valor);
    NoRubro* noRemover = localizar(arvore, valor);
    if (noRemover != arvore->nulo) {
        removerNo(arvore, noRemover);
    } else {
        printf("Valor %d nao encontrado para remocao.\n", valor);
    }
}

void removerNo(ArvoreRubro* arvore, NoRubro* noRemover) {
    NoRubro* y = noRemover;
    NoRubro* x;
    Cor corOriginalY = y->cor;

    if (noRemover->esquerda == arvore->nulo) {
        x = noRemover->direita;
        transplantar(arvore, noRemover, noRemover->direita);
    } else if (noRemover->direita == arvore->nulo) {
        x = noRemover->esquerda;
        transplantar(arvore, noRemover, noRemover->esquerda);
    } else {
        y = minimo(arvore, noRemover->direita);
        corOriginalY = y->cor;
        x = y->direita;
        if (y->pai == noRemover) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->direita);
            y->direita = noRemover->direita;
            y->direita->pai = y;
        }
        transplantar(arvore, noRemover, y);
        y->esquerda = noRemover->esquerda;
        y->esquerda->pai = y;
        y->cor = noRemover->cor;
    }
    free(noRemover);

    // Se o nó removido era preto, a árvore precisa ser rebalanceada
    if (corOriginalY == Preto) {
        balancearRemocao(arvore, x);
    }
}

void balancearRemocao(ArvoreRubro* arvore, NoRubro* no) {
    while (no != arvore->raiz && no->cor == Preto) {
        contRubro++; // Conta a "subida" do problema do "duplo-preto"
        if (no == no->pai->esquerda) {
            NoRubro* irmao = no->pai->direita;
            if (irmao->cor == Vermelho) { // Caso 1
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no->pai);
                irmao = no->pai->direita;
            }
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) { // Caso 2
                irmao->cor = Vermelho;
                no = no->pai; // O problema sobe na árvore
            } else {
                if (irmao->direita->cor == Preto) { // Caso 3
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireita(arvore, irmao);
                    irmao = no->pai->direita;
                }
                // Caso 4
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerda(arvore, no->pai);
                no = arvore->raiz; // Problema resolvido
            }
        } else { // Casos simétricos
            NoRubro* irmao = no->pai->esquerda;
            if (irmao->cor == Vermelho) { // Caso 1
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarDireita(arvore, no->pai);
                irmao = no->pai->esquerda;
            }
            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) { // Caso 2
                irmao->cor = Vermelho;
                no = no->pai;
            } else {
                if (irmao->esquerda->cor == Preto) { // Caso 3
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerda(arvore, irmao);
                    irmao = no->pai->esquerda;
                }
                // Caso 4
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireita(arvore, no->pai);
                no = arvore->raiz;
            }
        }
    }
    no->cor = Preto;
}


// --- FUNÇÕES AUXILIARES ---

void transplantar(ArvoreRubro* arvore, NoRubro* u, NoRubro* v) {
    if (u->pai == arvore->nulo) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

NoRubro* minimo(ArvoreRubro* arvore, NoRubro* no) {
    while (no->esquerda != arvore->nulo) {
        contRubro++; // Conta a "descida" para o mínimo
        no = no->esquerda;
    }
    return no;
}

NoRubro* localizar(ArvoreRubro* arvore, int valor) {
    NoRubro* noRubro = arvore->raiz;
    while (noRubro != arvore->nulo) {
        if (noRubro->valor == valor) {
            return noRubro;
        }
        contRubro++; // Conta a "descida" na busca
        noRubro = valor < noRubro->valor ? noRubro->esquerda : noRubro->direita;
    }
    return arvore->nulo; // Retorna o sentinela se não encontrar
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

// --- FUNÇÕES DE EXIBIÇÃO ---

void percorrerProfundidadeInOrder(ArvoreRubro* arvore, NoRubro* noRubro, void (*callback)(int)) {
    if (noRubro != arvore->nulo) {
        percorrerProfundidadeInOrder(arvore, noRubro->esquerda, callback);
        callback(noRubro->valor);
        percorrerProfundidadeInOrder(arvore, noRubro->direita, callback);
    }
}

void visitar(int valor) {
    printf("%d ", valor);
}

int main() {
    ArvoreRubro* a = criar();
    
    printf("--- INSERINDO ELEMENTOS ---\n");
    adicionar(a, 10);
    adicionar(a, 85);
    adicionar(a, 15);
    adicionar(a, 70);
    adicionar(a, 20);
    adicionar(a, 60);
    adicionar(a, 30);
    adicionar(a, 50);

    printf("\nIn-order inicial: ");
    percorrerProfundidadeInOrder(a, a->raiz, visitar);
    printf("\n");
    printf("Contador Rubro-Negro apos insercoes: %d\n\n", contRubro);
    
    printf("--- REMOVENDO ELEMENTOS ---\n");
    contRubro = 0; // Reseta para medir apenas as remoções
    remover(a, 15);
    remover(a, 70);
    remover(a, 10);
    
    printf("\nIn-order final: ");
    percorrerProfundidadeInOrder(a, a->raiz, visitar);
    printf("\n");
    printf("Contador Rubro-Negro nas remocoes: %d\n", contRubro);
    
    // free(a->nulo);
    // free(a);
    return 0;
}