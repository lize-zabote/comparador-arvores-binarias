#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* ============================================================================= */
/* --- FUNÇÕES UTILITÁRIAS --- */
/* ============================================================================= */

void embaralhar(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = n - 1; i > 0; i--) {
            size_t j = rand() % (i + 1);
            int temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

/* ============================================================================= */
/* --- MÓDULO: ÁRVORE AVL --- */
/* ============================================================================= */

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

ArvoreAVL* avl_criar();
void avl_destruir(ArvoreAVL* arvore);
void avl_adicionar(ArvoreAVL* arvore, int valor);
void avl_remover(ArvoreAVL* arvore, int valor);
int avl_altura(NoAVL* no);
int avl_fb(NoAVL* no);
void avl_percorrer(NoAVL* no, void (*callback)(int));
int avl_maximo(int a, int b);
void avl_balanceamento(ArvoreAVL* arvore, NoAVL* no);
NoAVL* avl_rsd(ArvoreAVL* arvore, NoAVL* no);
NoAVL* avl_rse(ArvoreAVL* arvore, NoAVL* no);
NoAVL* avl_rdd(ArvoreAVL* arvore, NoAVL* no);
NoAVL* avl_rde(ArvoreAVL* arvore, NoAVL* no);
NoAVL* avl_localizar(NoAVL* no, int valor);
void avl_removerNo(ArvoreAVL* arvore, NoAVL* noRemover);
NoAVL* avl_minimo(NoAVL* no);
void avl_destruir_no(NoAVL* no);

int avl_maximo(int a, int b) {
    contAVL++; // added counter for comparison
    return a > b ? a : b;
}

ArvoreAVL* avl_criar() {
    ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
    if (arvore == NULL) return NULL; // added null check
    arvore->raiz = NULL;
    return arvore;
}

int avl_altura(NoAVL* no) {
    contAVL++; // added counter for null check
    return no != NULL ? no->altura : 0;
}

int avl_fb(NoAVL* no) {
    contAVL++; // added counter for null check
    if (no == NULL) return 0;
    return avl_altura(no->esquerda) - avl_altura(no->direita);
}

void avl_adicionar(ArvoreAVL* arvore, int valor) {
    NoAVL* pai = NULL;
    NoAVL* atual = arvore->raiz;

    while (atual != NULL) {
        contAVL++; // count comparison
        pai = atual;
        contAVL++; // count comparison
        if (valor > atual->valor) {
            atual = atual->direita;
        } else {
            atual = atual->esquerda;
        }
    }

    NoAVL* novo = malloc(sizeof(NoAVL));
    if (novo == NULL) return; // added null check
    novo->valor = valor;
    novo->pai = pai;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1;

    contAVL++; // count null check
    if (pai == NULL) {
        arvore->raiz = novo;
    } else {
        contAVL++; // count comparison
        if (valor > pai->valor) {
            pai->direita = novo;
        } else {
            pai->esquerda = novo;
        }
        avl_balanceamento(arvore, pai);
    }
}

void avl_balanceamento(ArvoreAVL* arvore, NoAVL* no) {
    while (no != NULL) {
        contAVL++; // count null check
        no->altura = 1 + avl_maximo(avl_altura(no->esquerda), avl_altura(no->direita));
        int fator = avl_fb(no);
        contAVL++; // count comparison
        if (fator > 1) {
            contAVL++; // count comparison
            if (avl_fb(no->esquerda) >= 0) {
                avl_rsd(arvore, no);
            } else {
                avl_rdd(arvore, no);
            }
        } else if (fator < -1) {
            contAVL++; // count comparison
            if (avl_fb(no->direita) <= 0) {
                avl_rse(arvore, no);
            } else {
                avl_rde(arvore, no);
            }
        }
        no = no->pai;
    }
}

NoAVL* avl_rse(ArvoreAVL* arvore, NoAVL* no) {
    contAVL++; // count rotation operation
    NoAVL* pai = no->pai;
    NoAVL* direita = no->direita;

    contAVL++; // count null check
    if (direita->esquerda != NULL) direita->esquerda->pai = no;
    no->direita = direita->esquerda;
    no->pai = direita;
    direita->esquerda = no;
    direita->pai = pai;

    contAVL++; // count null check
    if (pai == NULL) {
        arvore->raiz = direita;
    } else {
        contAVL++; // count comparison
        if (pai->esquerda == no) pai->esquerda = direita;
        else pai->direita = direita;
    }

    no->altura = 1 + avl_maximo(avl_altura(no->esquerda), avl_altura(no->direita));
    direita->altura = 1 + avl_maximo(avl_altura(direita->esquerda), avl_altura(direita->direita));
    return direita;
}

NoAVL* avl_rsd(ArvoreAVL* arvore, NoAVL* no) {
    contAVL++; // count rotation operation
    NoAVL* pai = no->pai;
    NoAVL* esquerda = no->esquerda;

    contAVL++; // count null check
    if (esquerda->direita != NULL) esquerda->direita->pai = no;
    no->esquerda = esquerda->direita;
    no->pai = esquerda;
    esquerda->direita = no;
    esquerda->pai = pai;

    contAVL++; // count null check
    if (pai == NULL) {
        arvore->raiz = esquerda;
    } else {
        contAVL++; // count comparison
        if (pai->esquerda == no) pai->esquerda = esquerda;
        else pai->direita = esquerda;
    }

    no->altura = 1 + avl_maximo(avl_altura(no->esquerda), avl_altura(no->direita));
    esquerda->altura = 1 + avl_maximo(avl_altura(esquerda->esquerda), avl_altura(esquerda->direita));
    return esquerda;
}

NoAVL* avl_rde(ArvoreAVL* arvore, NoAVL* no) {
    contAVL++; // count rotation operation (replaced contAVL+=2)
    no->direita = avl_rsd(arvore, no->direita);
    return avl_rse(arvore, no);
}

NoAVL* avl_rdd(ArvoreAVL* arvore, NoAVL* no) {
    contAVL++; // count rotation operation (replaced contAVL+=2)
    no->esquerda = avl_rse(arvore, no->esquerda);
    return avl_rsd(arvore, no);
}

NoAVL* avl_localizar(NoAVL* no, int valor) {
    while (no != NULL) {
        contAVL++; // count comparison
        if (no->valor == valor) return no;
        contAVL++; // count comparison
        no = valor < no->valor ? no->esquerda : no->direita;
    }
    return NULL;
}

NoAVL* avl_minimo(NoAVL* no) {
    while (no != NULL && no->esquerda != NULL) {
        contAVL++; // count null check and comparison
        no = no->esquerda;
    }
    return no;
}

void avl_removerNo(ArvoreAVL* arvore, NoAVL* noRemover) {
    contAVL++; // count operation
    NoAVL* noPai = noRemover->pai;
    NoAVL* filho = (noRemover->esquerda != NULL) ? noRemover->esquerda : noRemover->direita;

    contAVL++; // count comparison
    if (noRemover->esquerda == NULL || noRemover->direita == NULL) {
        contAVL++; // count null check
        if (noPai == NULL) {
            arvore->raiz = filho;
        } else if (noPai->esquerda == noRemover) {
            contAVL++; // count comparison
            noPai->esquerda = filho;
        } else {
            noPai->direita = filho;
        }

        contAVL++; // count null check
        if (filho != NULL) filho->pai = noPai;
        free(noRemover);
    } else {
        NoAVL* sucessor = avl_minimo(noRemover->direita);
        noRemover->valor = sucessor->valor;
        avl_removerNo(arvore, sucessor);
        return;
    }

    contAVL++; // count null check
    if (noPai != NULL) {
        avl_balanceamento(arvore, noPai);
    }
}

void avl_remover(ArvoreAVL* arvore, int valor) {
    contAVL++; // count operation
    NoAVL* noRemover = avl_localizar(arvore->raiz, valor);
    contAVL++; // count null check
    if (noRemover != NULL) {
        avl_removerNo(arvore, noRemover);
    }
}

void avl_percorrer(NoAVL* no, void (*callback)(int)) {
    if (no != NULL) {
        avl_percorrer(no->esquerda, callback);
        callback(no->valor);
        avl_percorrer(no->direita, callback);
    }
}

void avl_destruir_no(NoAVL* no) {
    if (no == NULL) return;
    avl_destruir_no(no->esquerda);
    avl_destruir_no(no->direita);
    free(no);
}

void avl_destruir(ArvoreAVL* arvore) {
    if (arvore == NULL) return;
    avl_destruir_no(arvore->raiz);
    free(arvore);
}

/* ============================================================================= */
/* --- MÓDULO: ÁRVORE B --- */
/* ============================================================================= */

int contB = 0;

typedef struct noB {
    int total;
    int* chaves;
    struct noB** filhos;
    struct noB* pai;
} NoB;

typedef struct arvoreB {
    NoB* raiz;
    int ordem;
} ArvoreB;

ArvoreB* btree_criar(int ordem);
void btree_destruir(ArvoreB* arvore);
void btree_adicionar(ArvoreB* arvore, int chave);
void btree_remover(ArvoreB* arvore, int chave);
void btree_percorrer(NoB* no);
NoB* btree_criaNo(ArvoreB* arvore);
int btree_pesquisaBinaria(NoB* no, int chave);
NoB* btree_localizaNo(ArvoreB* arvore, int chave);
void btree_adicionar_recursivo(ArvoreB* arvore, NoB* no, NoB* novo, int chave);
void btree_adicionaChaveNo(NoB* no, NoB* novo, int chave);
NoB* btree_divideNo(ArvoreB* arvore, NoB* no);
int btree_transbordo(ArvoreB* arvore, NoB* no);
void btree_remover_interno(ArvoreB* arvore, NoB* no, int chave);
void remover_de_folha(NoB* no, int idx);
void remover_de_nao_folha(ArvoreB* arvore, NoB* no, int idx);
void preencher_filho(ArvoreB* arvore, NoB* no, int idx_filho);
void emprestar_do_anterior(NoB* no, int idx);
void emprestar_do_proximo(NoB* no, int idx);
void fundir_com_proximo(ArvoreB* arvore, NoB* no, int idx);
NoB* btree_getPredecessor(ArvoreB* arvore, NoB* no, int pos);
void btree_destruir_no(NoB* no);

ArvoreB* btree_criar(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    if (a == NULL) return NULL; // added null check
    a->ordem = ordem;
    a->raiz = btree_criaNo(a);
    contB++; // count operation
    return a;
}

NoB* btree_criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    NoB* no = malloc(sizeof(NoB));
    if (no == NULL) return NULL; // added null check
    no->pai = NULL;
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB*) * (max + 2));
    if (no->chaves == NULL || no->filhos == NULL) {
        free(no->chaves);
        free(no->filhos);
        free(no);
        return NULL; // added null check
    }
    no->total = 0;
    for (int i = 0; i < max + 2; i++) {
        contB++; // count loop iteration
        no->filhos[i] = NULL;
    }
    return no;
}

