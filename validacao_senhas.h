#ifndef VALIDACAO_SENHAS_H_
#define VALIDACAO_SENHAS_H_

extern int nr_digitados;
extern char senha[7];
extern const char* validacao;
extern char admLogin;
extern char estado[16];
extern double parametros[3][4];


void lendo_senha(char tecla);

const char* validar_senha(void);

int resultado_validacao(void);

void digitarSenha(char teclaInterrupcao);
void lerSenha();
void edicaoParametros();
void edicaoVel();
void edicaoDist();
void edicaoTempo();
void edicaoTempoParada();
void edicaoTempoViagem();

#endif /* VALIDACAO_SENHAS_H_ */