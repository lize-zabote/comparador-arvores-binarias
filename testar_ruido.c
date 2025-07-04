#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

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
NoB* btree_criaNo(ArvoreB* arvore);
int btree_pesquisaBinaria(NoB* no, int chave);
NoB* btree_localizaNo(ArvoreB* arvore, int chave);
void btree_adicionar_recursivo(ArvoreB* arvore, NoB* no, NoB* novo, int chave);
void btree_adicionaChaveNo(NoB* no, NoB* novo, int chave);
NoB* btree_divideNo(ArvoreB* arvore, NoB* no);
int btree_transbordo(ArvoreB* arvore, NoB* no);
void btree_destruir_no(NoB* no);

ArvoreB* btree_criar(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    if (a == NULL) return NULL;
    a->ordem = ordem;
    a->raiz = btree_criaNo(a);
    return a;
}

NoB* btree_criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    NoB* no = malloc(sizeof(NoB));
    if (no == NULL) return NULL;
    no->pai = NULL;
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB*) * (max + 2));
    if (no->chaves == NULL || no->filhos == NULL) {
        free(no->chaves);
        free(no->filhos);
        free(no);
        return NULL;
    }
    no->total = 0;
    for (int i = 0; i < max + 2; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

void btree_adicionar(ArvoreB* arvore, int chave) {
    NoB* no = btree_localizaNo(arvore, chave);
    btree_adicionar_recursivo(arvore, no, NULL, chave);
}

int btree_pesquisaBinaria(NoB* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
        meio = inicio + (fim - inicio) / 2;
        if (no->chaves[meio] == chave) return meio;
        if (no->chaves[meio] > chave) fim = meio - 1;
        else inicio = meio + 1;
    }
    return inicio;
}

NoB* btree_localizaNo(ArvoreB* arvore, int chave) {
    NoB* no = arvore->raiz;
    while (no != NULL) {
        int i = btree_pesquisaBinaria(no, chave);
        if (no->filhos[i] == NULL) return no;
        no = no->filhos[i];
    }
    return NULL;
}

void btree_adicionar_recursivo(ArvoreB* arvore, NoB* no, NoB* novo, int chave) {
    btree_adicionaChaveNo(no, novo, chave);

    if (btree_transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        NoB* novoNo = btree_divideNo(arvore, no);
        contB++; // Contar só a divisão

        if (no->pai == NULL) {
            NoB* pai = btree_criaNo(arvore);
            pai->filhos[0] = no;
            btree_adicionaChaveNo(pai, novoNo, promovido);
            no->pai = pai;
            novoNo->pai = pai;
            arvore->raiz = pai;
        } else {
            btree_adicionar_recursivo(arvore, no->pai, novoNo, promovido);
        }
    }
}

void btree_adicionaChaveNo(NoB* no, NoB* novo, int chave) {
    int i = btree_pesquisaBinaria(no, chave);
    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;
    if (novo != NULL) novo->pai = no;
    no->total++;
}

NoB* btree_divideNo(ArvoreB* arvore, NoB* no) {
    int meio = no->total / 2;
    NoB* novo = btree_criaNo(arvore);
    novo->pai = no->pai;

    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        if (novo->filhos[novo->total] != NULL) {
            novo->filhos[novo->total]->pai = novo;
        }
        novo->chaves[novo->total] = no->chaves[i];
        novo->total++;
    }
    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;

    no->total = meio;
    return novo;
}

int btree_transbordo(ArvoreB* arvore, NoB* no) {
    return no->total > arvore->ordem * 2;
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
/* --- FUNÇÃO PRINCIPAL (TESTE DE RUÍDO) --- */
/* ============================================================================= */

int main() {
    const int N = 3000; // Tamanho fixo para teste
    const int NUM_AMOSTRAS = 100; // Aumentado para 100 amostras
    const int MAX_VALOR_CHAVE = 100000;
    srand(time(NULL));

    FILE* f_ruido = fopen("resultados_ruido.csv", "w");
    if (f_ruido == NULL) {
        perror("Erro ao criar arquivo de resultado");
        return 1;
    }

    fprintf(f_ruido, "Amostra,B_Ordem_1,B_Ordem_5,B_Ordem_10\n");

    for (int amostra = 0; amostra < NUM_AMOSTRAS; amostra++) {
        int* chaves = malloc(sizeof(int) * N);
        bool* numeros_usados = calloc(MAX_VALOR_CHAVE, sizeof(bool));
        if (chaves == NULL || numeros_usados == NULL) {
            perror("Erro ao alocar memoria");
            free(chaves);
            free(numeros_usados);
            fclose(f_ruido);
            return 1;
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

        // Teste B_Ordem_1
        ArvoreB* b1 = btree_criar(1);
        if (b1 == NULL) {
            free(chaves);
            fclose(f_ruido);
            return 1;
        }
        contB = 0;
        for (int i = 0; i < N; i++) btree_adicionar(b1, chaves[i]);
        long long cont_b1 = contB;
        btree_destruir(b1);

        // Teste B_Ordem_5
        ArvoreB* b5 = btree_criar(5);
        if (b5 == NULL) {
            free(chaves);
            fclose(f_ruido);
            return 1;
        }
        contB = 0;
        for (int i = 0; i < N; i++) btree_adicionar(b5, chaves[i]);
        long long cont_b5 = contB;
        btree_destruir(b5);

        // Teste B_Ordem_10
        ArvoreB* b10 = btree_criar(10);
        if (b10 == NULL) {
            free(chaves);
            fclose(f_ruido);
            return 1;
        }
        contB = 0;
        for (int i = 0; i < N; i++) btree_adicionar(b10, chaves[i]);
        long long cont_b10 = contB;
        btree_destruir(b10);

        fprintf(f_ruido, "%d,%lld,%lld,%lld\n", amostra, cont_b1, cont_b5, cont_b10);

        free(chaves);
    }

    fclose(f_ruido);
    printf("Teste de ruido concluido! Resultados salvos em 'resultados_ruido.csv'.\n");

    return 0;
}