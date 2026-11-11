#include <stdio.h>
#include "entrada.h"

static FILE *arq = NULL;
static int linha = 1, coluna = 1;
static int linha_ant = 1, coluna_ant = 1;

int entrada_abrir(const char *caminho) {
    arq = fopen(caminho, "r");
    linha = coluna = linha_ant = coluna_ant = 1;
    return arq != NULL;
}

void entrada_fechar(void) {
    if (arq != NULL) {
        fclose(arq);
        arq = NULL;
    }
}

int entrada_ler(void) {
    int c = fgetc(arq);
    linha_ant = linha;
    coluna_ant = coluna;
    if (c == '\n') {
        linha++;
        coluna = 1;
    } else if (c != EOF) {
        coluna++;
    }
    return c;
}

void entrada_devolver(int c) {
    if (c != EOF) ungetc(c, arq);
    linha = linha_ant;
    coluna = coluna_ant;
}

int entrada_espiar(void) {
    int c = fgetc(arq);
    if (c != EOF) ungetc(c, arq);
    return c;
}

int entrada_linha(void)  { return linha; }
int entrada_coluna(void) { return coluna; }