void btree_adicionar(ArvoreB* arvore, int chave) {
    NoB* no = btree_localizaNo(arvore, chave);
    contB++; // count operation
    btree_adicionar_recursivo(arvore, no, NULL, chave);
}

void btree_remover(ArvoreB* arvore, int chave) {
    contB++; // count operation
    contB++; // count null check
    if (!arvore->raiz) return;

    btree_remover_interno(arvore, arvore->raiz, chave);

    contB++; // count comparison
    if (arvore->raiz->total == 0 && arvore->raiz->filhos[0] != NULL) {
        NoB* tmp = arvore->raiz;
        arvore->raiz = arvore->raiz->filhos[0];
        arvore->raiz->pai = NULL;
        free(tmp->chaves);
        free(tmp->filhos);
        free(tmp);
    }
}

int btree_pesquisaBinaria(NoB* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
        contB++; // count comparison
        meio = inicio + (fim - inicio) / 2;
        contB++; // count comparison
        if (no->chaves[meio] == chave) return meio;
        contB++; // count comparison
        if (no->chaves[meio] > chave) fim = meio - 1;
        else inicio = meio + 1;
    }
    return inicio;
}

NoB* btree_localizaNo(ArvoreB* arvore, int chave) {
    NoB* no = arvore->raiz;
    while (no != NULL) {
        int i = btree_pesquisaBinaria(no, chave);
        contB++; // count null check
        if (no->filhos[i] == NULL) return no;
        contB++; // count operation
        no = no->filhos[i];
    }
    return NULL;
}

