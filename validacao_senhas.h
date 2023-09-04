#ifndef VALIDACAO_SENHAS_H_
#define VALIDACAO_SENHAS_H_

extern int nr_digitados;
extern int tempoPortasAbertas;
extern int ida;
extern char senha[7];
extern const char* validacao;
extern char admLogin;
extern char estado[16];
extern double parametros[3][4];
extern int portas_abertas;
extern int tempoDeEspera;
extern char estado_viagem[16];

void lendo_senha(char tecla);

const char* validar_senha(void);

int resultado_validacao(void);

void digitarSenha(char teclaInterrupcao);
void tostring(char str[], int num);
void lerSenha();
void edicaoParametros();
void edicaoVel();
void edicaoDist();
void edicaoTempo();
void edicaoTempoParada();
void edicaoTempoViagem();
void selecaoModoMaquinista();
void modoAuto();
void modoManual();
#endif /* VALIDACAO_SENHAS_H_ */