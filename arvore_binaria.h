#ifndef BINARIAP_H
#define BINARIAP_H

#define MAX_TAM_CHAVE 1000

#include "utilitarios.h"

// typedef struct {
    
//     int indice; // valor que identifica a pagina

// } Pagina;

// // =======================================  [LANÇAR NO UTILITARIOS_H DPS]
// typedef struct {
    
//     int chave;
//     long int valor;
//     char string[TAM_TITULO];

// } Registro;

// typedef struct {

//     Registro item;
//     long left, right;
    
// } Item;
// =======================================

void caminhamento(FILE *arqDestino);
bool findChave(FILE*, long int, int, int*, int*);
void montarArvore(FILE*, FILE*, int, int*, int*);
void inserirItem(FILE*, No, long int, short, int*, int*);
//void arvoreBinaria(FILE*,FILE*,int, int);
//void resultados(double, double, int, int, int, int);

void inserirAscendente(FILE* arqDestino, Registro registro);
void inserirDescendente(FILE* arqDestino, Registro registro);

#endif