void btree_adicionar_recursivo(ArvoreB* arvore, NoB* no, NoB* novo, int chave) {
    btree_adicionaChaveNo(no, novo, chave);

    contB++; // count comparison
    if (btree_transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        NoB* novoNo = btree_divideNo(arvore, no);

        contB++; // count null check
        if (no->pai == NULL) {
            NoB* pai = btree_criaNo(arvore);
            pai->filhos[0] = no;
            btree_adicionaChaveNo(pai, novoNo, promovido);
            no->pai = pai;
            novoNo->pai = pai;
            arvore->raiz = pai;
            contB++; // count operation
        } else {
            contB++; // count operation
            btree_adicionar_recursivo(arvore, no->pai, novoNo, promovido);
        }
    }
}

void btree_adicionaChaveNo(NoB* no, NoB* novo, int chave) {
    int i = btree_pesquisaBinaria(no, chave);

    for (int j = no->total - 1; j >= i; j--) {
        contB++; // count loop iteration
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }

    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;
    contB++; // count null check
    if (novo != NULL) novo->pai = no;
    no->total++;
    contB++; // count operation
}

NoB* btree_divideNo(ArvoreB* arvore, NoB* no) {
    int meio = no->total / 2;
    NoB* novo = btree_criaNo(arvore);
    novo->pai = no->pai;

    contB++; // count operation
    for (int i = meio + 1; i < no->total; i++) {
        contB++; // count loop iteration
        novo->filhos[novo->total] = no->filhos[i];
        contB++; // count null check
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        novo->chaves[novo->total] = no->chaves[i];
        novo->total++;
    }
    contB++; // count null check
    novo->filhos[novo->total] = no->filhos[no->total];
    contB++; // count null check
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;

    no->total = meio;
    return novo;
}

int btree_transbordo(ArvoreB* arvore, NoB* no) {
    contB++; // count comparison
    return no->total > arvore->ordem * 2;
}

