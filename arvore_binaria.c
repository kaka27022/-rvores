#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "arvore_binaria.h"

void caminhamento(FILE *arqDestino) {
    No itemTeste;

    rewind(arqDestino);

    while (fread(&itemTeste, sizeof(No), 1, arqDestino) == 1) {
        printf("%d\n", itemTeste.registro.chave);
    }

    return;
}

// [LÊ O REGISTRO E MANDA INSERIR O ITEM]
void montarArvore(FILE *arqBase, FILE *arqDestino, int numDeRegistros, int* comparacoes, int* transferencia) {
    No newItem;
    Registro raiz;
    Registro newRegistro;

    //long int deslocamentoReferencia = 0;

    // [INSERÇÃO DA RAIZ]
    fread(&raiz, sizeof(Registro), 1, arqBase);
    newItem.registro = raiz;
    newItem.esq = newItem.dir = -1;
    inserirItem(arqDestino, newItem, 0,  1, comparacoes, transferencia);

    //  [INSERÇÃO DOS DEMAIS ITENS] // -1 pq ja leu a raiz -- 
    for (int i = 1; i < numDeRegistros; i++) {
        fread(&newRegistro, sizeof(Registro), 1, arqBase);
        newItem.registro = newRegistro;
        newItem.esq = newItem.dir = -1;
        inserirItem(arqDestino, newItem, 0, 0, comparacoes, transferencia);
    }
}


// arquivo p inserir -> item a ser inserido -> posicao do itemReferencia (itemAtual) -> bool: raiz ou nao
void inserirItem(FILE *arqDestino, No newItem, long int deslocamentoReferencia, short flagRaiz, int *comparacoes, int* transferencias) {
    if (flagRaiz ==  1) {
        // apenas vai para o final do arquivo e grava a raiz
        *comparacoes = *comparacoes + 1;
        fseek(arqDestino, 0, SEEK_SET);
        fwrite(&newItem, sizeof(No), 1, arqDestino);
        (*transferencias)++;
        return; // p/  interromper a continuidade
    } 

    No itemReferencia; 
    
    // [LEITURA DO ITEM NA POSIÇÃO "deslocamentoReferencia"] -- 
    fseek(arqDestino, deslocamentoReferencia, SEEK_SET);
    fread(&itemReferencia, sizeof(No), 1, arqDestino);
    fseek(arqDestino, 0, SEEK_END);
    
    //  [caminhamento para achar o nó que será pai]
    if (newItem.registro.chave < itemReferencia.registro.chave) { // insercao aa esquerda
        // se nao tiver filho aa esquerda, insere
        *comparacoes = *comparacoes + 1;
        if (itemReferencia.esq == -1) {
            *comparacoes = *comparacoes + 1;
            itemReferencia.esq = ftell(arqDestino);
            fwrite(&newItem, sizeof(No), 1, arqDestino);
            (*transferencias)++;
            
            // [REESCREVE O ITEM PAI C/ FILHOS ATUALIZADOS, E VOLTA PARA O FINAL DO ARQUIVO]  --
            fseek(arqDestino, deslocamentoReferencia, SEEK_SET);
            fwrite(&itemReferencia, sizeof(No), 1, arqDestino);
            fseek(arqDestino, 0, SEEK_END);

            return; 
        }
        // se tiver filho aa esquerda, caminha denovo
        inserirItem(arqDestino, newItem, itemReferencia.esq, 0, comparacoes, transferencias);

    } else {   //                                insercao aa direita

        // se nao tiver filho aa direita, insere
        *comparacoes = *comparacoes + 1;
        if (itemReferencia.dir == -1) {
            *comparacoes = *comparacoes + 1;
            itemReferencia.dir = ftell(arqDestino); // recebe o final do arquivo, onde o item vai ser inserido
            fwrite(&newItem, sizeof(No), 1, arqDestino);
            (*transferencias)++;       

            // [REESCREVE O ITEM PAI C/ FILHOS ATUALIZADOS, E VOLTA PARA O FINAL DO ARQUIVO]  --
            fseek(arqDestino, deslocamentoReferencia, SEEK_SET);
            fwrite(&itemReferencia, sizeof(No), 1, arqDestino);
            fseek(arqDestino, 0, SEEK_END);       

            return;
        } 
        // se tiver filho aa direita, caminha denovo
        inserirItem(arqDestino, newItem, itemReferencia.dir, 0, comparacoes, transferencias);
    } 
} 

bool findChave(FILE* arqDestino, long int root_offset, int chaveAlvo, int* comparacoes, int* trasnferencias) {
    // [ATINGIU UM NÓ FOLHA]
    if(root_offset == -1){ 
        *comparacoes = *comparacoes + 1;
        return false;
    }
        
    No itemAtual;

    fseek(arqDestino, root_offset, SEEK_SET);
    fread(&itemAtual, sizeof(No), 1, arqDestino);

    (*trasnferencias)++;

    if(chaveAlvo == itemAtual.registro.chave){
        *comparacoes = *comparacoes + 1;
        return true;
    } else if (chaveAlvo < itemAtual.registro.chave){
        *comparacoes = *comparacoes + 1;
        return findChave(arqDestino, itemAtual.esq, chaveAlvo, comparacoes, trasnferencias);
    } else {
        *comparacoes = *comparacoes + 1;
        return findChave(arqDestino, itemAtual.dir, chaveAlvo, comparacoes, trasnferencias);
    }
}


// [INSERÇÃO: ASCENDENTE & DESCENDENTE] --- N necessarias
void inserirDescendente(FILE* arqDestino, Registro registro) {
    No newItem;

    newItem.dir = -1;
    newItem.registro = registro;

    //  move para o final do arquivo e salva a posição do novo item
    fseek(arqDestino, 0, SEEK_END);
    fwrite(&newItem, sizeof(No), 1, arqDestino);

    newItem.esq = ftell(arqDestino);
}

void inserirAscendente(FILE *arqDestino, Registro registro) {
    No newItem;

    newItem.esq = -1;
    newItem.registro = registro;
    
    //  move para o final do arquivo e salva a posição
    fseek(arqDestino, 0, SEEK_END);
    fwrite(&newItem, sizeof(No), 1, arqDestino);

    newItem.dir = ftell(arqDestino);

}
