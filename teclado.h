#ifndef TECLADO_H_
#define TECLADO_H_

#define bounce 

char teclado [4][4];

char teclaDeboucing( char bitColuna, char indColuna, char indLinha);

char ligaLinhas(char bitColuna, char indColuna);

char procuraLinhas(void);

char procuraTecla(void);

#endif
