#ifndef arvoreB_h
#define arvoreB_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "utilitarios.h"

#define M 2
#define MM (2 * M)

typedef struct pagina {
    int tamanho;
    Registro registros[MM];
    struct pagina* apontadores[MM + 1];
} Pagina;

void printDados(clock_t tempo, long long int, long long int);

void inicializaArvoreB(Pagina**);

bool pesquisaArvore(Registro*, Pagina*, Metrica*);

void insereRegistros(FILE*, Pagina**, unsigned int, Metrica*);

void insere(Registro, Pagina**, Metrica*);

void insereNaPagina(Pagina*, Registro, Pagina*, Metrica*);

void ins(Registro, Pagina*, bool*, Registro*, Pagina**, Metrica*);

void desalocaArvoreB(Pagina**);

void printArvoreB(Pagina*);

#endif