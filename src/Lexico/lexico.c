#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexico.h"
#include "entrada.h"
#include "erros.h"
#include "simbolos.h"

typedef enum {
    C_LETRA, C_EXP, C_DIGITO, C_PONTO,
    C_MAIS, C_MENOS, C_VEZES, C_BARRA, C_PERCENT, C_CIRCUNFLEXO,
    C_IGUAL, C_MENOR, C_MAIOR, C_EXCLAMACAO, C_ECOMERCIAL, C_BARRA_VERT,
    C_ABRE_PAR, C_FECHA_PAR, C_ABRE_COL, C_FECHA_COL,
    C_VIRGULA, C_PONTO_VIRGULA, C_DOIS_PONTOS,
    C_ASPAS, C_BARRA_INV, C_NEWLINE, C_ESPACO, C_EOF, C_OUTRO,
    N_CLASSES
} Classe;

typedef enum {
    Q_INICIAL, Q_ID, Q_INT, Q_PONTO_NUM, Q_FLOAT,
    Q_EXP, Q_EXP_SINAL, Q_EXP_DIG,
    Q_STR, Q_STR_ESC,
    Q_MENOR, Q_MAIOR, Q_IGUAL, Q_EXCL, Q_ECOM, Q_BVERT,

    F_ID, F_INT, F_FLOAT, F_STRING,
    F_MAIS, F_MENOS, F_VEZES, F_DIV, F_RESTO, F_POTENCIA,
    F_ATRIB, F_IGUAL, F_DIFERENTE,
    F_MENOR, F_MENOR_IGUAL, F_MAIOR, F_MAIOR_IGUAL,
    F_E_LOGICO, F_OU_LOGICO, F_NAO,
    F_ABRE_PAR, F_FECHA_PAR, F_ABRE_COL, F_FECHA_COL,
    F_VIRGULA, F_PONTO_VIRGULA, F_DOIS_PONTOS,
    F_NEWLINE, F_EOF,

    E_CARACTERE, E_STRING, E_NUMERO, E_OPERADOR,
    N_ESTADOS
} Estado;

static Estado     matriz[N_ESTADOS][N_CLASSES];
static TipoToken  token_do_estado[N_ESTADOS];
static int        retrocede[N_ESTADOS];
static CodigoErro erro_do_estado[N_ESTADOS];

static int eh_final(Estado e) { return e >= F_ID && e < E_CARACTERE; }
static int eh_erro(Estado e)  { return e >= E_CARACTERE; }

static void padrao(Estado e, Estado destino) {
    for (int c = 0; c < N_CLASSES; c++) matriz[e][c] = destino;
}
static void trans(Estado e, Classe c, Estado destino) { matriz[e][c] = destino; }
static void def_final(Estado f, TipoToken t, int ret) {
    token_do_estado[f] = t;
    retrocede[f] = ret;
}
static void def_erro(Estado e, CodigoErro cod, int ret) {
    erro_do_estado[e] = cod;
    retrocede[e] = ret;
}

