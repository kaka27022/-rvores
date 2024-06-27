#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "utilitarios.h"

Registro* alocarRegistros(int n)
{
    Registro* registro = (Registro*) malloc(n * sizeof(Registro));

    return registro;
}

void desalocarRegistros(Registro **registros)
{
    free(*registros);
}

bool lerEntrada(Entrada* entrada, int argc, char** argumentos)
{
    // Verifica se o numero de argumentos esta dentro do esperado
    if(argc < 5 || argc > 6)
        return false;

    entrada->metodo = atoi(argumentos[1]);
    entrada->quantidade_registros = atoi(argumentos[2]);
    entrada->situacao = atoi(argumentos[3]);
    entrada->chave_buscada = atoi(argumentos[4]);
    
    if(argc == 6)
    {
        strcpy(entrada->opcional, argumentos[5]);

        // Converte para LOWERCASE de modo a facilitar a comparacao futura
        for(int i = 0 ; entrada->opcional[i] != '\0' ; i++)
            entrada->opcional[i] = tolower(entrada->opcional[i]);

        // Se o argumento opcional for diferente de -P, entao retorna falso
        if(strcmp(entrada->opcional, "-p"))
            return false;
    }
    else
        entrada->opcional[0] = '\0';

    if(entrada->metodo < 1 || entrada->metodo > 4 || entrada->situacao < 1 || entrada->situacao > 3)
        return false;

    return true;
}

bool gerarChaves(int chaves[], Entrada *entrada, FILE* arq_bin)
{
    int i;

    // Arquivo esta ordenado ascendentemente
    if(entrada->situacao == 1)
    {
        // Garante que QTDE_CHAVES_TESTE - 4 chaves existam
        for(i = 0 ; i < QTDE_CHAVES_TESTE - 4 ; i++)
            chaves[i] = i + 1;

        // Garante que quatro chaves nao existam
        chaves[i] = 2000001;
        chaves[i + 1] = 2000002;
        chaves[i + 2] = 2000003;
        chaves[i + 3] = 2000004;
    }
    // Arquivo esta ordenado descendentemente
    else if(entrada->situacao == 2)
    {
        // Garante que QTDE_CHAVES_TESTE - 4 chaves existam
        for(i = 0 ; i < QTDE_CHAVES_TESTE - 4; i++)
            chaves[i] = 2000000 - i;

        // Garante que quatro chaves nao existam
        chaves[i] = -1;
        chaves[i + 1] = -2;
        chaves[i + 2] = -3;
        chaves[i + 3] = -4;
    }
    // Arquivo esta ordenado aleatoriamente
    else
    {
        Registro registros[QTDE_CHAVES_TESTE];

        if(fread(registros, sizeof(Registro), QTDE_CHAVES_TESTE - 4, arq_bin) == 0)
            return false;

        // Garante que QTDE_CHAVES_TESTE - 4 chaves existam
        for(i = 0 ; i < QTDE_CHAVES_TESTE - 4; i++)
            chaves[i] = registros[i].chave;

        // Garante que quatro chaves nao existam
        chaves[i] = 2000001;
        chaves[i + 1] = 2000002;
        chaves[i + 2] = 2000003;
        chaves[i + 3] = 2000004;

        rewind(arq_bin);
    }

    return true;
}

void exibirChavesGeradas(int chaves[])
{
    printf("CHAVES A SEREM PESQUISADAS\n");
    printf("[\n");

    for(int i = 0 ; i < QTDE_CHAVES_TESTE ; i++)
        printf("\t%d\n", chaves[i]);

    printf("]\n");
}

void resetarMetricas(Metrica *metricas)
{
    metricas->n_comparacoes_pesquisa = 0;
    metricas->n_comparacoes_pre_processamento = 0;
    metricas->n_leitura_pre_processamento = 0;
    metricas->n_leitura_pesquisa = 0;
    metricas->t_pre_processamento = 0;
    metricas->t_pesquisa = 0;
}

void exibirMetricas(Metrica *metricas)
{
    printf("\n--------- METRICAS ---------\n");
    // printf("TEMPO MEDIO DE PRE-PROCESSAMENTO: %.8fs\n", metricas->t_pre_processamento / QTDE_CHAVES_TESTE);
    // printf("TEMPO MEDIO DE PESQUISA: %.8fs\n", metricas->t_pesquisa / QTDE_CHAVES_TESTE);
    printf("TEMPO TOTAL DE PRE-PROCESSAMENTO: %.8fs\n", metricas->t_pre_processamento);
    printf("TEMPO TOTAL DE PESQUISA: %.8fs\n", metricas->t_pesquisa);
    printf("TEMPO TOTAL: %.8fs\n", metricas->t_pesquisa + metricas->t_pre_processamento);
    // printf("NUMERO MEDIO DE LEITURA EM PRE-PROCESSAMENTO: %.2f\n", ((double) metricas->n_leitura_pre_processamento) / QTDE_CHAVES_TESTE);
    // printf("NUMERO MEDIO DE LEITURA EM PESQUISA: %.2f\n", ((double) metricas->n_leitura_pesquisa) / QTDE_CHAVES_TESTE);
    printf("NUMERO TOTAL DE LEITURA EM PRE-PROCESSAMENTO: %lu\n", metricas->n_leitura_pre_processamento);
    printf("NUMERO TOTAL DE LEITURA EM PESQUISA: %lu\n", metricas->n_leitura_pesquisa);
    printf("NUMERO TOTAL DE LEITURAS: %lu\n", metricas->n_leitura_pesquisa + metricas->n_leitura_pre_processamento);
    // printf("NUMERO MEDIO DE COMPARACOES EM PRE-PROCESSAMENTO: %.2f\n", ((double) metricas->n_comparacoes_pre_processamento) / QTDE_CHAVES_TESTE);
    printf("NUMERO TOTAL DE COMPARACOES EM PRE-PROCESSAMENTO: %ld\n", metricas->n_comparacoes_pre_processamento);
    printf("NUMERO TOTAL DE COMPARACOES EM PESQUISA: %lu\n", metricas->n_comparacoes_pesquisa);
    printf("NUMERO TOTAL DE COMPARACOES: %lu\n", metricas->n_comparacoes_pesquisa + metricas->n_comparacoes_pre_processamento);
    // printf("NUMERO MEDIO DE COMPARACOES EM PESQUISA: %.2f\n", ((double) metricas->n_comparacoes_pesquisa) / QTDE_CHAVES_TESTE);
    printf("----------------------------\n");
}
