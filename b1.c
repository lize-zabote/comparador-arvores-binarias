#include <stdio.h>
#include <stdlib.h>

// Contador de esforço algorítmico
int contB = 0;

// --- ESTRUTURAS ---
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

// --- PROTÓTIPOS DAS FUNÇÕES ---
ArvoreB* criaArvore(int);
NoB* criaNo(ArvoreB*);
void percorreArvore(NoB*);
int pesquisaBinaria(NoB*, int);
NoB* localizaNo(ArvoreB*, int);

// Funções de Adição
void adicionaChave(ArvoreB*, int);
void adicionaChaveRecursivo(ArvoreB*, NoB*, NoB*, int);
void adicionaChaveNo(NoB*, NoB*, int);
NoB* divideNo(ArvoreB*, NoB*);
int transbordo(ArvoreB*, NoB*);

// Funções de Remoção
void removerChave(ArvoreB*, int);
void remover(ArvoreB*, NoB*, int);
void rebalancear(ArvoreB*, NoB*);
void fundirNos(ArvoreB*, NoB*, NoB*);
void redistribuirChaves(NoB*, NoB*);
NoB* getPredecessor(ArvoreB*, NoB*, int);


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);
    return a;
}

NoB* criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    NoB* noB = malloc(sizeof(NoB));

    noB->pai = NULL;
    noB->chaves = malloc(sizeof(int) * (max + 1));
    noB->filhos = malloc(sizeof(NoB*) * (max + 2));
    noB->total = 0;

    for (int i = 0; i < max + 2; i++)
        noB->filhos[i] = NULL;

    return noB;
}

void percorreArvore(NoB* noB) {
    if (noB != NULL) {
        for (int i = 0; i < noB->total; i++) {
            percorreArvore(noB->filhos[i]);
            printf("%d ", noB->chaves[i]);
        }
        percorreArvore(noB->filhos[noB->total]);
    }
}

