#ifndef ERROS_H
#define ERROS_H

typedef enum {
    ERR_CARACTERE_INVALIDO,
    ERR_STRING_NAO_FECHADA,
    ERR_NUMERO_MALFORMADO,
    ERR_OPERADOR_INCOMPLETO,
    ERR_LEXEMA_LONGO,
    ERR_NUMERO_FORA_FAIXA,
    ERR_QTD
} CodigoErro;

void erros_registrar(CodigoErro codigo, int linha, int coluna, const char *trecho);
int  erros_quantidade(void);
void erros_imprimir(void);
void erros_liberar(void);

#endif