void lexico_iniciar(void) {
    padrao(Q_INICIAL, E_CARACTERE);
    trans(Q_INICIAL, C_LETRA, Q_ID);
    trans(Q_INICIAL, C_EXP, Q_ID);
    trans(Q_INICIAL, C_DIGITO, Q_INT);
    trans(Q_INICIAL, C_MAIS, F_MAIS);
    trans(Q_INICIAL, C_MENOS, F_MENOS);
    trans(Q_INICIAL, C_VEZES, F_VEZES);
    trans(Q_INICIAL, C_BARRA, F_DIV);
    trans(Q_INICIAL, C_PERCENT, F_RESTO);
    trans(Q_INICIAL, C_CIRCUNFLEXO, F_POTENCIA);
    trans(Q_INICIAL, C_IGUAL, Q_IGUAL);
    trans(Q_INICIAL, C_MENOR, Q_MENOR);
    trans(Q_INICIAL, C_MAIOR, Q_MAIOR);
    trans(Q_INICIAL, C_EXCLAMACAO, Q_EXCL);
    trans(Q_INICIAL, C_ECOMERCIAL, Q_ECOM);
    trans(Q_INICIAL, C_BARRA_VERT, Q_BVERT);
    trans(Q_INICIAL, C_ABRE_PAR, F_ABRE_PAR);
    trans(Q_INICIAL, C_FECHA_PAR, F_FECHA_PAR);
    trans(Q_INICIAL, C_ABRE_COL, F_ABRE_COL);
    trans(Q_INICIAL, C_FECHA_COL, F_FECHA_COL);
    trans(Q_INICIAL, C_VIRGULA, F_VIRGULA);
    trans(Q_INICIAL, C_PONTO_VIRGULA, F_PONTO_VIRGULA);
    trans(Q_INICIAL, C_DOIS_PONTOS, F_DOIS_PONTOS);
    trans(Q_INICIAL, C_ASPAS, Q_STR);
    trans(Q_INICIAL, C_NEWLINE, F_NEWLINE);
    trans(Q_INICIAL, C_EOF, F_EOF);

    padrao(Q_ID, F_ID);
    trans(Q_ID, C_LETRA, Q_ID);
    trans(Q_ID, C_EXP, Q_ID);
    trans(Q_ID, C_DIGITO, Q_ID);

    padrao(Q_INT, F_INT);
    trans(Q_INT, C_DIGITO, Q_INT);
    trans(Q_INT, C_PONTO, Q_PONTO_NUM);
    trans(Q_INT, C_EXP, Q_EXP);
    trans(Q_INT, C_LETRA, E_NUMERO);

    padrao(Q_PONTO_NUM, E_NUMERO);
    trans(Q_PONTO_NUM, C_DIGITO, Q_FLOAT);

    padrao(Q_FLOAT, F_FLOAT);
    trans(Q_FLOAT, C_DIGITO, Q_FLOAT);
    trans(Q_FLOAT, C_EXP, Q_EXP);
    trans(Q_FLOAT, C_LETRA, E_NUMERO);
    trans(Q_FLOAT, C_PONTO, E_NUMERO);

    padrao(Q_EXP, E_NUMERO);
    trans(Q_EXP, C_DIGITO, Q_EXP_DIG);
    trans(Q_EXP, C_MAIS, Q_EXP_SINAL);
    trans(Q_EXP, C_MENOS, Q_EXP_SINAL);

    padrao(Q_EXP_SINAL, E_NUMERO);
    trans(Q_EXP_SINAL, C_DIGITO, Q_EXP_DIG);

    padrao(Q_EXP_DIG, F_FLOAT);
    trans(Q_EXP_DIG, C_DIGITO, Q_EXP_DIG);
    trans(Q_EXP_DIG, C_LETRA, E_NUMERO);
    trans(Q_EXP_DIG, C_EXP, E_NUMERO);
    trans(Q_EXP_DIG, C_PONTO, E_NUMERO);

    /* strings */
    padrao(Q_STR, Q_STR);
    trans(Q_STR, C_ASPAS, F_STRING);
    trans(Q_STR, C_BARRA_INV, Q_STR_ESC);
    trans(Q_STR, C_NEWLINE, E_STRING);
    trans(Q_STR, C_EOF, E_STRING);

    padrao(Q_STR_ESC, Q_STR);
    trans(Q_STR_ESC, C_NEWLINE, E_STRING);
    trans(Q_STR_ESC, C_EOF, E_STRING);

    padrao(Q_MENOR, F_MENOR);    trans(Q_MENOR, C_IGUAL, F_MENOR_IGUAL);
    padrao(Q_MAIOR, F_MAIOR);    trans(Q_MAIOR, C_IGUAL, F_MAIOR_IGUAL);
    padrao(Q_IGUAL, F_ATRIB);    trans(Q_IGUAL, C_IGUAL, F_IGUAL);
    padrao(Q_EXCL,  F_NAO);      trans(Q_EXCL,  C_IGUAL, F_DIFERENTE);
    padrao(Q_ECOM,  E_OPERADOR); trans(Q_ECOM,  C_ECOMERCIAL, F_E_LOGICO);
    padrao(Q_BVERT, E_OPERADOR); trans(Q_BVERT, C_BARRA_VERT, F_OU_LOGICO);

    def_final(F_ID, TOK_ID, 1);
    def_final(F_INT, TOK_INT, 1);
    def_final(F_FLOAT, TOK_FLOAT, 1);
    def_final(F_STRING, TOK_STRING, 0);
    def_final(F_MAIS, TOK_MAIS, 0);
    def_final(F_MENOS, TOK_MENOS, 0);
    def_final(F_VEZES, TOK_VEZES, 0);
    def_final(F_DIV, TOK_DIV, 0);
    def_final(F_RESTO, TOK_RESTO, 0);
    def_final(F_POTENCIA, TOK_POTENCIA, 0);
    def_final(F_ATRIB, TOK_ATRIB, 1);
    def_final(F_IGUAL, TOK_IGUAL, 0);
    def_final(F_DIFERENTE, TOK_DIFERENTE, 0);
    def_final(F_MENOR, TOK_MENOR, 1);
    def_final(F_MENOR_IGUAL, TOK_MENOR_IGUAL, 0);
    def_final(F_MAIOR, TOK_MAIOR, 1);
    def_final(F_MAIOR_IGUAL, TOK_MAIOR_IGUAL, 0);
    def_final(F_E_LOGICO, TOK_E_LOGICO, 0);
    def_final(F_OU_LOGICO, TOK_OU_LOGICO, 0);
    def_final(F_NAO, TOK_NAO, 1);
    def_final(F_ABRE_PAR, TOK_ABRE_PAR, 0);
    def_final(F_FECHA_PAR, TOK_FECHA_PAR, 0);
    def_final(F_ABRE_COL, TOK_ABRE_COL, 0);
    def_final(F_FECHA_COL, TOK_FECHA_COL, 0);
    def_final(F_VIRGULA, TOK_VIRGULA, 0);
    def_final(F_PONTO_VIRGULA, TOK_PONTO_VIRGULA, 0);
    def_final(F_DOIS_PONTOS, TOK_DOIS_PONTOS, 0);
    def_final(F_NEWLINE, TOK_NEWLINE, 0);
    def_final(F_EOF, TOK_EOF, 0);

    def_erro(E_CARACTERE, ERR_CARACTERE_INVALIDO, 0);
    def_erro(E_STRING, ERR_STRING_NAO_FECHADA, 0);
    def_erro(E_NUMERO, ERR_NUMERO_MALFORMADO, 0);
    def_erro(E_OPERADOR, ERR_OPERADOR_INCOMPLETO, 1);
}

