#ifndef TECLADO_H_
#define TECLADO_H_


extern char tecla;

char teclaDeboucing( char bitColuna, char indColuna, char indLinha);

char ligaLinhas(char bitColuna, char indColuna);

char procuraLinhas(void);

char procuraTecla(void);

char lerTeclado();

#endif