void btree_remover_interno(ArvoreB* arvore, NoB* no, int chave) {
    int idx = btree_pesquisaBinaria(no, chave);

    contB++; // count comparison
    if (idx < no->total && no->chaves[idx] == chave) {
        contB++; // count null check
        if (no->filhos[0] == NULL)
            remover_de_folha(no, idx);
        else
            remover_de_nao_folha(arvore, no, idx);
    } else {
        contB++; // count null check
        if (no->filhos[0] == NULL) return;

        bool ultimo_filho = (idx == no->total);
        contB++; // count comparison
        if (no->filhos[idx]->total < arvore->ordem) {
            preencher_filho(arvore, no, idx);
        }

        contB++; // count comparison
        if (ultimo_filho && idx > no->total) {
            contB++; // count operation (uncommented and standardized)
            btree_remover_interno(arvore, no->filhos[idx - 1], chave);
        } else {
            contB++; // count operation (uncommented and standardized)
            btree_remover_interno(arvore, no->filhos[idx], chave);
        }
    }
}

void remover_de_folha(NoB* no, int idx) {
    for (int i = idx + 1; i < no->total; ++i) {
        contB++; // count loop iteration
        no->chaves[i - 1] = no->chaves[i];
    }
    no->total--;
    contB++; // count operation
}

void remover_de_nao_folha(ArvoreB* arvore, NoB* no, int idx) {
    int chave = no->chaves[idx];

    contB++; // count comparison
    if (no->filhos[idx]->total >= arvore->ordem) {
        NoB* pred_node = btree_getPredecessor(arvore, no, idx);
        int predecessor = pred_node->chaves[pred_node->total - 1];
        no->chaves[idx] = predecessor;
        btree_remover_interno(arvore, no->filhos[idx], predecessor);
    } else if (no->filhos[idx + 1]->total >= arvore->ordem) {
        contB++; // count comparison
        NoB* suc_node = no->filhos[idx + 1];
        while (suc_node->filhos[0] != NULL) {
            contB++; // count null check
            suc_node = suc_node->filhos[0];
        }
        int sucessor = suc_node->chaves[0];
        no->chaves[idx] = sucessor;
        btree_remover_interno(arvore, no->filhos[idx + 1], sucessor);
    } else {
        fundir_com_proximo(arvore, no, idx);
        btree_remover_interno(arvore, no->filhos[idx], chave);
    }
}

void preencher_filho(ArvoreB* arvore, NoB* no, int idx_filho) {
    contB++; // count comparison
    if (idx_filho != 0 && no->filhos[idx_filho - 1]->total >= arvore->ordem)
        emprestar_do_anterior(no, idx_filho);
    else if (idx_filho != no->total && no->filhos[idx_filho + 1]->total >= arvore->ordem) {
        contB++; // count comparison
        emprestar_do_proximo(no, idx_filho);
    } else {
        contB++; // count comparison
        if (idx_filho != no->total)
            fundir_com_proximo(arvore, no, idx_filho);
        else
            fundir_com_proximo(arvore, no, idx_filho - 1);
    }
}

void emprestar_do_anterior(NoB* no, int idx) {
    contB++; // count operation (standardized counter)
    NoB* filho = no->filhos[idx];
    NoB* irmao = no->filhos[idx - 1];

    for (int i = filho->total - 1; i >= 0; --i) {
        contB++; // count loop iteration
        filho->chaves[i + 1] = filho->chaves[i];
    }
    contB++; // count null check
    if (filho->filhos[0] != NULL) {
        for (int i = filho->total; i >= 0; --i) {
            contB++; // count loop iteration
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }

    filho->chaves[0] = no->chaves[idx - 1];
    contB++; // count null check
    if (filho->filhos[0] != NULL) {
        filho->filhos[0] = irmao->filhos[irmao->total];
        contB++; // count null check
        if (filho->filhos[0]) filho->filhos[0]->pai = filho;
    }

    no->chaves[idx - 1] = irmao->chaves[irmao->total - 1];
    filho->total++;
    irmao->total--;
    contB++; // count operation
}

void emprestar_do_proximo(NoB* no, int idx) {
    contB++; // count operation (standardized counter)
    NoB* filho = no->filhos[idx];
    NoB* irmao = no->filhos[idx + 1];

    filho->chaves[filho->total] = no->chaves[idx];
    contB++; // count null check
    if (filho->filhos[0] != NULL) {
        filho->filhos[filho->total + 1] = irmao->filhos[0];
        contB++; // count null check
        if (filho->filhos[filho->total + 1]) filho->filhos[filho->total + 1]->pai = filho;
    }

    no->chaves[idx] = irmao->chaves[0];
    for (int i = 1; i < irmao->total; ++i) {
        contB++; // count loop iteration
        irmao->chaves[i - 1] = irmao->chaves[i];
    }
    contB++; // count null check
    if (irmao->filhos[0] != NULL) {
        for (int i = 1; i <= irmao->total; ++i) {
            contB++; // count loop iteration
            irmao->filhos[i - 1] = irmao->filhos[i];
        }
    }

    filho->total++;
    irmao->total--;
    contB++; // count operation
}

void fundir_com_proximo(ArvoreB* arvore, NoB* no, int idx) {
    contB++; // count operation (standardized counter)
    NoB* filho = no->filhos[idx];
    NoB* irmao = no->filhos[idx + 1];

    filho->chaves[arvore->ordem - 1] = no->chaves[idx];

    for (int i = 0; i < irmao->total; ++i) {
        contB++; // count loop iteration
        filho->chaves[i + arvore->ordem] = irmao->chaves[i];
    }
    contB++; // count null check
    if (filho->filhos[0] != NULL) {
        for (int i = 0; i <= irmao->total; ++i) {
            contB++; // count loop iteration
            filho->filhos[i + arvore->ordem] = irmao->filhos[i];
            contB++; // count null check
            if (filho->filhos[i + arvore->ordem]) filho->filhos[i + arvore->ordem]->pai = filho;
        }
    }

    for (int i = idx + 1; i < no->total; ++i) {
        contB++; // count loop iteration
        no->chaves[i - 1] = no->chaves[i];
    }
    for (int i = idx + 2; i <= no->total; ++i) {
        contB++; // count loop iteration
        no->filhos[i - 1] = no->filhos[i];
    }

    filho->total += irmao->total + 1;
    no->total--;

    free(irmao->chaves);
    free(irmao->filhos);
    free(irmao);
    contB++; // count operation
}

NoB* btree_getPredecessor(ArvoreB* arvore, NoB* no, int pos) {
    contB++; // count operation
    NoB* atual = no->filhos[pos];
    while (atual->filhos[atual->total] != NULL) {
        contB++; // count null check
        atual = atual->filhos[atual->total];
    }
    return atual;
}

void btree_percorrer(NoB* no) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++) {
            btree_percorrer(no->filhos[i]);
            printf("%d ", no->chaves[i]);
        }
        btree_percorrer(no->filhos[no->total]);
    }
}

