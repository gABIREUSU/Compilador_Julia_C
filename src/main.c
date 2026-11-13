#include <stdio.h>
#include "Lexico/token.h"
#include "Lexico/entrada.h"
#include "Lexico/lexico.h"
#include "Lexico/simbolos.h"
#include "Lexico/erros.h"

static void imprimir_token(const Token *t) {
    printf("%-14s %3d:%-3d  %-12s", token_nome(t->tipo), t->linha, t->coluna, t->lexema);
    switch (t->tipo) {
        case TOK_ID:    printf("simbolo=%d", t->indice_simbolo); break;
        case TOK_INT:   printf("valor=%lld", t->valor_inteiro); break;
        case TOK_FLOAT: printf("valor=%g", t->valor_real); break;
        default: break;
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "uso: %s arquivo.jl\n", argv[0]);
        return 1;
    }
    if (!entrada_abrir(argv[1])) {
        perror(argv[1]);
        return 1;
    }

    simbolos_inicializar();
    lexico_iniciar();

    printf("=== TOKENS ===\n");
    Token t;
    do {
        t = lexico_proximo_token();
        imprimir_token(&t);
    } while (t.tipo != TOK_EOF);

    simbolos_imprimir();
    erros_imprimir();

    int tem_erros = erros_quantidade() > 0;
    simbolos_liberar();
    erros_liberar();
    entrada_fechar();
    return tem_erros ? 1 : 0;
}
