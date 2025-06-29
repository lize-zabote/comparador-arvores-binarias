#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

ArvoreB* criaArvore(int);
NoB* criaNo(ArvoreB*);
void percorreArvore(NoB*);
int pesquisaBinaria(NoB*, int);
int localizaChave(ArvoreB*, int);
NoB* localizaNo(ArvoreB*, int);
void adicionaChaveNo(NoB*, NoB*, int);
int transbordo(ArvoreB*,NoB*);
NoB* divideNo(ArvoreB*, NoB*);
void adicionaChaveRecursivo(ArvoreB*, NoB*, NoB*, int);
void adicionaChave(ArvoreB*, int);

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);

    return a;
}

int contB = 0;

NoB* criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    NoB* noB = malloc(sizeof(NoB));

    noB->pai = NULL;
        
    noB->chaves = malloc(sizeof(int) * (max + 1));
    noB->filhos = malloc(sizeof(NoB) * (max + 2));
    noB->total = 0;

    for (int i = 0; i < max + 2; i++)
        noB->filhos[i] = NULL;

    return noB;
}

void percorreArvore(NoB* noB) {
    if (noB != NULL) {
        for (int i = 0; i < noB->total; i++){
            percorreArvore(noB->filhos[i]); //visita o filho a esquerda
            
            printf("%d ",noB->chaves[i]);
        }

        percorreArvore(noB->filhos[noB->total]); //visita ultimo filho (direita)
    }
}

int pesquisaBinaria(NoB* noB, int chave) {
    int inicio = 0, fim = noB->total - 1, meio;		
    
    while (inicio <= fim) {	
        contB++;
        
        meio = (inicio + fim) / 2;
        
        if (noB->chaves[meio] == chave) {	
		    return meio; //encontrou	
        } else if (noB->chaves[meio] > chave) {
                fim	= meio - 1;	
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; //não encontrou	
}

int localizaChave(ArvoreB* arvore, int chave) {	
    NoB *noB = arvore->raiz;
    
    while (noB != NULL) {
        int i = pesquisaBinaria(noB, chave);

        if (i < noB->total && noB->chaves[i] == chave) {
            return 1; //encontrou
        } else {
            noB = noB->filhos[i];
        }
    }

    return 0; //não encontrou	
}

NoB* localizaNo(ArvoreB* arvore, int chave) {	
    NoB *noB = arvore->raiz;
    
    while (noB != NULL) {
        contB++;

        int i = pesquisaBinaria(noB, chave);

        if (noB->filhos[i] == NULL)
            return noB; //encontrou nó
        else
            noB = noB->filhos[i];
    }

    return NULL; //não encontrou nenhum nó
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

    noB->total++;
}

int transbordo(ArvoreB* arvore, NoB* noB) {
    contB++;
    
    return noB->total > arvore->ordem * 2;
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

void adicionaChaveRecursivo(ArvoreB* arvore, NoB* noB, NoB* novo, int chave) {
    contB++;
    
    adicionaChaveNo(noB, novo, chave);
    
    if (transbordo(arvore, noB)) {
        int promovido = noB->chaves[arvore->ordem]; 
        NoB* novo = divideNo(arvore, noB);

        if (noB->pai == NULL) {
            contB++;
            
            NoB* pai = criaNo(arvore);            
            pai->filhos[0] = noB;
            adicionaChaveNo(pai, novo, promovido);
            
            noB->pai = pai;
            novo->pai = pai;
            arvore->raiz = pai;
        } else
            adicionaChaveRecursivo(arvore, noB->pai, novo, promovido);
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    NoB* noB = localizaNo(arvore, chave);

    adicionaChaveRecursivo(arvore, noB, NULL, chave);
}

int main() {
    // ArvoreB* arvore = criaArvore(1);
    
    // contB = 0;

    // adicionaChave(arvore, 12);
    // adicionaChave(arvore, 3);
    // adicionaChave(arvore, 5);
    // adicionaChave(arvore, 7);
    // adicionaChave(arvore, 15);
    // adicionaChave(arvore, 99);
    // adicionaChave(arvore, 1);

    // percorreArvore(arvore->raiz);

    // printf("\nNúmero de operações: %d\n", contB);

    ArvoreB* a = criaArvore(1);
    ArvoreB* b = criaArvore(5);
    ArvoreB* c = criaArvore(10);

    //Ordem 1 = qtdDeFolhas: 1+2 = 3
    //Ordem 5 = qtdDeFolhas: 1+2+4+8+16+32 = 63
    //Ordem 10 = qtdDeFolhas: 1+2+4+8+16+32+64+128+256+512+1024 = 2047
    int qtdDeFolhas[3] = {3,63,2047};
    ArvoreB* arvores[3] = {a,b,c};

    srand(time(NULL));

    int media[3] = {0,0,0};

    for (int x = 0; x < 3; x++) //interação em cada ordem
    {
        for (int v = 0; v < 10; v++) //interação para definir as médias por ordem
        {
            contB = 0;
            for (int i = 0; i < qtdDeFolhas[x]; i++)
            {
                adicionaChave(arvores[x],(rand() % 10000) + 1);
            }
            printf("In-order: ");
            percorreArvore(arvores[x]->raiz);
            printf("\nNúmero de operações: %d\n", contB);
            media[x] += contB;
            printf("\n");
        }
        
        
    }
    printf("\nMédia de operações: %d\n", (media[0] / 10));
    printf("\nMédia de operações: %d\n", (media[1] / 10));
    printf("\nMédia de operações: %d\n", (media[2] / 10));

}