void btree_destruir_no(NoB* no) {
    if (no == NULL) return;
    for (int i = 0; i <= no->total; i++) {
        btree_destruir_no(no->filhos[i]);
    }
    free(no->chaves);
    free(no->filhos);
    free(no);
}

void btree_destruir(ArvoreB* arvore) {
    if (arvore == NULL) return;
    btree_destruir_no(arvore->raiz);
    free(arvore);
}

/* ============================================================================= */
/* --- MÓDULO: ÁRVORE RUBRO-NEGRA --- */
/* ============================================================================= */

int contRubro = 0;

typedef enum { Vermelho, Preto } Cor;

typedef struct noRubro {
    struct noRubro* pai;
    struct noRubro* esquerda;
    struct noRubro* direita;
    Cor cor;
    int valor;
} NoRubro;

typedef struct arvoreRubro {
    struct noRubro* raiz;
    struct noRubro* nulo;
} ArvoreRubro;

ArvoreRubro* rb_criar();
void rb_destruir(ArvoreRubro* arvore);
void rb_adicionar(ArvoreRubro* arvore, int valor);
void rb_remover(ArvoreRubro* arvore, int valor);
void rb_percorrer(ArvoreRubro* arvore, NoRubro* no, void (*callback)(int));
bool rb_vazia(ArvoreRubro* arvore);
NoRubro* rb_criarNo(ArvoreRubro* arvore, NoRubro* pai, int valor);
NoRubro* rb_adicionarNo(ArvoreRubro* arvore, NoRubro* no, int valor);
void rb_balancear_insercao(ArvoreRubro* arvore, NoRubro* no);
void rb_removerNo(ArvoreRubro* arvore, NoRubro* noRemover);
void rb_balancear_remocao(ArvoreRubro* arvore, NoRubro* no);
void rb_transplantar(ArvoreRubro* arvore, NoRubro* u, NoRubro* v);
NoRubro* rb_minimo(ArvoreRubro* arvore, NoRubro* no);
NoRubro* rb_localizar(ArvoreRubro* arvore, int valor);
void rb_rotacionarEsquerda(ArvoreRubro* arvore, NoRubro* no);
void rb_rotacionarDireita(ArvoreRubro* arvore, NoRubro* no);
void rb_destruir_no(ArvoreRubro* arvore, NoRubro* no);

ArvoreRubro* rb_criar() {
    ArvoreRubro *arvore = malloc(sizeof(ArvoreRubro));
    if (arvore == NULL) return NULL; // added null check
    arvore->nulo = malloc(sizeof(NoRubro));
    if (arvore->nulo == NULL) {
        free(arvore);
        return NULL; // added null check
    }
    arvore->nulo->cor = Preto;
    arvore->nulo->valor = 0;
    arvore->nulo->pai = NULL;
    arvore->nulo->esquerda = NULL;
    arvore->nulo->direita = NULL;
    arvore->raiz = arvore->nulo;
    contRubro++; // count operation
    return arvore;
}

bool rb_vazia(ArvoreRubro* arvore) {
    contRubro++; // count comparison
    return arvore->raiz == arvore->nulo;
}

NoRubro* rb_criarNo(ArvoreRubro* arvore, NoRubro* pai, int valor) {
    NoRubro* no = malloc(sizeof(NoRubro));
    if (no == NULL) return NULL; // added null check
    no->pai = pai;
    no->valor = valor;
    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;
    no->cor = Vermelho;
    contRubro++; // count operation
    return no;
}