static Classe classe_de(int c) {
    if (c == EOF) return C_EOF;
    if (c == 'e' || c == 'E') return C_EXP;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') return C_LETRA;
    if (c >= '0' && c <= '9') return C_DIGITO;
    switch (c) {
        case '.':  return C_PONTO;
        case '+':  return C_MAIS;
        case '-':  return C_MENOS;
        case '*':  return C_VEZES;
        case '/':  return C_BARRA;
        case '%':  return C_PERCENT;
        case '^':  return C_CIRCUNFLEXO;
        case '=':  return C_IGUAL;
        case '<':  return C_MENOR;
        case '>':  return C_MAIOR;
        case '!':  return C_EXCLAMACAO;
        case '&':  return C_ECOMERCIAL;
        case '|':  return C_BARRA_VERT;
        case '(':  return C_ABRE_PAR;
        case ')':  return C_FECHA_PAR;
        case '[':  return C_ABRE_COL;
        case ']':  return C_FECHA_COL;
        case ',':  return C_VIRGULA;
        case ';':  return C_PONTO_VIRGULA;
        case ':':  return C_DOIS_PONTOS;
        case '"':  return C_ASPAS;
        case '\\': return C_BARRA_INV;
        case '\n': return C_NEWLINE;
        case ' ': case '\t': case '\r': return C_ESPACO;
        default:   return C_OUTRO;
    }
}

