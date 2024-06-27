#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "implementacao/arvoreB.h"
#include "implementacao/arvoreBs.h"
#include "implementacao/sequencial.h"
#include "implementacao/utilitarios.h"
#include "implementacao/arvore_binaria.h"

int main(int argc, char* argv[])
{
    short int retorno_funcao;
    int chaves_geradas[QTDE_CHAVES_TESTE];
    FILE *arq_bin, *arq_arv_bin;
    Metrica metricas;
    Registro reg_retorno;
    Entrada entrada;

    // ARVORE BINARIA VARIAVEIS ---------------
    //Variaveis de Tempo
    //double tempoInserir;
    double tempoPesquisa;
    //double contadorPesquisa = 0;

    //Variaveis de Comparacoes
    int compInserir = 0;
    int compPesquisa = 0;
    //int contadorComp = 0;

    //Variaveis de Transferencias
    int transfInserir = 0;
    int transfPesquisa = 0;
    //int contadorTrans = 0;

    bool valueRetorno;
    // ------------------------------------------

    // Le entrada enquanto verifica se eh valida. Retorna "true" se for valida e "false" caso contrario.
    if(lerEntrada(&entrada, argc, argv) == false)
    {
        printf("O modo de executar o programa esta errado ou algum argumento inserido nao e valido, execute de acordo:\n\n");
        printf("%s <metodo> <quantidade> <situacao> <chave> [-P|-p]\n", argv[0]);

        exit(1);
    }

    /*
        Se o arquivo "registros.bin" nao existir no diretorio corrente,
        termina a execucao e para.
    */
    if((arq_bin = fopen("registros.bin", "rb")) == NULL)
    {
        printf("\nNao foi possivel abrir o arquivo binario \"registros.bin\". Abortando o programa...\n");
        return 0;
    }

    // Gera as chaves aleatórias que irão ser utilizadas como teste
    if(gerarChaves(chaves_geradas, &entrada, arq_bin) == false)
    {
        printf("Nao foi possivel gerar as chaves aleatorias! Abortando o programa...\n");
        exit(1);
    }

    resetarMetricas(&metricas);
    switch(entrada.metodo)
    {
        // Acesso Sequencial Indexado
        case 1:
            // Se o arquivo passado esta desordenado, nao eh possivel utilizar este metodo.
            if(entrada.situacao == 3)
                printf("O metodo escolhido nao funciona se o arquivo estiver desordenado! Abortando o programa...\n");
            else
            {
                Tabela tabela;

                if(preencheTabela(arq_bin, &tabela, &entrada, &metricas) == -1)
                {
                    printf("Nao foi possivel alocar dinamicamente um vetor em memoria principal. Abortando o programa...\n");
                        exit(1);
                }

                // if(argc == 6)
                //     printSequencial(arq_bin, &tabela, &entrada);

                // for(int i = 0 ; i < QTDE_CHAVES_TESTE ; i++)
                // {
                // entrada.chave_buscada = chaves_geradas[i];
                retorno_funcao = sequencial(arq_bin, &tabela, &entrada, &reg_retorno, &metricas);
                if(retorno_funcao == 1)
                    printf("A chave %d foi encontrada no arquivo!\n", reg_retorno.chave);
                else
                    printf("A chave passada como argumento (%d) não existe no arquivo!\n", entrada.chave_buscada);
                // }

                exibirMetricas(&metricas);
            }
        break;

        // Arvore Binaria de Pesquisa
        case 2:
            // Caso nao consiga criar o novo arquivo binario de estrutura arvore binaria
            if((arq_arv_bin = fopen("registros_arvore.bin", "w+b")) == NULL)
                printf("Nao foi possivel abrir/gerar o arquivo binario de arvore. Abortando o programa...\n");
            else
            {
                rewind(arq_bin); rewind(arq_arv_bin);

                // CONTAGEM DE TEMPO INSERCAO: INICIO
                clock_t startI = clock();

                montarArvore(arq_bin, arq_arv_bin, entrada.quantidade_registros, &compInserir, &transfInserir);

                if(argc == 6)
                    caminhamento(arq_arv_bin);

                // CONTAGEM DE TEMPO INSERCAO: FIM
                clock_t endI = clock();
                metricas.t_pre_processamento =((double)(endI - startI)) / CLOCKS_PER_SEC;
                metricas.n_comparacoes_pre_processamento = compInserir;
                metricas.n_leitura_pre_processamento = transfInserir;

                // for(int i = 0; i < QTDE_CHAVES_TESTE; i++){
                    
                    // CONTAGEM DE TEMPO PESQUISA: INICIO
                startI = clock();

                // valueRetorno = findChave(arq_arv_bin, 0, chaves_geradas[i], &compPesquisa, &transfPesquisa);
                valueRetorno = findChave(arq_arv_bin, 0, entrada.chave_buscada, &compPesquisa, &transfPesquisa);

                // CONTAGEM DE TEMPO PESQUISA : FIM
                endI = clock();
                tempoPesquisa = ((double)(endI - startI)) / CLOCKS_PER_SEC;

                if (valueRetorno){
                    printf("A chave %d esta presente na arvore\n", entrada.chave_buscada);
                    //resultados(tempoInserir, tempoPesquisa, compInserir, compPesquisa, transfInserir, transfPesquisa);
                    //printf("-----------------------------------\n\n");
                }
                else{
                    printf("O valor %d não foi encontrado.\n", entrada.chave_buscada);
                    //resultados(tempoInserir, tempoPesquisa, compInserir, compPesquisa, transfInserir, transfPesquisa);
                    //printf("-----------------------------------\n\n");
                }
                // }
                metricas.t_pesquisa = tempoPesquisa;
                metricas.n_comparacoes_pesquisa = compPesquisa;
                metricas.n_leitura_pesquisa = transfPesquisa;

                exibirMetricas(&metricas);
            }
        break;

        // Arvore B
        case 3:
        {
            Pagina* raiz;

            printf("GERANDO A ARVORE B A PARTIR DO ARQUIVO OFERECIDO...\n\n");
            insereRegistros(arq_bin, &raiz, entrada.quantidade_registros, &metricas);

            if(raiz == NULL)
            {
                printf("Nao foi possivel alocar memoria dinamicamente em alguma parte do processo. Abortando o programa...\n");
                exit(1);
            }

            if (raiz == NULL)
                printf("Nao foi possivel alocar memoria dinamicamente em alguma parte do processo. Abortando o programa...\n");
            else {
                if(argc == 6)
                    printArvoreB(raiz);

                // for(int i = 0 ; i < QTDE_CHAVES_TESTE ; i++)
                //     {
                    reg_retorno.chave = entrada.chave_buscada;

                    if(pesquisaArvore(&reg_retorno, raiz, &metricas))
                        printf("A chave %d foi encontrada no arquivo!\n", entrada.chave_buscada);
                    else
                        printf("A chave passada como argumento (%d) não existe no arquivo.\n", entrada.chave_buscada);
                    // }
            }

            exibirMetricas(&metricas);
            desalocaArvoreB(&raiz);
        }
        break;

        // Arvore B*
        case 4:
        {
            Pagina_ *raiz;

            printf("GERANDO A ARVORE B* A PARTIR DO ARQUIVO OFERECIDO...\n\n");
            raiz = gerarArvoreB_(arq_bin, &entrada, &metricas);

            if(raiz == NULL)
                printf("Nao foi possivel alocar memoria dinamicamente em alguma parte do processo. Abortando o programa...\n");
            else
            {
                if(argc == 6)
                    printArvoreB_(raiz);
                    
                // for(int i = 0 ; i < QTDE_CHAVES_TESTE ; i++)
                // {
                if(arvoreB_(raiz, entrada.chave_buscada, &reg_retorno, &metricas))
                    printf("A chave %d foi encontrada no arquivo!\n", reg_retorno.chave);
                else
                    printf("A chave passada como argumento (%d) não existe no arquivo.\n", entrada.chave_buscada);
                // }
                exibirMetricas(&metricas);

                desalocarArvoreB_(&raiz);
            }
        }
        break;
    }

    fclose(arq_bin);

    return 0;
}