void rb_adicionar(ArvoreRubro* arvore, int valor) {
    contRubro++; // count operation
    if (rb_vazia(arvore)) {
        arvore->raiz = rb_criarNo(arvore, arvore->nulo, valor);
        contRubro++; // count null check
        if (arvore->raiz != NULL) arvore->raiz->cor = Preto;
    } else {
        NoRubro* novoNo = rb_adicionarNo(arvore, arvore->raiz, valor);
        rb_balancear_insercao(arvore, novoNo);
    }
}

NoRubro* rb_adicionarNo(ArvoreRubro* arvore, NoRubro* no, int valor) {
    contRubro++; // count operation
    contRubro++; // count comparison
    if (valor >= no->valor) {
        contRubro++; // count null check
        if (no->direita == arvore->nulo) {
            no->direita = rb_criarNo(arvore, no, valor);
            return no->direita;
        }
        return rb_adicionarNo(arvore, no->direita, valor);
    } else {
        contRubro++; // count null check
        if (no->esquerda == arvore->nulo) {
            no->esquerda = rb_criarNo(arvore, no, valor);
            return no->esquerda;
        }
        return rb_adicionarNo(arvore, no->esquerda, valor);
    }
}

void rb_balancear_insercao(ArvoreRubro* arvore, NoRubro* no) {
    while (no != arvore->raiz && no->pai->cor == Vermelho) {
        contRubro++; // count comparison
        NoRubro* pai = no->pai;
        NoRubro* avo = pai->pai;
        contRubro++; // count comparison
        if (pai == avo->esquerda) {
            NoRubro *tio = avo->direita;
            contRubro++; // count comparison
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                pai->cor = Preto;
                avo->cor = Vermelho;
                no = avo;
                contRubro++; // count operation
            } else {
                contRubro++; // count comparison
                if (no == pai->direita) {
                    no = pai;
                    rb_rotacionarEsquerda(arvore, no);
                }
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                rb_rotacionarDireita(arvore, no->pai->pai);
            }
        } else {
            NoRubro *tio = avo->esquerda;
            contRubro++; // count comparison
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                pai->cor = Preto;
                avo->cor = Vermelho;
                no = avo;
                contRubro++; // count operation
            } else {
                contRubro++; // count comparison
                if (no == pai->esquerda) {
                    no = pai;
                    rb_rotacionarDireita(arvore, no);
                }
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                rb_rotacionarEsquerda(arvore, no->pai->pai);
            }
        }
    }
    arvore->raiz->cor = Preto;
}

void rb_remover(ArvoreRubro* arvore, int valor) {
    contRubro++; // count operation
    NoRubro* noRemover = rb_localizar(arvore, valor);
    contRubro++; // count null check
    if (noRemover != arvore->nulo) {
        rb_removerNo(arvore, noRemover);
    }
}

void rb_removerNo(ArvoreRubro* arvore, NoRubro* noRemover) {
    NoRubro *y = noRemover, *x;
    Cor corOriginalY = y->cor;

    contRubro++; // count comparison
    if (noRemover->esquerda == arvore->nulo) {
        x = noRemover->direita;
        rb_transplantar(arvore, noRemover, noRemover->direita);
    } else if (noRemover->direita == arvore->nulo) {
        contRubro++; // count comparison
        x = noRemover->esquerda;
        rb_transplantar(arvore, noRemover, noRemover->esquerda);
    } else {
        y = rb_minimo(arvore, noRemover->direita);
        corOriginalY = y->cor;
        x = y->direita;
        contRubro++; // count comparison
        if (y->pai == noRemover) {
            x->pai = y;
        } else {
            rb_transplantar(arvore, y, y->direita);
            y->direita = noRemover->direita;
            y->direita->pai = y;
        }
        rb_transplantar(arvore, noRemover, y);
        y->esquerda = noRemover->esquerda;
        y->esquerda->pai = y;
        y->cor = noRemover->cor;
    }
    free(noRemover);
    contRubro++; // count operation
    contRubro++; // count comparison
    if (corOriginalY == Preto) {
        rb_balancear_remocao(arvore, x);
    }
}

void rb_balancear_remocao(ArvoreRubro* arvore, NoRubro* no) {
    while (no != arvore->raiz && no->cor == Preto) {
        contRubro++; // count comparison
        contRubro++; // count comparison
        if (no == no->pai->esquerda) {
            NoRubro* irmao = no->pai->direita;
            contRubro++; // count comparison
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rb_rotacionarEsquerda(arvore, no->pai);
                irmao = no->pai->direita;
            }
            contRubro++; // count comparison
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;
                contRubro++; // count operation
            } else {
                contRubro++; // count comparison
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rb_rotacionarDireita(arvore, irmao);
                    irmao = no->pai->direita;
                }
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rb_rotacionarEsquerda(arvore, no->pai);
                no = arvore->raiz;
            }
        } else {
            NoRubro* irmao = no->pai->esquerda;
            contRubro++; // count comparison
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rb_rotacionarDireita(arvore, no->pai);
                irmao = no->pai->esquerda;
            }
            contRubro++; // count comparison
            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;
                contRubro++; // count operation
            } else {
                contRubro++; // count comparison
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rb_rotacionarEsquerda(arvore, irmao);
                    irmao = no->pai->esquerda;
                }
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rb_rotacionarDireita(arvore, no->pai);
                no = arvore->raiz;
            }
        }
    }
    no->cor = Preto;
}

