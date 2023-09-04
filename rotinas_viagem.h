#ifndef ROTINAS_VIAGEM_H_
#define ROTINAS_VIAGEM_H_

extern int tempoPortasAbertas;
extern int ida;
extern double parametros[3][4];
extern int portas_abertas;
extern int tempoDeEspera;
extern char estado_viagem[16];


void abrePortasEspera();
void realizar_viagem(int estacao);
void digitarSenha(char teclaInterrupcao);
void tostring(char str[], int num);
void modoAuto();
void modoManual();
#endif /* ROTINAS_VIAGEM_H_ */