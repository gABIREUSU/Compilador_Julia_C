#ifndef TOKEN_H
#define TOKEN_H

#define TAM_LEXEMA 256

typedef enum {
    TOK_EOF,
    TOK_NEWLINE,
    TOK_ID,
    TOK_INT,
    TOK_FLOAT,
    TOK_STRING,

    TOK_IF, TOK_ELSEIF, TOK_ELSE, TOK_WHILE, TOK_FOR, TOK_IN,
    TOK_FUNCTION, TOK_RETURN, TOK_END, TOK_BREAK, TOK_CONTINUE,
    TOK_TRUE, TOK_FALSE,

    TOK_MAIS, TOK_MENOS, TOK_VEZES, TOK_DIV, TOK_RESTO, TOK_POTENCIA,
    TOK_ATRIB, TOK_IGUAL, TOK_DIFERENTE,
    TOK_MENOR, TOK_MENOR_IGUAL, TOK_MAIOR, TOK_MAIOR_IGUAL,
    TOK_E_LOGICO, TOK_OU_LOGICO, TOK_NAO,

    TOK_ABRE_PAR, TOK_FECHA_PAR, TOK_ABRE_COL, TOK_FECHA_COL,
    TOK_VIRGULA, TOK_PONTO_VIRGULA, TOK_DOIS_PONTOS,
    TOK_QTD
} TipoToken;

typedef struct {
    TipoToken tipo;
    int linha;
    int coluna;
    char lexema[TAM_LEXEMA];
    int indice_simbolo;
    long long valor_inteiro;
    double valor_real;
} Token;

const char *token_nome(TipoToken tipo);

#endif
