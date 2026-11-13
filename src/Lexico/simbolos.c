#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

#define TAM_BALDES 211

static Entrada *baldes[TAM_BALDES];
static Entrada **ordem = NULL;
static int qtd = 0;
static int capacidade = 0;

typedef struct {
    const char *palavra;
    TipoToken token;
} Reservada;

static const Reservada reservadas[] = {
    {"if", TOK_IF},         {"elseif", TOK_ELSEIF},   {"else", TOK_ELSE},
    {"while", TOK_WHILE},   {"for", TOK_FOR},         {"in", TOK_IN},
    {"function", TOK_FUNCTION}, {"return", TOK_RETURN}, {"end", TOK_END},
    {"break", TOK_BREAK},   {"continue", TOK_CONTINUE},
    {"true", TOK_TRUE},     {"false", TOK_FALSE}
};

static void *alocar(size_t n) {
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "sem memoria\n");
        exit(1);
    }
    return p;
}

static char *duplicar(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = alocar(n);
    memcpy(p, s, n);
    return p;
}

static unsigned long funcao_hash(const char *s) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*s++)) {
        h = h * 33 + c;
    }
    return h;
}

Entrada *simbolos_buscar(const char *lexema) {
    Entrada *e = baldes[funcao_hash(lexema) % TAM_BALDES];
    while (e != NULL) {
        if (strcmp(e->lexema, lexema) == 0) return e;
        e = e->proxima;
    }
    return NULL;
}

Entrada *simbolos_inserir(const char *lexema, CategoriaSimbolo categoria,
                          TipoToken token, int linha) {
    Entrada *e = alocar(sizeof *e);
    unsigned long b = funcao_hash(lexema) % TAM_BALDES;

    e->lexema = duplicar(lexema);
    e->categoria = categoria;
    e->token = token;
    e->indice = qtd;
    e->primeira_linha = linha;
    e->contador = 0;
    e->proxima = baldes[b];
    baldes[b] = e;

    if (qtd == capacidade) {
        int nova = (capacidade == 0) ? 32 : capacidade * 2;
        Entrada **tmp = realloc(ordem, nova * sizeof *ordem);
        if (tmp == NULL) {
            fprintf(stderr, "sem memoria\n");
            exit(1);
        }
        ordem = tmp;
        capacidade = nova;
    }
    ordem[qtd++] = e;
    return e;
}

void simbolos_inicializar(void) {
    size_t n = sizeof reservadas / sizeof reservadas[0];
    for (size_t i = 0; i < n; i++) {
        simbolos_inserir(reservadas[i].palavra, CAT_RESERVADA, reservadas[i].token, 0);
    }
}

void simbolos_registrar_uso(Entrada *e, int linha) {
    if (e->contador == 0) e->primeira_linha = linha;
    e->contador++;
}

Entrada *simbolos_obter(int indice) {
    if (indice < 0 || indice >= qtd) return NULL;
    return ordem[indice];
}

void simbolos_imprimir(void) {
    printf("\nTABELA DE SIMBOLOS\n");
    printf("%-6s %-14s %-14s %-8s %-8s %s\n",
           "indice", "lexema", "categoria", "token", "1a linha", "usos");
    for (int i = 0; i < qtd; i++) {
        Entrada *e = ordem[i];
        if (e->categoria == CAT_RESERVADA && e->contador == 0) continue;
        printf("%-6d %-14s %-14s %-8s %-8d %d\n",
               e->indice, e->lexema,
               e->categoria == CAT_RESERVADA ? "reservada" : "identificador",
               token_nome(e->token), e->primeira_linha, e->contador);
    }
}

void simbolos_liberar(void) {
    for (int i = 0; i < qtd; i++) {
        free(ordem[i]->lexema);
        free(ordem[i]);
    }
    free(ordem);
    ordem = NULL;
    qtd = capacidade = 0;
    memset(baldes, 0, sizeof baldes);
}
