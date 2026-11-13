#ifndef ENTRADA_H
#define ENTRADA_H

int  entrada_abrir(const char *caminho);
void entrada_fechar(void);
int  entrada_ler(void);
void entrada_devolver(int c);
int  entrada_espiar(void);
int  entrada_linha(void);
int  entrada_coluna(void);

#endif
