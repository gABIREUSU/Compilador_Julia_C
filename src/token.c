#include "token.h"

static const char *nomes[TOK_QTD] = {
    [TOK_EOF] = "EOF",
    [TOK_NEWLINE] = "NEWLINE",
    [TOK_ID] = "ID",
    [TOK_INT] = "INT",
    [TOK_FLOAT] = "FLOAT",
    [TOK_STRING] = "STRING",
    [TOK_IF] = "IF",
    [TOK_ELSEIF] = "ELSEIF",
    [TOK_ELSE] = "ELSE",
    [TOK_WHILE] = "WHILE",
    [TOK_FOR] = "FOR",
    [TOK_IN] = "IN",
    [TOK_FUNCTION] = "FUNCTION",
    [TOK_RETURN] = "RETURN",
    [TOK_END] = "END",
    [TOK_BREAK] = "BREAK",
    [TOK_CONTINUE] = "CONTINUE",
    [TOK_TRUE] = "TRUE",
    [TOK_FALSE] = "FALSE",
    [TOK_MAIS] = "MAIS",
    [TOK_MENOS] = "MENOS",
    [TOK_VEZES] = "VEZES",
    [TOK_DIV] = "DIV",
    [TOK_RESTO] = "RESTO",
    [TOK_POTENCIA] = "POTENCIA",
    [TOK_ATRIB] = "ATRIB",
    [TOK_IGUAL] = "IGUAL",
    [TOK_DIFERENTE] = "DIFERENTE",
    [TOK_MENOR] = "MENOR",
    [TOK_MENOR_IGUAL] = "MENOR_IGUAL",
    [TOK_MAIOR] = "MAIOR",
    [TOK_MAIOR_IGUAL] = "MAIOR_IGUAL",
    [TOK_E_LOGICO] = "E_LOGICO",
    [TOK_OU_LOGICO] = "OU_LOGICO",
    [TOK_NAO] = "NAO",
    [TOK_ABRE_PAR] = "ABRE_PAR",
    [TOK_FECHA_PAR] = "FECHA_PAR",
    [TOK_ABRE_COL] = "ABRE_COL",
    [TOK_FECHA_COL] = "FECHA_COL",
    [TOK_VIRGULA] = "VIRGULA",
    [TOK_PONTO_VIRGULA] = "PONTO_VIRGULA",
    [TOK_DOIS_PONTOS] = "DOIS_PONTOS"
};

const char *token_nome(TipoToken tipo) {
    if (tipo < 0 || tipo >= TOK_QTD) return "?";
    return nomes[tipo];
}