int pesquisaBinaria(NoB* noB, int chave) {
    int inicio = 0, fim = noB->total - 1, meio;
    while (inicio <= fim) {
        contB++;
        meio = (inicio + fim) / 2;
        if (noB->chaves[meio] == chave) {
            return meio;
        } else if (noB->chaves[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return inicio;
}

NoB* localizaNo(ArvoreB* arvore, int chave) {
    NoB *noB = arvore->raiz;
    while (noB != NULL) {
        contB++;
        int i = pesquisaBinaria(noB, chave);
        if (noB->filhos[i] == NULL)
            return noB;
        else
            noB = noB->filhos[i];
    }
    return NULL;
}


// --- FUNÇÕES DE ADIÇÃO ---

void adicionaChave(ArvoreB* arvore, int chave) {
    NoB* noB = localizaNo(arvore, chave);
    adicionaChaveRecursivo(arvore, noB, NULL, chave);
}

void adicionaChaveRecursivo(ArvoreB* arvore, NoB* noB, NoB* novo, int chave) {
    contB++;
    adicionaChaveNo(noB, novo, chave);

    if (transbordo(arvore, noB)) {
        int promovido = noB->chaves[arvore->ordem];
        NoB* novoNo = divideNo(arvore, noB);
        if (noB->pai == NULL) {
            contB++;
            NoB* pai = criaNo(arvore);
            pai->filhos[0] = noB;
            adicionaChaveNo(pai, novoNo, promovido);
            noB->pai = pai;
            novoNo->pai = pai;
            arvore->raiz = pai;
        } else {
            adicionaChaveRecursivo(arvore, noB->pai, novoNo, promovido);
        }
    }
}

void adicionaChaveNo(NoB* noB, NoB* novo, int chave) {
    int i = pesquisaBinaria(noB, chave);
    contB++;
    for (int j = noB->total - 1; j >= i; j--) {
        noB->chaves[j + 1] = noB->chaves[j];
        noB->filhos[j + 2] = noB->filhos[j + 1];
    }
    noB->chaves[i] = chave;
    noB->filhos[i + 1] = novo;
    if (novo != NULL) novo->pai = noB;
    noB->total++;
}

NoB* divideNo(ArvoreB* arvore, NoB* noB) {
    int meio = noB->total / 2;
    NoB* novo = criaNo(arvore);
    novo->pai = noB->pai;
    contB++;
    for (int i = meio + 1; i < noB->total; i++) {
        novo->filhos[novo->total] = noB->filhos[i];
        novo->chaves[novo->total] = noB->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        novo->total++;
    }
    novo->filhos[novo->total] = noB->filhos[noB->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
    noB->total = meio;
    return novo;
}

int transbordo(ArvoreB* arvore, NoB* noB) {
    contB++;
    return noB->total > arvore->ordem * 2;
}

// --- FUNÇÕES DE REMOÇÃO ---

void removerChave(ArvoreB* arvore, int chave) {
    remover(arvore, arvore->raiz, chave);
}

void remover(ArvoreB* arvore, NoB* no, int chave) {
    contB++; // Conta a operação/chamada recursiva
    if (!no) {
        printf("Chave %d nao encontrada.\n", chave);
        return;
    }

    int pos = pesquisaBinaria(no, chave);
    int ehFolha = (no->filhos[0] == NULL);

    // Caso 1: A chave está no nó atual
    if (pos < no->total && no->chaves[pos] == chave) {
        if (ehFolha) { // Se for folha, remove diretamente
            contB++;
            for (int i = pos; i < no->total - 1; i++) {
                no->chaves[i] = no->chaves[i + 1];
            }
            no->total--;
            rebalancear(arvore, no);
        } else { // Se for nó interno
            // Substitui pela chave predecessora e remove recursivamente
            NoB* predecessor = getPredecessor(arvore, no, pos);
            int chavePredecessora = predecessor->chaves[predecessor->total - 1];
            no->chaves[pos] = chavePredecessora;
            remover(arvore, no->filhos[pos], chavePredecessora);
        }
    } else { // Caso 2: A chave não está no nó atual, desce para o filho
        if (ehFolha) {
            printf("Chave %d nao encontrada.\n", chave);
            return;
        }
        remover(arvore, no->filhos[pos], chave);
    }
}

void rebalancear(ArvoreB* arvore, NoB* no) {
    contB++; // Conta a verificação de rebalanceamento
    if (no == arvore->raiz) {
        if (no->total == 0) {
            arvore->raiz = no->filhos[0];
            if(arvore->raiz != NULL) arvore->raiz->pai = NULL;
            free(no->chaves);
            free(no->filhos);
            free(no);
        }
        return;
    }

    if (no->total >= arvore->ordem) {
        return; // Não precisa rebalancear
    }

    // Encontra a posição do nó no pai
    int posNoPai = 0;
    while (posNoPai <= no->pai->total && no->pai->filhos[posNoPai] != no) {
        posNoPai++;
    }

    // Tenta redistribuir com o irmão esquerdo
    if (posNoPai > 0 && no->pai->filhos[posNoPai - 1]->total > arvore->ordem) {
        redistribuirChaves(no->pai->filhos[posNoPai - 1], no);
    } 
    // Tenta redistribuir com o irmão direito
    else if (posNoPai < no->pai->total && no->pai->filhos[posNoPai + 1]->total > arvore->ordem) {
        redistribuirChaves(no, no->pai->filhos[posNoPai + 1]);
    } 
    // Se não puder redistribuir, funde os nós
    else {
        if (posNoPai > 0) {
            fundirNos(arvore, no->pai->filhos[posNoPai - 1], no);
        } else {
            fundirNos(arvore, no, no->pai->filhos[posNoPai + 1]);
        }
        // A fusão pode causar subfluxo no pai, então rebalanceia o pai
        rebalancear(arvore, no->pai);
    }
}


void fundirNos(ArvoreB* arvore, NoB* esq, NoB* dir) {
    contB++; // Conta a operação de fusão
    int posPai = 0;
    while (posPai < esq->pai->total && esq->pai->filhos[posPai] != esq) {
        posPai++;
    }

    // Puxa a chave do pai para o nó esquerdo
    esq->chaves[esq->total] = esq->pai->chaves[posPai];
    esq->total++;

    // Copia as chaves e filhos do nó direito para o esquerdo
    for (int i = 0; i < dir->total; i++) {
        contB++;
        esq->chaves[esq->total] = dir->chaves[i];
        esq->filhos[esq->total] = dir->filhos[i];
        if (esq->filhos[esq->total] != NULL) esq->filhos[esq->total]->pai = esq;
        esq->total++;
    }
    esq->filhos[esq->total] = dir->filhos[dir->total];
    if (esq->filhos[esq->total] != NULL) esq->filhos[esq->total]->pai = esq;

    // Remove a chave e o ponteiro do pai
    for (int i = posPai; i < esq->pai->total - 1; i++) {
        esq->pai->chaves[i] = esq->pai->chaves[i + 1];
        esq->pai->filhos[i + 1] = esq->pai->filhos[i + 2];
    }
    esq->pai->total--;

    // Libera o nó direito
    free(dir->chaves);
    free(dir->filhos);
    free(dir);
}


void redistribuirChaves(NoB* esq, NoB* dir) {
    contB++; // Conta a operação de redistribuição
    int posPai = 0;
    while(posPai < esq->pai->total && esq->pai->filhos[posPai] != esq) {
        posPai++;
    }
    
    // Caso 1: Nó direito precisa de chaves do esquerdo
    if(esq->total > dir->total){
        // Move a chave do pai para o início do nó direito
        for (int i = dir->total; i > 0; i--) {
            dir->chaves[i] = dir->chaves[i-1];
            dir->filhos[i+1] = dir->filhos[i];
        }
        dir->filhos[1] = dir->filhos[0];
        dir->total++;
        dir->chaves[0] = esq->pai->chaves[posPai];
        dir->filhos[0] = esq->filhos[esq->total];
        if(dir->filhos[0] != NULL) dir->filhos[0]->pai = dir;

        // Move a última chave do nó esquerdo para o pai
        esq->pai->chaves[posPai] = esq->chaves[esq->total-1];
        esq->total--;
    } 
    // Caso 2: Nó esquerdo precisa de chaves do direito
    else {
        // Move a chave do pai para o final do nó esquerdo
        esq->chaves[esq->total] = esq->pai->chaves[posPai];
        esq->total++;
        esq->filhos[esq->total] = dir->filhos[0];
        if(esq->filhos[esq->total] != NULL) esq->filhos[esq->total]->pai = esq;

        // Move a primeira chave do nó direito para o pai
        esq->pai->chaves[posPai] = dir->chaves[0];
        
        // Remove a chave e o filho movidos do nó direito
        for(int i=0; i < dir->total-1; i++){
            dir->chaves[i] = dir->chaves[i+1];
            dir->filhos[i] = dir->filhos[i+1];
        }
        dir->filhos[dir->total-1] = dir->filhos[dir->total];
        dir->total--;
    }
}


NoB* getPredecessor(ArvoreB* arvore, NoB* no, int pos) {
    contB++; // Conta a descida para encontrar o predecessor
    NoB* atual = no->filhos[pos];
    while (atual->filhos[atual->total] != NULL) {
        contB++;
        atual = atual->filhos[atual->total];
    }
    return atual;
}


int main() {
    // Ordem da árvore (ordem t => mínimo t chaves, máximo 2t chaves)
    // Aqui, ordem 1 significa que cada nó tem no mínimo 1 e no máximo 2 chaves.
    ArvoreB* arvore = criaArvore(1);

    printf("--- INSERINDO ELEMENTOS ---\n");
    adicionaChave(arvore, 10);
    adicionaChave(arvore, 20);
    adicionaChave(arvore, 30);
    adicionaChave(arvore, 5);
    adicionaChave(arvore, 15);
    adicionaChave(arvore, 25);
    adicionaChave(arvore, 35);
    adicionaChave(arvore, 1);

    printf("Arvore inicial: ");
    percorreArvore(arvore->raiz);
    printf("\nNumero de operacoes na insercao: %d\n\n", contB);
    
    printf("--- REMOVENDO ELEMENTOS ---\n");
    contB = 0;
    
    printf("Removendo chave 15 (causa redistribuicao)...\n");
    removerChave(arvore, 15);
    printf("Arvore apos remocao: ");
    percorreArvore(arvore->raiz);
    printf("\nNumero de operacoes: %d\n\n", contB);

    contB = 0;
    printf("Removendo chave 20 (causa fusao)...\n");
    removerChave(arvore, 20);
    printf("Arvore apos remocao: ");
    percorreArvore(arvore->raiz);
    printf("\nNumero de operacoes: %d\n\n", contB);

    contB = 0;
    printf("Removendo chave 10 (raiz)...\n");
    removerChave(arvore, 10);
    printf("Arvore apos remocao: ");
    percorreArvore(arvore->raiz);
    printf("\nNumero de operacoes: %d\n\n", contB);
    
    // Idealmente, haveria uma função para liberar toda a memória da árvore.
    return 0;
}