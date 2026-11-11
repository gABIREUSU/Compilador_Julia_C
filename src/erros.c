#include <stdio.h>
#include <stdlib.h>
#include "erros.h"

#define TAM_TRECHO 64

typedef struct {
    CodigoErro codigo;
    int linha;
    int coluna;
    char trecho[TAM_TRECHO];
} RegistroErro;

static RegistroErro *lista = NULL;
static int qtd = 0;
static int capacidade = 0;

static const char *mensagens[ERR_QTD] = {
    [ERR_CARACTERE_INVALIDO]  = "caractere invalido",
    [ERR_STRING_NAO_FECHADA]  = "string nao fechada",
    [ERR_NUMERO_MALFORMADO]   = "numero malformado",
    [ERR_OPERADOR_INCOMPLETO] = "operador incompleto",
    [ERR_LEXEMA_LONGO]        = "lexema maior que o limite",
    [ERR_NUMERO_FORA_FAIXA]   = "numero inteiro fora da faixa"
};

void erros_registrar(CodigoErro codigo, int linha, int coluna, const char *trecho) {
    if (qtd == capacidade) {
        int nova = (capacidade == 0) ? 8 : capacidade * 2;
        RegistroErro *tmp = realloc(lista, nova * sizeof *lista);
        if (tmp == NULL) {
            fprintf(stderr, "sem memoria\n");
            exit(1);
        }
        lista = tmp;
        capacidade = nova;
    }
    RegistroErro *r = &lista[qtd++];
    r->codigo = codigo;
    r->linha = linha;
    r->coluna = coluna;
    snprintf(r->trecho, TAM_TRECHO, "%s", trecho);
}

int erros_quantidade(void) { return qtd; }

void erros_imprimir(void) {
    printf("\nERROS LEXICOS (%d)\n", qtd);
    for (int i = 0; i < qtd; i++) {
        printf("linha %d, coluna %d: %s ('%s')\n",
               lista[i].linha, lista[i].coluna,
               mensagens[lista[i].codigo], lista[i].trecho);
    }
}

void erros_liberar(void) {
    free(lista);
    lista = NULL;
    qtd = capacidade = 0;
}