void rb_transplantar(ArvoreRubro* arvore, NoRubro* u, NoRubro* v) {
    contRubro++; // count comparison
    if (u->pai == arvore->nulo) arvore->raiz = v;
    else if (u == u->pai->esquerda) u->pai->esquerda = v;
    else u->pai->direita = v;
    v->pai = u->pai;
    contRubro++; // count operation
}

NoRubro* rb_minimo(ArvoreRubro* arvore, NoRubro* no) {
    while (no->esquerda != arvore->nulo) {
        contRubro++; // count comparison
        no = no->esquerda;
    }
    return no;
}

NoRubro* rb_localizar(ArvoreRubro* arvore, int valor) {
    NoRubro* no = arvore->raiz;
    while (no != arvore->nulo) {
        contRubro++; // count comparison
        if (no->valor == valor) return no;
        contRubro++; // count comparison
        no = valor < no->valor ? no->esquerda : no->direita;
    }
    return arvore->nulo;
}

void rb_rotacionarEsquerda(ArvoreRubro* arvore, NoRubro* no) {
    contRubro++; // count rotation operation
    NoRubro* direita = no->direita;
    no->direita = direita->esquerda;
    contRubro++; // count null check
    if (direita->esquerda != arvore->nulo) direita->esquerda->pai = no;
    direita->pai = no->pai;

    contRubro++; // count comparison
    if (no->pai == arvore->nulo) arvore->raiz = direita;
    else if (no == no->pai->esquerda) no->pai->esquerda = direita;
    else no->pai->direita = direita;

    direita->esquerda = no;
    no->pai = direita;
}

void rb_rotacionarDireita(ArvoreRubro* arvore, NoRubro* no) {
    contRubro++; // count rotation operation
    NoRubro* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    contRubro++; // count null check
    if (esquerda->direita != arvore->nulo) esquerda->direita->pai = no;
    esquerda->pai = no->pai;

    contRubro++; // count comparison
    if (no->pai == arvore->nulo) arvore->raiz = esquerda;
    else if (no == no->pai->esquerda) no->pai->esquerda = esquerda;
    else no->pai->direita = esquerda;

    esquerda->direita = no;
    no->pai = esquerda;
}

void rb_percorrer(ArvoreRubro* arvore, NoRubro* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        rb_percorrer(arvore, no->esquerda, callback);
        callback(no->valor);
        rb_percorrer(arvore, no->direita, callback);
    }
}

void rb_destruir_no(ArvoreRubro* arvore, NoRubro* no) {
    if (no == NULL || no == arvore->nulo) return;
    rb_destruir_no(arvore, no->esquerda);
    rb_destruir_no(arvore, no->direita);
    free(no);
}

void rb_destruir(ArvoreRubro* arvore) {
    if (arvore == NULL) return;
    rb_destruir_no(arvore, arvore->raiz);
    free(arvore->nulo);
    free(arvore);
}

