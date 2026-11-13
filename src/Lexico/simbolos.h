#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include "token.h"

typedef enum { CAT_RESERVADA, CAT_IDENTIFICADOR } CategoriaSimbolo;

typedef struct Entrada {
    char *lexema;
    CategoriaSimbolo categoria;
    TipoToken token;
    int indice;
    int primeira_linha;
    int contador;
    struct Entrada *proxima;
} Entrada;

void     simbolos_inicializar(void);
Entrada *simbolos_buscar(const char *lexema);
Entrada *simbolos_inserir(const char *lexema, CategoriaSimbolo categoria,
                          TipoToken token, int linha);
void     simbolos_registrar_uso(Entrada *e, int linha);
Entrada *simbolos_obter(int indice);
void     simbolos_imprimir(void);
void     simbolos_liberar(void);

#endif
