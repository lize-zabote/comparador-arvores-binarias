#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int contAVL = 0;

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    int valor;
    int altura;
} NoAVL;

typedef struct arvoreAVL {
    struct no* raiz;
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
    NoAVL* no = arvoreAVL->raiz;

    contAVL++;

    while (no != NULL) {
        if (valor > no->valor) {
            if (no->direita != NULL) {
                no = no->direita;
            } else {
                break;
            }
        } else {
            if (no->esquerda != NULL) {
                no = no->esquerda;
            } else {
                break;
            }
        }
    }

    NoAVL* novo = malloc(sizeof(NoAVL));
    novo->valor = valor;
    novo->pai = no;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1;

    if (no == NULL) {    
        arvoreAVL->raiz = novo;
    } else {
        if (valor > no->valor) {
            no->direita = novo;
        } else {
            no->esquerda = novo;
        }
        
        balanceamento(arvoreAVL, no);
    }
}

NoAVL* localizar(NoAVL* no, int valor) {
    while (no != NULL) {
        if (no->valor == valor) {
            return no;
        }
        
        no = valor < no->valor ? no->esquerda : no->direita;
    }

    return NULL;
}

void percorrer(NoAVL* no, void (*callback)(int)) {
    if (no != NULL) {
        percorrer(no->esquerda,callback);
        callback(no->valor);
        percorrer(no->direita,callback);
    }
}

void percorrerProfundidadeInOrder(ArvoreAVL* arvoreAVL, NoAVL* no, void (*callback)(int)) {
    if (no != NULL) {
        
        
        percorrerProfundidadeInOrder(arvoreAVL, no->esquerda,callback);
        callback(no->valor);
        percorrerProfundidadeInOrder(arvoreAVL, no->direita,callback);
    }
}

void visitar(int valor){
    printf("%d ", valor);
}

void balanceamento(ArvoreAVL* arvoreAVL, NoAVL* no) {
    while (no != NULL) {
        no->altura = maxx(altura(no->esquerda), altura(no->direita)) + 1;
        int fator = fb(no);

        if (fator > 1) { //árvore mais pesada para esquerda
            //rotação para a direita
            if (fb(no->esquerda) > 0) {
                rsd(arvoreAVL, no); //rotação simples a direita, pois o FB do filho tem sinal igual
            } else {
                rdd(arvoreAVL, no); //rotação dupla a direita, pois o FB do filho tem sinal diferente
            }
        } else if (fator < -1) { //árvore mais pesada para a direita
            //rotação para a esquerda
            if (fb(no->direita) < 0) {
                rse(arvoreAVL, no); //rotação simples a esquerda, pois o FB do filho tem sinal igual
            } else {
                rde(arvoreAVL, no); //rotação dupla a esquerda, pois o FB do filho tem sinal diferente
            }
        }

        no = no->pai; 
    }
}

int altura(NoAVL* no){
    return no != NULL ? no->altura : 0;
}

int fb(NoAVL* no) {
    int esquerda = 0,direita = 0;
  
    if (no->esquerda != NULL) {
        esquerda = no->esquerda->altura;
    }

    if (no->direita != NULL) {
        direita = no->direita->altura;
    }
  
    return esquerda - direita;
}

NoAVL* rse(ArvoreAVL* arvoreAVL, NoAVL* no) {
    NoAVL* pai = no->pai;
    NoAVL* direita = no->direita;

    if (direita->esquerda != NULL) {
        direita->esquerda->pai = no;
    } 
  
    no->direita = direita->esquerda;
    no->pai = direita;

    direita->esquerda = no;
    direita->pai = pai;

    if (pai == NULL) {
        arvoreAVL->raiz = direita;
    } else {
        if (pai->esquerda == no) {
            pai->esquerda = direita;
        } else {
            pai->direita = direita;
        }
    }

    no->altura = maxx(altura(no->esquerda), altura(no->direita)) + 1;
    direita->altura = maxx(altura(direita->esquerda), altura(direita->direita)) + 1;

    return direita;
}

NoAVL* rsd(ArvoreAVL* arvoreAVL, NoAVL* no) {
    NoAVL* pai = no->pai;
    NoAVL* esquerda = no->esquerda;

    if (esquerda->direita != NULL) {
        esquerda->direita->pai = no;
    } 
  
    no->esquerda = esquerda->direita;
    no->pai = esquerda;
  
    esquerda->direita = no;
    esquerda->pai = pai;

    if (pai == NULL) {
        arvoreAVL->raiz = esquerda;
    } else {
        if (pai->esquerda == no) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }

    no->altura = maxx(altura(no->esquerda), altura(no->direita)) + 1;
    esquerda->altura = maxx(altura(esquerda->esquerda), altura(esquerda->direita)) + 1;

    return esquerda;
}

NoAVL* rde(ArvoreAVL* arvoreAVL, NoAVL* no) {
    no->direita = rsd(arvoreAVL, no->direita);
    return rse(arvoreAVL, no);
}

NoAVL* rdd(ArvoreAVL* arvoreAVL, NoAVL* no) {
    no->esquerda = rse(arvoreAVL, no->esquerda);
    return rsd(arvoreAVL, no);
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


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai; 
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
    NoB* no = malloc(sizeof(NoB));

    no->pai = NULL;
        
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB) * (max + 2));
    no->total = 0;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

void percorreArvore(NoB* no) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++){
            percorreArvore(no->filhos[i]); //visita o filho a esquerda
            
            printf("%d ",no->chaves[i]);
        }

        percorreArvore(no->filhos[no->total]); //visita ultimo filho (direita)
    }
}

int pesquisaBinaria(NoB* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;		
    
    while (inicio <= fim) {	
        contB++;
        
        meio = (inicio + fim) / 2;
        
        if (no->chaves[meio] == chave) {	
		    return meio; //encontrou	
        } else if (no->chaves[meio] > chave) {
                fim	= meio - 1;	
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; //não encontrou	
}

int localizaChave(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return 1; //encontrou
        } else {
            no = no->filhos[i];
        }
    }

    return 0; //não encontrou	
}

NoB* localizaNo(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        contB++;

        int i = pesquisaBinaria(no, chave);

        if (no->filhos[i] == NULL)
            return no; //encontrou nó
        else
            no = no->filhos[i];
    }

    return NULL; //não encontrou nenhum nó
}

void adicionaChaveNo(NoB* no, NoB* novo, int chave) {
    int i = pesquisaBinaria(no, chave);
    
    contB++;

    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    
    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;

    no->total++;
}

int transbordo(ArvoreB* arvore, NoB* no) {
    contB++;
    
    return no->total > arvore->ordem * 2;
}

NoB* divideNo(ArvoreB* arvore, NoB* no) {
    int meio = no->total / 2;
    NoB* novo = criaNo(arvore);
    novo->pai = no->pai;

    contB++;
    
    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        
        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;    
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, NoB* no, NoB* novo, int chave) {
    contB++;
    
    adicionaChaveNo(no, novo, chave);
    
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem]; 
        NoB* novo = divideNo(arvore, no);

        if (no->pai == NULL) {
            contB++;
            
            NoB* pai = criaNo(arvore);            
            pai->filhos[0] = no;
            adicionaChaveNo(pai, novo, promovido);
            
            no->pai = pai;
            novo->pai = pai;
            arvore->raiz = pai;
        } else
            adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    NoB* no = localizaNo(arvore, chave);

    adicionaChaveRecursivo(arvore, no, NULL, chave);
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