/* ============================================================================= */
/* --- FUNÇÃO PRINCIPAL (DRIVER DO EXPERIMENTO) --- */
/* ============================================================================= */
int main() {
    const int MAX_N = 10000;
    const int PASSO = 1;
    const int NUM_AMOSTRAS = 10;
    const int MAX_VALOR_CHAVE = 100000;

    srand(time(NULL));

    FILE* f_adicao = fopen("resultados_adicao.csv", "w");
    FILE* f_remocao = fopen("resultados_remocao.csv", "w");

    if (f_adicao == NULL || f_remocao == NULL) {
        perror("Erro ao criar arquivos de resultado");
        return 1;
    }

    fprintf(f_adicao, "Tamanho,AVL,RubroNegra,B_Ordem_1,B_Ordem_5,B_Ordem_10\n");
    fprintf(f_remocao, "Tamanho,AVL,RubroNegra,B_Ordem_1,B_Ordem_5,B_Ordem_10\n");

    printf("Iniciando experimento de comparacao de arvores...\n");
    printf("Gerando resultados para N de %d a %d (passo de %d), com %d amostras cada.\n", PASSO, MAX_N, PASSO, NUM_AMOSTRAS);

    for (int N = 0; N <= MAX_N; N += PASSO) {
        long long soma_adicao_avl = 0, soma_remocao_avl = 0;
        long long soma_adicao_rb = 0, soma_remocao_rb = 0;
        long long soma_adicao_b1 = 0, soma_remocao_b1 = 0;
        long long soma_adicao_b5 = 0, soma_remocao_b5 = 0;
        long long soma_adicao_b10 = 0, soma_remocao_b10 = 0;

        for (int amostra = 0; amostra < NUM_AMOSTRAS; amostra++) {
            int* chaves = malloc(sizeof(int) * N);
            bool* numeros_usados = calloc(MAX_VALOR_CHAVE, sizeof(bool));
            if (chaves == NULL || numeros_usados == NULL) {
                perror("Erro ao alocar memoria para as chaves");
                free(chaves);
                free(numeros_usados);
                fclose(f_adicao);
                fclose(f_remocao);
                return 1; // added proper cleanup on allocation failure
            }

            for (int i = 0; i < N; i++) {
                int nova_chave;
                do {
                    nova_chave = rand() % MAX_VALOR_CHAVE;
                } while (numeros_usados[nova_chave]);
                chaves[i] = nova_chave;
                numeros_usados[nova_chave] = true;
            }
            free(numeros_usados);

            ArvoreAVL* avl = avl_criar();
            if (avl == NULL) {
                free(chaves);
                fclose(f_adicao);
                fclose(f_remocao);
                return 1; // added null check
            }
            contAVL = 0;
            for (int i = 0; i < N; i++) avl_adicionar(avl, chaves[i]);
            soma_adicao_avl += contAVL;

            embaralhar(chaves, N);
            contAVL = 0;
            for (int i = 0; i < N; i++) avl_remover(avl, chaves[i]);
            soma_remocao_avl += contAVL;
            avl_destruir(avl);

            ArvoreRubro* rb = rb_criar();
            if (rb == NULL) {
                free(chaves);
                fclose(f_adicao);
                fclose(f_remocao);
                return 1; // added null check
            }
            contRubro = 0;
            for (int i = 0; i < N; i++) rb_adicionar(rb, chaves[i]);
            soma_adicao_rb += contRubro;

            embaralhar(chaves, N);
            contRubro = 0;
            for (int i = 0; i < N; i++) rb_remover(rb, chaves[i]);
            soma_remocao_rb += contRubro;
            rb_destruir(rb);

            ArvoreB* b1 = btree_criar(1);
            if (b1 == NULL) {
                free(chaves);
                fclose(f_adicao);
                fclose(f_remocao);
                return 1; // added null check
            }
            contB = 0;
            for (int i = 0; i < N; i++) btree_adicionar(b1, chaves[i]);
            soma_adicao_b1 += contB;

            embaralhar(chaves, N);
            contB = 0;
            for (int i = 0; i < N; i++) btree_remover(b1, chaves[i]);
            soma_remocao_b1 += contB;
            btree_destruir(b1);

            ArvoreB* b5 = btree_criar(5);
            if (b5 == NULL) {
                free(chaves);
                fclose(f_adicao);
                fclose(f_remocao);
                return 1; // added null check
            }
            contB = 0;
            for (int i = 0; i < N; i++) btree_adicionar(b5, chaves[i]);
            soma_adicao_b5 += contB;

            embaralhar(chaves, N);
            contB = 0;
            for (int i = 0; i < N; i++) btree_remover(b5, chaves[i]);
            soma_remocao_b5 += contB;
            btree_destruir(b5);

            ArvoreB* b10 = btree_criar(10);
            if (b10 == NULL) {
                free(chaves);
                fclose(f_adicao);
                fclose(f_remocao);
                return 1; // added null check
            }
            contB = 0;
            for (int i = 0; i < N; i++) btree_adicionar(b10, chaves[i]);
            soma_adicao_b10 += contB;

            embaralhar(chaves, N);
            contB = 0;
            for (int i = 0; i < N; i++) btree_remover(b10, chaves[i]);
            soma_remocao_b10 += contB;
            btree_destruir(b10);

            free(chaves);
        }

        fprintf(f_adicao, "%d,%lld,%lld,%lld,%lld,%lld\n", N,
                soma_adicao_avl / NUM_AMOSTRAS, soma_adicao_rb / NUM_AMOSTRAS,
                soma_adicao_b1 / NUM_AMOSTRAS, soma_adicao_b5 / NUM_AMOSTRAS,
                soma_adicao_b10 / NUM_AMOSTRAS);
        fflush(f_adicao);

        fprintf(f_remocao, "%d,%lld,%lld,%lld,%lld,%lld\n", N,
                soma_remocao_avl / NUM_AMOSTRAS, soma_remocao_rb / NUM_AMOSTRAS,
                soma_remocao_b1 / NUM_AMOSTRAS, soma_remocao_b5 / NUM_AMOSTRAS,
                soma_remocao_b10 / NUM_AMOSTRAS);
        fflush(f_remocao);

        printf("Concluido para N = %d\n", N);
    }

    fclose(f_adicao);
    fclose(f_remocao);

    printf("\nExperimento finalizado com sucesso!\n");
    printf("Resultados salvos em 'resultados_adicao.csv' e 'resultados_remocao.csv'.\n");

    return 0;
}