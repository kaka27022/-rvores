#include "arvoreB.h"

void printArvoreB(Pagina *raiz)
{
    if(raiz == NULL)
        return;

    for(int i = 0 ; i <= raiz->tamanho ; i++)
    {
        printArvoreB(raiz->apontadores[i]);

        if(i < raiz->tamanho)
            printf("%d\n", raiz->registros[i].chave);
    }
}

void inicializaArvoreB(Pagina** arvore) {
    *arvore = NULL;
}

void desalocaArvoreB(Pagina **raiz)
{
    if(*raiz == NULL)
        return;

    for(int i = 0 ; i <= (*raiz)->tamanho ; i++)
        desalocaArvoreB(&(*raiz)->apontadores[i]);

    free(*raiz);
}


bool pesquisaArvore(Registro* x, Pagina* apontador, Metrica *metricas) {
    long int i = 1;
    bool achou;
    clock_t inicio, fim;

    inicio = clock();

    if (apontador == NULL)
        return false;

    metricas->n_comparacoes_pesquisa++;
    while (i < apontador->tamanho && x->chave > apontador->registros[i - 1].chave) {
        if (i != 1) metricas->n_comparacoes_pesquisa++;
        i++;
    }
    
    metricas->n_comparacoes_pesquisa++;
    if (x->chave == apontador->registros[i - 1].chave)
        return true;
    
    metricas->n_comparacoes_pesquisa++;
    if (x->chave < apontador->registros[i - 1].chave)
        achou = pesquisaArvore(x, apontador->apontadores[i - 1], metricas);
    
    else achou = pesquisaArvore(x, apontador->apontadores[i], metricas);

    fim = clock();

    metricas->t_pesquisa += ((double) fim - inicio) / CLOCKS_PER_SEC;

    return achou;
}

void insereRegistros(FILE* arquivo, Pagina** arvore, unsigned int tamanho, Metrica *metricas) {
    Registro item;
    clock_t inicio, fim;

    // ------- INICIO PRE-PROCESSAMENTO ------------ //

    inicio = clock();

    inicializaArvoreB(arvore);

    for (int i = 0; i < tamanho; i++) {
        fseek(arquivo, i * sizeof(Registro), 0);
        fread(&item, sizeof(item), 1, arquivo);
        metricas->n_leitura_pre_processamento++;
        insere(item, arvore, metricas);
    }

    fim = clock();

    metricas->t_pre_processamento += ((double) fim - inicio) / CLOCKS_PER_SEC;
    // ------- FIM PRE-PROCESSAMENTO ------------ //
}

void insere(Registro registro, Pagina** apontador, Metrica *metricas) {
    bool cresceu;
    Registro registroRetorno;
    Pagina *apontadorRetorno, *apontadorTemporario;

    ins(registro, *apontador, &cresceu, &registroRetorno, &apontadorRetorno, metricas);

    if(cresceu) {
        apontadorTemporario = (Pagina *)malloc(sizeof(Pagina));
        apontadorTemporario->tamanho = 1;
        apontadorTemporario->registros[0] = registroRetorno;
        apontadorTemporario->apontadores[1] = apontadorRetorno;
        apontadorTemporario->apontadores[0] = *apontador;
        *apontador = apontadorTemporario;
    }
}


void ins(Registro registro, Pagina* apontador, bool *cresceu, Registro* registroRetorno, Pagina** apontadorRetorno, Metrica *metricas) {
    int i = 1;
    Pagina* apTemp;

    if(apontador == NULL){
        (*cresceu) = true;
        *registroRetorno = registro;
        *apontadorRetorno = NULL;
        return;
    }

    while(i < apontador->tamanho && registro.chave > apontador->registros[i - 1].chave) {
        metricas->n_comparacoes_pre_processamento++;
        i++;
    }

    metricas->n_comparacoes_pre_processamento++;
    if(registro.chave == apontador->registros[i - 1].chave) {
        (*cresceu) = false;
        return;
    }

    metricas->n_comparacoes_pre_processamento++;
    if(registro.chave < apontador->registros[i - 1].chave) {
        i--;
    }

    ins(registro, apontador->apontadores[i], cresceu, registroRetorno, apontadorRetorno, metricas);

    if(!(*cresceu)) {
        return;
    }

    if(apontador->tamanho < MM) {
        insereNaPagina(apontador, *registroRetorno, *apontadorRetorno, metricas);
        (*cresceu) = false;
        return;
    }

    apTemp = (Pagina*)malloc(sizeof(Pagina));
    apTemp->tamanho = 0;
    apTemp->apontadores[0]=NULL;

    if(i < M+1) {
        insereNaPagina(apTemp, apontador->registros[MM - 1], apontador->apontadores[MM], metricas);
        apontador->tamanho--;
        insereNaPagina(apontador, *registroRetorno, *apontadorRetorno, metricas);
    } else {
        insereNaPagina(apTemp, *registroRetorno, *apontadorRetorno, metricas);
    }

    for(int j = M+2; j <= MM; j++)
        insereNaPagina(apTemp, apontador->registros[j - 1], apontador->apontadores[j], metricas);

    apontador->tamanho = M;
    apTemp->apontadores[0] = apontador->apontadores[M + 1];
    *registroRetorno = apontador->registros[M];
    *apontadorRetorno = apTemp;
}


void insereNaPagina(Pagina* apontador, Registro registro, Pagina* apontadorDireita, Metrica *metricas) {
    bool naoEncontrou=true;
    int k;

    k = apontador->tamanho;
    naoEncontrou = (k > 0);

    while(naoEncontrou) {
        metricas->n_comparacoes_pre_processamento++;
        if(registro.chave >= apontador->registros[k - 1].chave) {
            naoEncontrou = false;
        } else {
            metricas->n_comparacoes_pre_processamento++;
            apontador->registros[k] = apontador->registros[k - 1];
            apontador->apontadores[k + 1] = apontador->apontadores[k];              
            k--;                                                                  
            
            if (k < 1)
                naoEncontrou = false;                                               
        }                                                                           
    }
    metricas->n_comparacoes_pre_processamento++;
    apontador->registros[k] = registro;
    apontador->apontadores[k + 1] = apontadorDireita;
    apontador->tamanho++;
}