static int eh_delimitador(int c) {
    return c == EOF || c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == ';';
}

static void recuperar(void) {
    while (!eh_delimitador(entrada_espiar())) entrada_ler();
}

static void pular_espacos_e_comentarios(void) {
    for (;;) {
        int c = entrada_espiar();
        if (c == ' ' || c == '\t' || c == '\r') {
            entrada_ler();
        } else if (c == '#') {
            while ((c = entrada_espiar()) != '\n' && c != EOF) entrada_ler();
        } else {
            break;
        }
    }
}

static void reportar(CodigoErro cod, int linha, int coluna, const char *lexema, int c) {
    char trecho[TAM_LEXEMA + 2];
    size_t n = strlen(lexema);
    memcpy(trecho, lexema, n);
    if (c != EOF && isprint(c) && c != ' ') trecho[n++] = (char)c;
    trecho[n] = '\0';
    erros_registrar(cod, linha, coluna, trecho);
}

static Token montar_token(Estado estado, const char *lexema, int linha, int coluna) {
    Token t;
    memset(&t, 0, sizeof t);
    t.tipo = token_do_estado[estado];
    t.linha = linha;
    t.coluna = coluna;
    t.indice_simbolo = -1;
    snprintf(t.lexema, TAM_LEXEMA, "%s", lexema);

    switch (t.tipo) {
    case TOK_ID: {
        Entrada *e = simbolos_buscar(lexema);
        if (e == NULL) {
            e = simbolos_inserir(lexema, CAT_IDENTIFICADOR, TOK_ID, linha);
        }
        simbolos_registrar_uso(e, linha);
        if (e->categoria == CAT_RESERVADA) {
            t.tipo = e->token;
        } else {
            t.indice_simbolo = e->indice;
        }
        break;
    }
    case TOK_INT:
        errno = 0;
        t.valor_inteiro = strtoll(lexema, NULL, 10);
        if (errno == ERANGE) {
            erros_registrar(ERR_NUMERO_FORA_FAIXA, linha, coluna, lexema);
        }
        break;
    case TOK_FLOAT:
        t.valor_real = strtod(lexema, NULL);
        break;
    case TOK_NEWLINE:
        snprintf(t.lexema, TAM_LEXEMA, "\\n");
        break;
    default:
        break;
    }
    return t;
}

Token lexico_proximo_token(void) {
    for (;;) {
        pular_espacos_e_comentarios();

        int linha_ini = entrada_linha();
        int col_ini = entrada_coluna();
        char lexema[TAM_LEXEMA];
        int n = 0;
        Estado estado = Q_INICIAL;

        for (;;) {
            lexema[n] = '\0';
            if (n >= TAM_LEXEMA - 1) {
                erros_registrar(ERR_LEXEMA_LONGO, linha_ini, col_ini, lexema);
                recuperar();
                break;
            }

            int c = entrada_ler();
            Estado novo = matriz[estado][classe_de(c)];

            if (eh_erro(novo)) {
                reportar(erro_do_estado[novo], linha_ini, col_ini, lexema, c);
                if (retrocede[novo]) {
                    entrada_devolver(c);
                } else {
                    if (eh_delimitador(c)) entrada_devolver(c);
                    recuperar();
                }
                break;
            }

            if (eh_final(novo)) {
                if (retrocede[novo]) {
                    entrada_devolver(c);
                } else if (c != EOF) {
                    lexema[n++] = (char)c;
                }
                lexema[n] = '\0';
                return montar_token(novo, lexema, linha_ini, col_ini);
            }

            lexema[n++] = (char)c;
            estado = novo;
        }
    }
}
